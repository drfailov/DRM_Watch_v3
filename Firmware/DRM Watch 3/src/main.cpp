/*
 *  Project     DRM Watch 3
 *  @author     Dr.  Failov
 *  @link       https://github.com/drfailov/DRM_Watch_v3
 *  @license    GNU GENERAL PUBLIC LICENSE
 *
 *  
 */


#include "Global.h"
#include "DrmPreferences.h"
#include "Battery.h"
#include "Buzzer.h"
#include "RTC.h"
#include "Led.h"
#include "Lcd.h"
#include "TimeSync.h"
#include "Backlight.h"
#include "Button.h"
#include "ModeTest.h"
#include "ModeAlertSettings.h"
#include "ModeTimer.h"
#include "ModeWatchface.h"
#include "SerialCommands.h"

#include <Arduino.h>

#include <ESP32Time.h>
#ifdef U8X8_HAVE_HW_SPI
#include <SPI.h>
#endif
#ifdef U8X8_HAVE_HW_I2C
#include <Wire.h>
#endif


void setup(void) {
  if(esp_sleep_get_wakeup_cause() != ESP_SLEEP_WAKEUP_TIMER && !isOff())/*NOT periodical wakeup*/
    Serial.begin(115200);   
  initPreferences();
  black = getBlackValue(); //load from memory
  white = getWhiteValue(); //load from memory
  if(readSensBatteryVoltage() < 3100){  //if deelpy discharged - go to sleep to let it charge
    setModeOff();
    return;
  }
  buzzerInit();
  if(esp_sleep_get_wakeup_cause() == ESP_SLEEP_WAKEUP_EXT0/*By button*/) buttonBeep();
  lcdInit();
  backlightInit();
  initButtons();
  initLed();
  if(esp_sleep_get_wakeup_cause() == 0 && !isOff()) drawMessageAnimated("Ініціалізація RTC...");
  initRtc();
  initTime();

  if(esp_sleep_get_wakeup_cause() == 0){  //normal wakeup with animation and sound
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
    if(readSensBatteryVoltage() > 3100){
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
  else{   //wakeup only to draw next frame
    modeSetup();
  }
}

void loop(void) {
  buttonsLoop();
  backlightLoop();
  alertLoop();
  timerLoop();
  serialLoop();
  if(modeLoop != 0){
     //unsigned long millisStarted = millis();  //routine needed to measure performance
    modeLoop();    //125ms 1MHz SPI  |   63ms  3MHz SPI
    firstDraw = false;
    // unsigned long millisEnd = millis();
    // Serial.print("Loop: "); Serial.print(millisEnd-millisStarted); Serial.println("ms.");
  }
  autoSleepLoop();
}













//----------------------//----------------------//----------------------//----------------------//----------------------//----------------------//----------------------



