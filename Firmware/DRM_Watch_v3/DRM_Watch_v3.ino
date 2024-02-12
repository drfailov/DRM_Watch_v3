#include <Arduino.h>
#include <U8g2lib.h>
#include <ESP32Time.h>
#ifdef U8X8_HAVE_HW_SPI
#include <SPI.h>
#endif
#ifdef U8X8_HAVE_HW_I2C
#include <Wire.h>
#endif

/*Do not use pins!
 D17 - Stays ON when firmware update
 D31 - Used for flash communication
 D32 - Used for flash communication
*/
//#define HW_REV_1
#define HW_REV_2

#ifdef HW_REV_1
#define LCD_CLK 7
#define LCD_SDA 11
#define LCD_CS 5
#define LCD_EN GPIO_NUM_6
#define BUT_UP GPIO_NUM_0 //Active LOW
#define BUT_CENTER GPIO_NUM_1 //Active LOW
#define BUT_DOWN GPIO_NUM_2 //Active LOW
#define SENS_USB_PIN 3
#define SENS_BATERY_PIN 4
#define BUZZER_PIN 12
#define LED_TOP_PIN GPIO_NUM_33      //OLD
#define LED_BOTTOM_PIN GPIO_NUM_34   //OLD
#define LED_STATUS_PIN 35            //OLD
#define TOUCH1_PIN 8
#define TOUCH2_PIN 9
#define TOUCH3_PIN 10
#define CHARGER_EN_PIN 37  //OLD
#endif

#ifdef HW_REV_2
#define LCD_CLK 7
#define LCD_SDA 11
#define LCD_CS 5
#define LCD_EN GPIO_NUM_6
#define BUT_UP GPIO_NUM_0 //Active LOW
#define BUT_CENTER GPIO_NUM_1 //Active LOW
#define BUT_DOWN GPIO_NUM_2 //Active LOW
#define SENS_USB_PIN 3
#define SENS_BATERY_PIN 4
#define BUZZER_PIN 12
#define LED_TOP_PIN GPIO_NUM_36      //NEW
#define LED_BOTTOM_PIN GPIO_NUM_37   //NEW
#define LED_STATUS_PIN GPIO_NUM_38   //NEW
#define TOUCH1_PIN 8
#define TOUCH2_PIN 9
#define TOUCH3_PIN 10
#define RTC_SDA_PIN 33    
#define RTC_SCL_PIN 35    
#endif


String version = "v0.26";        //================================== <<<<< VERSION
bool black = 1;
bool white = 0;

bool dontSleep = false;
const int autoReturnDefaultTime = 120000;//ms
const int autoSleepDefaultTime = 15000;//ms
const int autoSleepDefaultTimeWhenFlashlightOn = 300000;//ms
bool enableAutoReturn = false; //is set when new mode selected
bool enableAutoSleep = false; //is set when new mode selected
int autoReturnTime = autoReturnDefaultTime;//ms
int autoSleepTime = autoSleepDefaultTime;//ms

int items =0;     //global for menus
int selected = 0; //global for menus

typedef void (*Runnable)();
typedef void (*Drawable)(int x,int y, bool color);
RTC_DATA_ATTR Runnable modeSetup;
Runnable modeLoop = 0;
Runnable modeButtonUp = 0;
Runnable modeButtonCenter = 0;
Runnable modeButtonDown = 0;
Runnable modeButtonUpLong = 0;
Runnable modeButtonCenterLong = 0;
Runnable modeButtonDownLong = 0;
#define BUFFER_SIZE 40  //размер текстового буфера. Чем меньше тем экономнее.
char buffer[BUFFER_SIZE];   //общий на всю программу текстовый буфер чтобы не объявлять каждый раз локальную.


