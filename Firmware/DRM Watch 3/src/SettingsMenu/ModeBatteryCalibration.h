#ifndef ModeBatteryCalibration_H
#define ModeBatteryCalibration_H

/*PROTOTYPES*/
void setModeBatteryCalibration();
void ModeBatteryCalibrationLoop();
void ModeBatteryCalibrationButtonUp();
void ModeBatteryCalibrationButtonCenter();
void ModeBatteryCalibrationButtonDown();

#include <Arduino.h>
#include "Global.h"
#include "AutoSleep.h"
#include "Button.h"
#include "Battery.h"
#include "DrmPreferences.h"




void setModeBatteryCalibration(){
  if(modeExit != 0)
    modeExit();
  clearScreenAnimation();
  Serial.println(F("Set mode: Battery Calibration"));
  modeSetup = setModeBatteryCalibration;
  modeLoop = ModeBatteryCalibrationLoop;
  modeButtonUp = ModeBatteryCalibrationButtonUp;
  modeButtonCenter = ModeBatteryCalibrationButtonCenter;
  modeButtonDown = ModeBatteryCalibrationButtonDown;
  modeButtonUpLong = 0;
  modeButtonCenterLong = 0;
  modeButtonDownLong = 0;
  registerAction();
  enableAutoReturn = false;
  enableAutoSleep = false; 
  autoReturnTime = autoReturnDefaultTime;
  autoSleepTime = autoSleepDefaultTime;
}

void drawPlot(int x, int y, int w, int h, int16_t* values, int length){
  //config
  int padding = 15;

  //frame
  lcd()->setColorIndex(white);
  lcd()->drawBox(x, y, w, h);
  lcd()->setColorIndex(black);
  lcd()->drawFrame(x, y, w, h);
  
  //min and max
  int16_t min = values[0];
  int16_t max = values[0];
  for(int i = 1; i < length; i++){
    if(values[i] < min)
      min = values[i];
    if(values[i] > max)
      max = values[i];
  }
  lcd()->setFont(u8g2_font_unifont_t_cyrillic); //smalll
  lcd()->setCursor(x+2, y+13);
  lcd()->print(max);
  lcd()->setCursor(x+2, y+h-2);
  lcd()->print(min);
  
  //plot line
  float maxPoint = y+padding;
  float minPoint = y+h-padding;
  float leftPoint = x;
  float rightPoint = x+w;
  float lastX = map(0, 0, length, leftPoint, rightPoint);
  float lastY = map(values[0], min, max, minPoint, maxPoint);
  for(int i = 1; i < length; i++){
    float cx = map(i, 0, length, leftPoint, rightPoint);
    float cy = map(values[i], min, max, minPoint, maxPoint);
    lcd()->drawLine(cx, cy, lastX, lastY);
    lastX = cx;
    lastY = cy;
  }
}

