#ifndef MODETEST_H
#define MODETEST_H

/*PROTOTYPES*/
void setModeTest();
void modeTestLoop();
void modeTestButtonUp();
void modeTestButtonCenter();
void modeTestButtonDown();
void wakeup_reason();


#include <Arduino.h>
#include "Global.h"
#include "AutoSleep.h"
#include "Button.h"
#include "Battery.h"
#include "DrmPreferences.h"
#include "ModeTestBatteryAnalysis.h"


#include <WiFi.h>
#include <esp_wifi.h>
void printMacAddress(){
  uint8_t baseMac[6];
  esp_err_t ret = esp_wifi_get_mac(WIFI_IF_STA, baseMac);
  if (ret == ESP_OK) {
    sprintf(buffer, "%02x:%02x:%02x:%02x:%02x:%02x\n",
                  baseMac[0], baseMac[1], baseMac[2],
                  baseMac[3], baseMac[4], baseMac[5]);
    lcd()->print(buffer);
  } else {
    lcd()->print("Failed to read MAC address");
  }
}

void setModeTest(){
  if(modeExit != 0)
    modeExit();
  clearScreenAnimation();
  Serial.println(F("Set mode: Test"));
  modeSetup = setModeTest;
  modeLoop = modeTestLoop;
  modeButtonUp = modeTestButtonUp;
  modeButtonCenter = modeTestButtonCenter;
  modeButtonDown = modeTestButtonDown;
  modeButtonUpLong = 0;
  modeButtonCenterLong = 0;
  modeButtonDownLong = 0;
  registerAction();
  enableAutoReturn = false;
  enableAutoSleep = true; 
  autoReturnTime = autoReturnDefaultTime;
  autoSleepTime = autoSleepDefaultTime;
  WiFi.mode(WIFI_STA);
}

void modeTestLoop(){ 
  lcd()->setColorIndex(white);
  lcd()->drawBox(0, 0, 400, 240);
  
  lcd()->setColorIndex(black);
  lcd()->setFont(u8g2_font_unifont_t_cyrillic); //smalll

  int y=2;
  int x=5;
  int interval = 13;

  y+=interval; lcd()->setCursor(x, y); lcd()->print("Internal RTC: "); lcd()->print(_rtcInternal()->getTime("%d %b %Y %H:%M:%S"));  lcd()->print(" ("); lcd()->print(_rtcInternal()->getEpoch());   lcd()->print(")");
  y+=interval; lcd()->setCursor(x, y); lcd()->print("External RTC: "); printRtcGetTimeRaw();   
  y+=interval; lcd()->setCursor(x, y); lcd()->print("SinceLastSync :"); displayPrintSecondsAsTime(rtcGetUtcEpoch()-getLastTimeSync());  lcd()->print(" Last sync:"); lcd()->print(getLastTimeSync());   
  y+=interval; lcd()->setCursor(x, y); lcd()->print("RTC CLK SRC: "); lcd()->print(getRtcSrc());

  y+=interval; lcd()->setCursor(x, y); lcd()->print("Wakeup_reason: "); wakeup_reason();
  y+=interval; lcd()->setCursor(x, y); lcd()->print("Millis:"); lcd()->print(millis()); lcd()->print(",   Don't sleep: "); lcd()->print(dontSleep);

  y+=interval; lcd()->setCursor(x, y); lcd()->print("RAW ");  lcd()->print("BATTERY: "); lcd()->print(readSensBatteryRawFiltered()); lcd()->print(", USB:"); lcd()->print(readSensUsbRaw()); lcd()->print(" : "); lcd()->print(isChargerConnected());
  y+=interval; lcd()->setCursor(x, y); lcd()->print("VOLTAGE ");  lcd()->print("BATTERY: "); lcd()->print(readSensBatteryVoltage()); lcd()->print(" ("); lcd()->print(batteryBars()); lcd()->print(" bars)"); 
  y+=interval; lcd()->setCursor(x, y); lcd()->print("MIN voltage: ");  lcd()->print(getBatteryMinVoltage());  lcd()->print(", MAX voltage: ");  lcd()->print(getBatteryMaxVoltage()); 
  y+=interval; lcd()->setCursor(x, y); lcd()->print("SinceLastCharged: "); displayPrintSecondsAsTime(getTimeSinceLastCharged());

  y+=interval; lcd()->setCursor(x, y); lcd()->print("BUTTONS "); lcd()->print("TOP:");lcd()->print(isPressed(BUT_UP)); lcd()->print(" CENTER:");lcd()->print(isPressed(BUT_CENTER)); lcd()->print(" BOTTOM:");lcd()->print(isPressed(BUT_DOWN));
  y+=interval; lcd()->setCursor(x, y); lcd()->print("Since last action: "); lcd()->print(sinceLastAction());

  y+=interval; lcd()->setCursor(x, y); lcd()->print("Temperature: "); lcd()->print(temperature());
  y+=interval; lcd()->setCursor(x, y); lcd()->print("rtcChipTemperature: "); lcd()->print(rtcChipTemperature());

  y+=interval; lcd()->setCursor(x, y); lcd()->print("Preferences remaining memory: "); lcd()->print(getPreferencesFreeSpace());
  y+=interval; lcd()->setCursor(x, y); lcd()->print("RAM State: "); lcd()->print(esp_get_free_heap_size());  lcd()->print(",  ");  lcd()->print(ESP.getFreeHeap()); //RAM diagnosis

  y+=interval; lcd()->setCursor(x, y); lcd()->print("MAC: "); printMacAddress();
  //uint32_t getBusClock(void);
  //void setBusClock(uint32_t clock_speed);
  
  draw_ic16_coffee(lx(), ly1(), black);
  draw_ic16_back(lx(), ly2(), black);
  draw_ic16_arrow_down(lx(), ly3(), black);

if(esp_sleep_get_wakeup_cause() != ESP_SLEEP_WAKEUP_TIMER) //if wake by timer, don't refresh display to keep image static, image will refresh when go to lock screen and drawing lock icon
    lcd()->sendBuffer();
}


void modeTestButtonUp(){
  //goToSleep();
  switchDontSleep();
}

void modeTestButtonCenter(){
  setModeAppsMenu();
  wifiOff();
}

void modeTestButtonDown(){
  setModeTestBatteryAnalysis();
  wifiOff();
}






//----------------------//----------------------//----------------------//----------------------//----------------------//----------------------//----------------------



#endif
