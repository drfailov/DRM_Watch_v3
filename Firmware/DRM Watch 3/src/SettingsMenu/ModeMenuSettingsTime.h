#ifndef MODEMENUSETTINGTIME_H
#define MODEMENUSETTINGTIME_H

void setModeMenuSettingsTime();
void ModeMenuSettingsTimeLoop();
void ModeMenuSettingsTimeButtonCenter();

#include "../Global.h"
#include "../AutoSleep.h"
#include "../Button.h"
#include "../Icons.h"
#include "../GlobalMenu.h"
#include "../TimeSync.h"
#include "../DrmPreferences.h"
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
  modeButtonUp = globalMenuButtonUp;
  modeButtonCenter = ModeMenuSettingsTimeButtonCenter;
  modeButtonDown = globalMenuButtonDown;
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
  lcd()->print(L("Параметри часу", "Time settings"));

  drawMenuLegend();
  drawStatusbar(363, 1, true);

  drawListItem    (ModeMenuSettingsTimeItemBack,            draw_ic24_back,     L("Повернутись", "Back"),                       L("В меню налаштувань", "To settings menu"),                  firstDraw); //
  drawListItem    (ModeMenuSettingsTimeItemSyncTime,        draw_ic24_sync,     L("Синхронізувати час зараз", "Sync time now"), L("Через збережену Wi-Fi мережу", "Over saved Wi-Fi"),        firstDraw); //
  drawListCheckbox(ModeMenuSettingsTimeItemAutoSyncEnabled, draw_ic24_sync,     L("Автосинхронізація часу", "Sync time daily"), L("Синхронізувати без команди", "Sync automatically"),        getTimeSyncEnabled(), firstDraw);
  drawListItem    (ModeMenuSettingsTimeItemSetTime,         draw_ic24_clock,    L("Встановити час", "Set time"),                L("Встановити поточний час вручну", "Set time manually"),     firstDraw);
  drawListItem    (ModeMenuSettingsTimeItemSetTimeZone,     draw_ic24_timezone, L("Встановити часовий пояс", "Set timezone"),   L("Встановити зміщення відносно UTC", "Set offset from UTC"), firstDraw);
  
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