#ifndef MODEMENUAPPS_H
#define MODEMENUAPPS_H

/*PROTOTYPES*/
void setModeAppsMenu();
void ModeAppsMenuLoop();
void ModeAppsMenuButtonCenter();

#include "Global.h"
#include "AutoSleep.h"
#include "Button.h"
#include "Icons.h"
#include "Lcd.h"
#include "ModeFire.h"
#include "SettingsMenu/ModeMenuSettings.h"
#include "ModeStopwatch.h"
#include "ModeTimer.h"
#include "ModeLife.h"
#include "ModeRandomNoize.h"
#include "ModeAlertsList.h"
#include "ModeMelodyList.h"
#include "ModeMeow.h"
#include "ModeCalendar.h"
#include "ModeAnt.h"
#include "ModeTest.h"
#include "ModeDots.h"
#include "ModeSerialLog.h"
#include "ModeMenuBackgrounds.h"
#include "Files/ModeFileManager.h"
#include "Files/ModeMemoryManager.h"

const int itemModeAppsBack=0;
const int itemModeAppsAlarm=1;
const int itemModeAppsCalendar=2;
const int itemModeAppsSettings=3;
const int itemModeAppsMusic=4;
const int itemModeAppsFileManager=5;
const int itemModeAppsMemoryManager=6;
const int itemModeAppsTimer=7;
const int itemModeAppsStopwatch=8;
const int itemModeAppsFlashlight=9;
const int itemModeAppsDebug=10;
const int itemModeAppsSerialLog=11;
const int itemModeAppsMeow=12;
const int itemModeAppsBackgrounds=13;


void setModeAppsMenu(){
  if(modeExit != 0)
    modeExit();
  clearScreenAnimation();
  Serial.println(F("Set mode: Apps Menu"));
  modeSetup = setModeAppsMenu;
  modeLoop = ModeAppsMenuLoop;
  modeButtonUp = modeMainMenuButtonUp;
  modeButtonCenter = ModeAppsMenuButtonCenter;
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
  items = 14;
}


void ModeAppsMenuLoop(){
  lcd()->setColorIndex(white);
  lcd()->drawBox(0, 0, 400, 240);

  lcd()->setFont(u8g2_font_10x20_t_cyrillic);  //ok
  lcd()->setColorIndex(black);
  lcd()->setCursor(5, 18); 
  lcd()->print(L("Програми", "Apps"));
  
  drawStatusbar(363, 1, true);
  drawMenuLegend();
  
  drawMenuItem(itemModeAppsBack,         draw_ic24_back,       L("Вийти", "Exit"),                         false);
  drawMenuItem(itemModeAppsAlarm,        draw_ic24_alarm,      L("Будильник", "Alert"),                     false);
  drawMenuItem(itemModeAppsCalendar,     draw_ic24_calendar,   L("Календар", "Calendar"),                      false);
  drawMenuItem(itemModeAppsSettings,     draw_ic24_settings,   L("Налаштування", "Settings"),                  firstDraw);
  drawMenuItem(itemModeAppsMusic,        draw_ic24_music,      L("Мелодії", "Melodies"),                       false);
  drawMenuItem(itemModeAppsFileManager,  draw_ic24_folder,     L("Файли", "Files"),                         false);
  drawMenuItem(itemModeAppsMemoryManager,draw_ic24_memory,     L("Пам'ять", "Memory"),                       false);
  drawMenuItem(itemModeAppsTimer,        draw_ic24_timer,      L("Таймер", "Timer"),                        firstDraw);
  drawMenuItem(itemModeAppsStopwatch,    draw_ic24_stopwatch,  L("Секундомір", "Stopwatch"),                    false);
  drawMenuItem(itemModeAppsFlashlight,   draw_ic24_flashlight2,L("Ліхтарик", "Flashlight"),                      false);
  drawMenuItem(itemModeAppsDebug,        draw_ic24_bug,        L("Інженерне меню", "Debug menu"),                false);
  drawMenuItem(itemModeAppsSerialLog,    draw_ic24_terminal,   L("Лог UART", "UART Log"),                      firstDraw);
  drawMenuItem(itemModeAppsMeow,         draw_ic24_meow,       L("Мявчання кошенят", "Meow"),                          false);
  drawMenuItem(itemModeAppsBackgrounds,  draw_ic24_watchface,  L("Заставки", "Screensavers"),                      false);
  
  lcd()->sendBuffer();
}

void ModeAppsMenuButtonCenter(){
  if(selected == itemModeAppsBack){
    setModeWatchface();
    return;
  }
  if(selected == itemModeAppsTimer){
    setModeTimer();
    return;
  }
  if(selected == itemModeAppsAlarm){
    setModeAlertsList();
    return;
  }
  if(selected==itemModeAppsMusic){
    setModeMelodyListMenu();
    selected = 0;
    return;
  }
  if(selected==itemModeAppsStopwatch){
    setModeStopwatch();
    return;
  }
  if(selected==itemModeAppsFlashlight){
    ledFlashlightToggle();
    return;
  }
  if(selected==itemModeAppsMeow){
    setModeMeow();
    return;
  }
  if(selected == itemModeAppsCalendar){
    setModeCalendar();
    return;
  }
  if(selected == itemModeAppsDebug){
    setModeTest();
    return;
  }
  if(selected == itemModeAppsSerialLog){
    setmodeSerialLog();
    return;
  }
  if(selected == itemModeAppsSettings){
    setModeSettingsMenu();
    return;
  }
  if(selected == itemModeAppsBackgrounds){
    setModeBackgroundsMenu();
    return;
  }
  if(selected == itemModeAppsFileManager){
    setmodeFileManager();
    selected = 0;
    return;
  }
  if(selected == itemModeAppsMemoryManager){
    setmodeMemoryManager();
    return;
  }
  
  //
}


#endif