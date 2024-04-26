/*
 *  Project     DRM Watch 3
 *  @author     Dr.  Failov
 *  @link       https://github.com/drfailov/DRM_Watch_v3
 *  @license    GNU GENERAL PUBLIC LICENSE
 *
 *  
 */



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
//#define HW_REV_2
#define HW_REV_3

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

#ifdef HW_REV_3
#define LCD_CLK 7
#define LCD_SDA 11
#define LCD_CS 5
#define LCD_EN GPIO_NUM_6
#define BACKLIGHT_EN GPIO_NUM_8
#define BUT_UP GPIO_NUM_0 //Active LOW
#define BUT_CENTER GPIO_NUM_1 //Active LOW
#define BUT_DOWN GPIO_NUM_2 //Active LOW
#define SENS_USB_PIN 3
#define SENS_BATERY_PIN 4
#define BUZZER_PIN 12
#define LED_TOP_PIN GPIO_NUM_36      //NEW
#define LED_BOTTOM_PIN GPIO_NUM_37   //NEW
#define LED_STATUS_PIN GPIO_NUM_38   //NEW
#define RTC_SDA_PIN 33    
#define RTC_SCL_PIN 35    
#endif

#define W 400
#define H 240
const int BUFF_SCALE = 2;  //screenBuffer  //2 is 3KB out of 8KB RTC Memory
const int BUFF_W = W/BUFF_SCALE;   //screenBuffer
const int BUFF_H = H/BUFF_SCALE;   //screenBuffer

String version = "FW:v2.12";          //================================== <<<<< VERSION
bool black = 1;
bool white = 0;

bool firstDraw = false;  //global for menus, used to animate first frame of any mode. Resets on clearScreenAnimation.
int items = 0;    //global for menus
int selected = 0; //global for menus
int roundness = 4; //global for menus
int frame = 2; //global for menus

typedef void (*Runnable)();
typedef void (*WatchfaceDrawable)(bool firstDraw);
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
  if(esp_sleep_get_wakeup_cause() == ESP_SLEEP_WAKEUP_EXT0/*By button*/) buttonBeep();
  lcdInit();
  backlightInit();
  initButtons();
  initLed();
  if(esp_sleep_get_wakeup_cause() == 0) drawMessage("Ініціалізація RTC...");
  initRtc();
  initTime();
  
  if(esp_sleep_get_wakeup_cause() == 0){
    if(isButtonDownPressed()){
      setModeTest();
      return;
    }
    lcd()->setColorIndex(white);
    lcd()->drawBox(0, 0, 400, 240);
    lcd()->setFont(u8g2_font_10x20_t_cyrillic);  //ok
    lcd()->setColorIndex(black);
    lcd()->setCursor(316, 234); 
    lcd()->print(version);
    int x=130;
    displayDrawVector(getPathZubat(), x, 60, 3.0, 3, 0, black);
    lcd()->sendBuffer();
    if(!isBatteryCritical()){
      ledSelftest();
      playInit();
    }
    while(x>40){
      displayDrawVector(getPathZubat(), x, 60, 3.0, 3, 0, white);
      x-=15;
      displayDrawVector(getPathZubat(), x, 60, 3.0, 3, 0, black);
      lcd()->sendBuffer();
    }
    displayDrawVector(getPathDrmWatch(), 190, 60, 3.0, 2, 3, black);
    lcd()->setCursor(340, 164); lcd()->setFont(u8g2_font_logisoso38_tn);  lcd()->print("3");
    lcd()->sendBuffer();
    delay(300);
    firstDraw = true;
    setModeWatchface();
  }
  else{
    modeSetup();
  }
}

void loop(void) {
  alertLoop();
  timerLoop();
  buttonsLoop();
  backlightLoop();
  if(modeLoop != 0){
     //unsigned long millisStarted = millis();  //routine needed to measure performance
    modeLoop();    //125ms 1MHz SPI  |   63ms  3MHz SPI
    firstDraw = false;
    // unsigned long millisEnd = millis();
    // Serial.print("Loop: "); Serial.print(millisEnd-millisStarted); Serial.println("ms.");
  }
  autoSleepLoop();
}






void doNothing(){} //to use as reference in modes for buttons











//----------------------//----------------------//----------------------//----------------------//----------------------//----------------------//----------------------