void ModeBatteryCalibrationLoop(){ 
  lcd()->setColorIndex(white);
  lcd()->drawBox(0, 0, 400, 240);
  
  
  
  //int y=2;
  //int x=5;
  //int interval = 13;
  lcd()->setColorIndex(black);
  drawCentered("Зарядіть перед калібруванням!", 15);
  drawCentered("Калібрування займе кілька годин.", 30);

  
  lcd()->setFont(u8g2_font_unifont_t_cyrillic); //smalll
  lcd()->setCursor(10, 50); lcd()->print("RAW: "); lcd()->print(readSensBatteryRaw());
  lcd()->setCursor(10, 65); lcd()->print("Total: "); lcd()->print(batteryCalibrationLength());
  lcd()->setCursor(10, 80); lcd()->print("isBatteryCalibrated(): "); lcd()->print(isBatteryCalibrated());

  drawPlot(10, 100, 350, 110, batteryCalibration, batteryCalibrationLength());

  // y+=interval; lcd()->setCursor(x, y); lcd()->print("Internal RTC: "); lcd()->print(_rtcInternal()->getTime("%d %b %Y %H:%M:%S"));  lcd()->print(" ("); lcd()->print(_rtcInternal()->getEpoch());   lcd()->print(")");
  // y+=interval; lcd()->setCursor(x, y); lcd()->print("External RTC: "); printRtcGetTimeRaw();   
  // y+=interval; lcd()->setCursor(x, y); lcd()->print("SinceLastSync :"); displayPrintSecondsAsTime(rtcGetUtcEpoch()-getLastTimeSync());  lcd()->print(" Last sync:"); lcd()->print(getLastTimeSync());   
  // y+=interval; lcd()->setCursor(x, y); lcd()->print("RTC CLK SRC: "); lcd()->print(getRtcSrc());

  // y+=interval; lcd()->setCursor(x, y); lcd()->print("Wakeup_reason: "); wakeup_reason();
  // y+=interval; lcd()->setCursor(x, y); lcd()->print("Millis:"); lcd()->print(millis()); lcd()->print(",   Don't sleep: "); lcd()->print(dontSleep);

  // y+=interval; lcd()->setCursor(x, y); lcd()->print("RAW ");  lcd()->print("BATTERY: "); lcd()->print(readSensBatteryRaw()); lcd()->print(", USB:"); lcd()->print(readSensUsbRaw()); lcd()->print(" : "); lcd()->print(isChargerConnected());
  // y+=interval; lcd()->setCursor(x, y); lcd()->print("VOLTAGE ");  lcd()->print("BATTERY: "); lcd()->print(readSensBatteryVoltage()); lcd()->print(" ("); lcd()->print(batteryBars()); lcd()->print(" bars)"); 
  // y+=interval; lcd()->setCursor(x, y); lcd()->print("MIN voltage: ");  lcd()->print(getBatteryMinVoltage());  lcd()->print(", MAX voltage: ");  lcd()->print(getBatteryMaxVoltage()); 
  // y+=interval; lcd()->setCursor(x, y); lcd()->print("SinceLastCharged: "); displayPrintSecondsAsTime(getTimeSinceLastCharged());

  // y+=interval; lcd()->setCursor(x, y); lcd()->print("BUTTONS "); lcd()->print("TOP:");lcd()->print(isPressed(BUT_UP)); lcd()->print(" CENTER:");lcd()->print(isPressed(BUT_CENTER)); lcd()->print(" BOTTOM:");lcd()->print(isPressed(BUT_DOWN));
  // y+=interval; lcd()->setCursor(x, y); lcd()->print("Since last action: "); lcd()->print(sinceLastAction());

  // y+=interval; lcd()->setCursor(x, y); lcd()->print("Temperature: "); lcd()->print(temperature());
  // y+=interval; lcd()->setCursor(x, y); lcd()->print("rtcChipTemperature: "); lcd()->print(rtcChipTemperature());

  // y+=interval; lcd()->setCursor(x, y); lcd()->print("Preferences remaining memory: "); lcd()->print(getPreferencesFreeSpace());
  // y+=interval; lcd()->setCursor(x, y); lcd()->print("RAM State: "); lcd()->print(esp_get_free_heap_size());  lcd()->print(",  ");  lcd()->print(ESP.getFreeHeap()); //RAM diagnosis

  //y+=interval; lcd()->setCursor(x, y); lcd()->print("MAC: "); printMacAddress();
  //uint32_t getBusClock(void);
  //void setBusClock(uint32_t clock_speed);
  
  draw_ic16_arrow_right(lx(), ly1(), black);
  draw_ic16_back(lx(), ly2(), black);
  draw_ic16_repeat(lx(), ly3(), black);

if(esp_sleep_get_wakeup_cause() != ESP_SLEEP_WAKEUP_TIMER) //if wake by timer, don't refresh display to keep image static, image will refresh when go to lock screen and drawing lock icon
    lcd()->sendBuffer();
}


void ModeBatteryCalibrationButtonUp(){
  //goToSleep();
  //switchDontSleep();
  batteryCalibrationAddValue(readSensBatteryRaw());
}

void ModeBatteryCalibrationButtonCenter(){
  //setModeAppsMenu();
  setModeMenuSettingsDisplay();
  //wifiOff();
}

void ModeBatteryCalibrationButtonDown(){
  resetBatteryCalibration();
}






//----------------------//----------------------//----------------------//----------------------//----------------------//----------------------//----------------------



#endif
