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
#include "ModeMenuSettings.h"
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
#include "Notepad.h"

const int itemModeAppsBack=0;
const int itemModeAppsAlarm=1;
const int itemModeAppsCalendar=2;
const int itemModeAppsSettings=3;
const int itemModeAppsMusic=4;
const int itemModeAppsNotepad=5;
const int itemModeAppsTimer=6;
const int itemModeAppsStopwatch=7;
const int itemModeAppsFlashlight=8;
const int itemModeAppsDebug=9;
const int itemModeAppsSerialLog=10;
const int itemModeAppsMeow=11;
const int itemModeAppsBackgrounds=12;


void setModeAppsMenu(){
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
  items = 13;
}


void ModeAppsMenuLoop(){
  lcd()->setColorIndex(white);
  lcd()->drawBox(0, 0, 400, 240);

  lcd()->setFont(u8g2_font_10x20_t_cyrillic);  //ok
  lcd()->setColorIndex(black);
  lcd()->setCursor(5, 18); 
  lcd()->print("Програми");
  
  drawStatusbar(363, 1, true);
  drawMenuLegend();
  
  drawMenuItem(itemModeAppsBack,       draw_ic24_back,       "Вийти",                         false);
  drawMenuItem(itemModeAppsAlarm,      draw_ic24_alarm,      "Будильник",                     false);
  drawMenuItem(itemModeAppsCalendar,   draw_ic24_calendar,   "Календар",                      false);
  drawMenuItem(itemModeAppsSettings,   draw_ic24_settings,   "Налаштування",                  firstDraw);
  drawMenuItem(itemModeAppsMusic,      draw_ic24_music,      "Мелодії",                       false);
  drawMenuItem(itemModeAppsNotepad,    draw_ic24_apps,       "Нотатки",                       false);
  drawMenuItem(itemModeAppsTimer,      draw_ic24_timer,      "Таймер",                        false);
  drawMenuItem(itemModeAppsStopwatch,  draw_ic24_stopwatch,  "Секундомір",                    firstDraw);
  drawMenuItem(itemModeAppsFlashlight, draw_ic24_flashlight2,"Ліхтарик",                      false);
  drawMenuItem(itemModeAppsDebug,      draw_ic24_bug,        "Інженерне меню",                false);
  drawMenuItem(itemModeAppsSerialLog,  draw_ic24_terminal,   "Лог UART",                      false);
  drawMenuItem(itemModeAppsMeow,       draw_ic24_meow,       "Meow",                          firstDraw);
  drawMenuItem(itemModeAppsBackgrounds,draw_ic24_watchface,  "Заставки",                      false);
  
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
  if(selected == itemModeAppsNotepad){
    setmodeNotepad();
    return;
  }
  
  //
}


#endif