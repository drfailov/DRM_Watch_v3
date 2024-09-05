#ifndef ModeTestBatteryAnalysis_H
#define ModeTestBatteryAnalysis_H

/*PROTOTYPES*/
void setModeTestBatteryAnalysis();
void ModeTestBatteryAnalysisLoop();
void ModeTestBatteryAnalysisButtonUp();
void ModeTestBatteryAnalysisButtonCenter();
void ModeTestBatteryAnalysisButtonDown();
void wakeup_reason();


#include <Arduino.h>
#include "Global.h"
#include "AutoSleep.h"
#include "Button.h"
#include "Battery.h"
#include "DrmPreferences.h"
#include "ModeTest.h"



void setModeTestBatteryAnalysis(){
  if(modeExit != 0)
    modeExit();
  clearScreenAnimation();
  Serial.println(F("Set mode: TestBatteryAnalysis"));
  modeSetup = setModeTestBatteryAnalysis;
  modeLoop = ModeTestBatteryAnalysisLoop;
  modeButtonUp = ModeTestBatteryAnalysisButtonUp;
  modeButtonCenter = ModeTestBatteryAnalysisButtonCenter;
  modeButtonDown = ModeTestBatteryAnalysisButtonDown;
  modeButtonUpLong = 0;
  modeButtonCenterLong = 0;
  modeButtonDownLong = 0;
  registerAction();
  enableAutoReturn = false;
  enableAutoSleep = false; 
  autoReturnTime = autoReturnDefaultTime;
  autoSleepTime = autoSleepDefaultTime;
}

RTC_DATA_ATTR float averageRaw = -1;
RTC_DATA_ATTR float averageDeviation = -1;


#define ModeTestBatteryAnalysisHistorySize 100
int16_t ModeTestBatteryAnalysisHistoryRaw[ModeTestBatteryAnalysisHistorySize]; 
int16_t ModeTestBatteryAnalysisHistoryAverageRaw[ModeTestBatteryAnalysisHistorySize]; 
int16_t ModeTestBatteryAnalysisHistoryDeviation[ModeTestBatteryAnalysisHistorySize]; 
int16_t ModeTestBatteryAnalysisHistoryAverageDeviation[ModeTestBatteryAnalysisHistorySize]; 
int16_t ModeTestBatteryAnalysisHistoryLimitDeviation[ModeTestBatteryAnalysisHistorySize]; 
int16_t ModeTestBatteryAnalysisHistoryFiltered[ModeTestBatteryAnalysisHistorySize]; 




