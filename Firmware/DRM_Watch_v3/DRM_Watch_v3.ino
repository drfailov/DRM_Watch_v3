#include <Arduino.h>
#include <U8g2lib.h>
#include <ESP32Time.h>

#ifdef U8X8_HAVE_HW_SPI
#include <SPI.h>
#endif
#ifdef U8X8_HAVE_HW_I2C
#include <Wire.h>
#endif
bool black = 0;
bool white = 1;

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
#define LED_TOP_PIN GPIO_NUM_33
#define LED_BOTTOM_PIN GPIO_NUM_34
#define LED_STATUS_PIN 35
#define TOUCH1_PIN 8
#define TOUCH2_PIN 9
#define TOUCH3_PIN 10
#define CHARGER_EN_PIN 37

int autoReturnTime = 120000;//ms
int items =0;     //global for menus
int selected = 0; //global for menus

typedef void (*Runnable)();
RTC_DATA_ATTR Runnable modeSetup;
Runnable modeLoop = 0;
Runnable modeButtonUp = 0;
Runnable modeButtonCenter = 0;
Runnable modeButtonDown = 0;
Runnable modeButtonUpLong = 0;
Runnable modeButtonCenterLong = 0;
Runnable modeButtonDownLong = 0;


void setup(void) {
  Serial.begin(115200);
  initPreferences();
  lcdInit();
  initButtons();
  initLed();
  buzzerInit();
  if(esp_sleep_get_wakeup_cause() == 0){
    delay(1000);
    setModeWatchface();
  }
  else{
    modeSetup();
    if(esp_sleep_get_wakeup_cause() == ESP_SLEEP_WAKEUP_EXT0)/*By button*/
      buttonBeep();
  }
}

void loop(void) {
  buttonsLoop();
  if(modeLoop != 0)
    modeLoop();
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
  //lcd()->setFont(u8g2_font_10x20_tf);
  //lcd()->drawStr(330, 125, "UNLOCK");
  lcd()->sendBuffer();
  delay(50);
  
  
  esp_sleep_enable_ext0_wakeup(BUT_CENTER, 0); //1 = High, 0 = Low
  esp_sleep_enable_timer_wakeup(28 * 1000000ULL);
  esp_deep_sleep_start();
}

void doNothing(){} //to use as reference in modes for buttons











//----------------------//----------------------//----------------------//----------------------//----------------------//----------------------//----------------------