void setup(void) {
  if(esp_sleep_get_wakeup_cause() != ESP_SLEEP_WAKEUP_TIMER && !isOff())/*NOT periodical wakeup*/
    Serial.begin(115200);  
  initPreferences();
  black = getBlackValue(); //load from memory
  white = getWhiteValue(); //load from memory
  buzzerInit();
  initRtc();
  initTime();
  lcdInit();
  initButtons();
  initLed();
  if(esp_sleep_get_wakeup_cause() == 0){
    lcd()->setColorIndex(white);
    lcd()->drawBox(0, 0, 400, 240);
    lcd()->setFont(u8g2_font_10x20_t_cyrillic);  //ok
    lcd()->setColorIndex(black);
    lcd()->setCursor(345, 234); 
    lcd()->print(version);
    int x=130;
    displayDrawVector(getPathZubat(), x, 60, 3.0, 3, 0, black);
    lcd()->sendBuffer();
    ledSelftest();
    playInit();
    while(x>40){
      displayDrawVector(getPathZubat(), x, 60, 3.0, 3, 0, white);
      x-=15;
      displayDrawVector(getPathZubat(), x, 60, 3.0, 3, 0, black);
      lcd()->sendBuffer();
    }
    displayDrawVector(getPathDrmWatch(), 190, 60, 3.0, 2, 4, black);
    lcd()->sendBuffer();
    delay(300);
    setModeWatchface();
  }
  else{
    modeSetup();
    if(esp_sleep_get_wakeup_cause() == ESP_SLEEP_WAKEUP_EXT0){/*By button*/
      buttonBeep();
    }
  }
}

void loop(void) {
  for(int alertIndex=0; alertIndex<getAlertsNumber(); alertIndex++){ //Обработка будильника 
    //play melody and mark this day as playen if:  alert enabled, in this day was not playen, this is right time to play
    bool alertIsEnabled = getAlertEnabled(alertIndex);
    int alertLastRunDay = getAlertLastRunDay(alertIndex);
    int alertTimeHour = getAlertHour(alertIndex);
    int alertTimeMinute = getAlertMinute(alertIndex);
    int alertMelodyIndex = getAlertMelody(alertIndex);
    int hour = rtcGetHour();
    int minute = rtcGetMinute();
    int day = rtcGetDay();
    if (alertIsEnabled) {
      if (alertLastRunDay != day) {
        if ((hour == alertTimeHour && minute >= alertTimeMinute) || (hour > alertTimeHour)) {
          saveAlertLastRunDay(alertIndex, day);
          long timeStarted = millis();
          long playTime = 180000;
          sprintf(buffer, "Alert %d (%02d:%02d)", alertIndex, alertTimeHour, alertTimeMinute);
          melodyPlayerSetMelodyName(String(buffer));
          while (melodyPlayerPlayMelody(getMelodyData(alertMelodyIndex)) && millis() - timeStarted < playTime);
        }
      }
    }
  }

  buttonsLoop();
  if(modeLoop != 0){
    // unsigned long millisStarted = millis();
    modeLoop();    //usually kakes about 125 ... 150ms
    // unsigned long millisEnd = millis();
    // Serial.print("Loop: "); Serial.print(millisEnd-millisStarted); Serial.println("ms.");
  }
  
  if(enableAutoReturn && sinceLastAction() > autoReturnTime && !dontSleep) //auto go to watchface
    setModeWatchface();
  
  int _autoSleepTime = isFlashlightOn()?autoSleepDefaultTimeWhenFlashlightOn:autoSleepTime;
  if(enableAutoSleep && sinceLastAction() > _autoSleepTime && !dontSleep && !isChargerConnected()) //auto go to sleep
    goToSleep();

  if(esp_sleep_get_wakeup_cause() == ESP_SLEEP_WAKEUP_TIMER)/*periodical wakeup*/
    goToSleep();
}






void goToSleep(){
  /*
  Battery Capacity: 250mA
  Deep sleep mode: 0.081mA  (Time = 250/0.081 = 3086h = 128d = 4.2month)
  Active mode: 40mA (Time = 250/40 = 6.25h)
  Music mode: 97mA  (Time = 250/97 = 2.57h)
  */
  lcd()->setColorIndex(0);
  
  draw_ic24_empty(lx(), ly1(), black);
  draw_ic24_unlock(lx(), ly2(), black);
  draw_ic24_empty(lx(), ly3(), black);
  lcd()->sendBuffer();
  delay(20);
  
  
  esp_sleep_enable_ext0_wakeup(BUT_CENTER, 0); //1 = High, 0 = Low
  
  if(isOff())
    esp_sleep_enable_timer_wakeup(600 * 1000000ULL);
  else
    esp_sleep_enable_timer_wakeup(55 * 1000000ULL);
  esp_deep_sleep_start();
}

void doNothing(){} //to use as reference in modes for buttons











//----------------------//----------------------//----------------------//----------------------//----------------------//----------------------//----------------------



