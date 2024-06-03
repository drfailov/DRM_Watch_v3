#ifndef MODEMENUSETTINGTIME_H
#define MODEMENUSETTINGTIME_H

void setModeMenuSettingsTime();
void ModeMenuSettingsTimeLoop();
void ModeMenuSettingsTimeButtonCenter();

#include "Global.h"
#include "AutoSleep.h"
#include "Button.h"
#include "Icons.h"
#include "ModeMainMenu.h"
#include "TimeSync.h"
#include "DrmPreferences.h"
#include "ModeSetTime.h"
#include "ModeSetTimezone.h"


const int ModeMenuSettingsTimeItemBack = 0;
const int ModeMenuSettingsTimeItemSyncTime = 1;
const int ModeMenuSettingsTimeItemAutoSyncEnabled = 2;
const int ModeMenuSettingsTimeItemSetTime = 3;
const int ModeMenuSettingsTimeItemSetTimeZone = 4;

void setModeMenuSettingsTime(){
  clearScreenAnimation();
  Serial.println(F("Set mode: ModeMenuSettingsTime"));
  modeSetup = setModeMenuSettingsTime;
  modeLoop = ModeMenuSettingsTimeLoop;
  modeButtonUp = modeMainMenuButtonUp;
  modeButtonCenter = ModeMenuSettingsTimeButtonCenter;
  modeButtonDown = modeMainMenuButtonDown;
  modeButtonUpLong = 0;
  modeButtonCenterLong = 0;
  modeButtonDownLong = 0;
  registerAction();
  enableAutoReturn = true;
  enableAutoSleep = false; 
  autoReturnTime = autoReturnDefaultTime;
  autoSleepTime = autoSleepDefaultTime;
  selected = 0;
  items = 5;
}


void ModeMenuSettingsTimeLoop(){
  lcd()->setColorIndex(white);
  lcd()->drawBox(0, 0, 400, 240);


  lcd()->setFont(u8g2_font_10x20_t_cyrillic);  //ok
  lcd()->setColorIndex(black);
  lcd()->setCursor(5, 18); 
  lcd()->print("Параметри часу");

  drawMenuLegend();
  drawStatusbar(363, 1, true);

  drawListItem(ModeMenuSettingsTimeItemBack, draw_ic24_back, "Повернутись", "В меню налаштувань", firstDraw); //
  drawListItem(ModeMenuSettingsTimeItemSyncTime, draw_ic24_sync, "Синхронізувати час зараз", "Через збережену Wi-Fi мережу", firstDraw); //
  drawListCheckbox(ModeMenuSettingsTimeItemAutoSyncEnabled, draw_ic24_sync, "Автосинхронізація часу", "Синхронізувати без команди", getTimeSyncEnabled(), firstDraw);
  drawListItem(ModeMenuSettingsTimeItemSetTime, draw_ic24_clock, "Встановити час", "Встановити поточний час вручну", firstDraw);
  drawListItem(ModeMenuSettingsTimeItemSetTimeZone, draw_ic24_timezone, "Встановити часовий пояс", "Встановити зміщення відносно UTC", firstDraw);
  


  lcd()->sendBuffer();
}

void ModeMenuSettingsTimeButtonCenter(){
  if(selected == ModeMenuSettingsTimeItemBack){
      setModeSettingsMenu();
  }
  if(selected == ModeMenuSettingsTimeItemSyncTime){
    timeSync();
    return;
  }
  if(selected == ModeMenuSettingsTimeItemAutoSyncEnabled){
    saveTimeSyncEnabled(!getTimeSyncEnabled());
    return;
  }
  if(selected==ModeMenuSettingsTimeItemSetTime){
    setModeSetTimeMenu();
    return;
  }
  if(selected==ModeMenuSettingsTimeItemSetTimeZone){
    setmodeSetTimezoneMenu();
    return;
  }

  
  
  
  
}


#endif