void ModeTestBatteryAnalysisLoop(){ 

  float raw = readSensBatteryRaw();
  if(averageRaw == -1)
    averageRaw = raw;
  float volt = readSensBatteryVoltage();
  float deviation = abs(raw-averageRaw);
  if(averageDeviation == -1)
    averageDeviation = deviation;
  float limitDeviation = averageDeviation*2;
  float filtered = raw;
  if(deviation > limitDeviation)
    filtered = averageRaw;

  averageRaw += (raw-averageRaw)*0.1;
  averageDeviation += (deviation-averageDeviation)*0.1;
  
  for(int i=1; i<ModeTestBatteryAnalysisHistorySize; i++){
    ModeTestBatteryAnalysisHistoryRaw[i-1]=ModeTestBatteryAnalysisHistoryRaw[i];
    ModeTestBatteryAnalysisHistoryAverageRaw[i-1]=ModeTestBatteryAnalysisHistoryAverageRaw[i];
    ModeTestBatteryAnalysisHistoryDeviation[i-1]=ModeTestBatteryAnalysisHistoryDeviation[i];
    ModeTestBatteryAnalysisHistoryAverageDeviation[i-1]=ModeTestBatteryAnalysisHistoryAverageDeviation[i];
    ModeTestBatteryAnalysisHistoryLimitDeviation[i-1]=ModeTestBatteryAnalysisHistoryLimitDeviation[i];
    ModeTestBatteryAnalysisHistoryFiltered[i-1]=ModeTestBatteryAnalysisHistoryFiltered[i];
  }
  ModeTestBatteryAnalysisHistoryRaw[ModeTestBatteryAnalysisHistorySize-1] = raw;
  ModeTestBatteryAnalysisHistoryAverageRaw[ModeTestBatteryAnalysisHistorySize-1] = averageRaw;
  ModeTestBatteryAnalysisHistoryDeviation[ModeTestBatteryAnalysisHistorySize-1] = deviation;
  ModeTestBatteryAnalysisHistoryAverageDeviation[ModeTestBatteryAnalysisHistorySize-1] = averageDeviation;
  ModeTestBatteryAnalysisHistoryLimitDeviation[ModeTestBatteryAnalysisHistorySize-1] = limitDeviation;
  ModeTestBatteryAnalysisHistoryFiltered[ModeTestBatteryAnalysisHistorySize-1] = filtered;




  lcd()->setColorIndex(white);
  lcd()->drawBox(0, 0, 400, 240);
  
  lcd()->setColorIndex(black);


  lcd()->setFont(u8g2_font_10x20_t_cyrillic); // ok
  lcd()->drawUTF8(10, 17, "Battery debug");

  

  lcd()->setCursor(150, 17);
  lcd()->setFont(u8g2_font_unifont_t_cyrillic); //smalll
  lcd()->print("RAW:");
  lcd()->print((int)raw);
  lcd()->print(", ");
  lcd()->print(((float)volt)/1000.0);
  lcd()->print("v");

  lcd()->setCursor(300, 17);
  lcd()->setFont(u8g2_font_unifont_t_cyrillic); //smalll
  lcd()->print("Avg:");
  lcd()->print(averageRaw);


  drawPlot      (/*x*/10, /*y*/40, /*w*/360, /*h*/90, ModeTestBatteryAnalysisHistoryDeviation, /*length*/ModeTestBatteryAnalysisHistorySize, /*highlight*/0);
  drawSecondPlot(/*x*/10, /*y*/40, /*w*/360, /*h*/90, ModeTestBatteryAnalysisHistoryDeviation, ModeTestBatteryAnalysisHistoryLimitDeviation, /*length*/ModeTestBatteryAnalysisHistorySize);

  drawPlot      (/*x*/10, /*y*/140, /*w*/360, /*h*/90, ModeTestBatteryAnalysisHistoryFiltered, /*length*/ModeTestBatteryAnalysisHistorySize, /*highlight*/0);
  drawSecondPlot(/*x*/10, /*y*/140, /*w*/360, /*h*/90, ModeTestBatteryAnalysisHistoryFiltered, ModeTestBatteryAnalysisHistoryAverageRaw, /*length*/ModeTestBatteryAnalysisHistorySize);
  drawSecondPlot(/*x*/10, /*y*/140, /*w*/360, /*h*/90, ModeTestBatteryAnalysisHistoryFiltered, ModeTestBatteryAnalysisHistoryRaw, /*length*/ModeTestBatteryAnalysisHistorySize);
  
  draw_ic16_arrow_up(lx(), ly1(), black);
  draw_ic16_back(lx(), ly2(), black);
  //draw_ic16_hashtag(lx(), ly3(), black);

if(esp_sleep_get_wakeup_cause() != ESP_SLEEP_WAKEUP_TIMER) //if wake by timer, don't refresh display to keep image static, image will refresh when go to lock screen and drawing lock icon
    lcd()->sendBuffer();
}


void ModeTestBatteryAnalysisButtonUp(){
  //goToSleep();
  //switchDontSleep();
  setModeTest();
}

void ModeTestBatteryAnalysisButtonCenter(){
  setModeAppsMenu();
  //wifiOff();
}

void ModeTestBatteryAnalysisButtonDown(){
  
}






//----------------------//----------------------//----------------------//----------------------//----------------------//----------------------//----------------------

void wakeup_reason() {
  /*
  0 = first start
  ESP_SLEEP_WAKEUP_TIMER = Wakeup periodically 

  */
  esp_sleep_wakeup_cause_t wakeup_reason = esp_sleep_get_wakeup_cause();
  switch (wakeup_reason)
  {
    case ESP_SLEEP_WAKEUP_EXT0 : lcd()->print("Wakeup external signal RTC_IO"); break;
    case ESP_SLEEP_WAKEUP_EXT1 : lcd()->print("Wakeup external signal RTC_CNTL"); break;
    case ESP_SLEEP_WAKEUP_TIMER : lcd()->print("Wakeup caused by timer"); break;
    case ESP_SLEEP_WAKEUP_TOUCHPAD : lcd()->print("Wakeup caused by touchpad"); break;
    case ESP_SLEEP_WAKEUP_ULP : lcd()->print("Wakeup caused by ULP program"); break;
    default :
      lcd()->print("not deep sleep:");
      lcd()->print(wakeup_reason);
      //rtc.setTime(30, 24, 15, 17, 1, 2021);  // 17th Jan 2021 15:24:30
      //rtc.setTime(1609459200);  // 1st Jan 2021 00:00:00
      //rtc.offset = 7200; // change offset value

      break;
  }
}


#endif
