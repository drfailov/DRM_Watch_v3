#ifndef MODEBackgroundsMenu_H
#define MODEBackgroundsMenu_H

/*PROTOTYPES*/
void setModeBackgroundsMenu();
void ModeBackgroundsMenuLoop();
void ModeBackgroundsMenuButtonCenter();

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

const int itemModeBackgroundsBack=0;
const int itemModeBackgroundsLife=1;
const int itemModeBackgroundsFire=2;
const int itemModeBackgroundsLava=3;
const int itemModeBackgroundsRandom=4;
const int itemModeBackgroundsAnt=5;
const int itemModeBackgroundsDots=6;


void setModeBackgroundsMenu(){
  clearScreenAnimation();
  Serial.println(F("Set mode: Backgrounds Menu"));
  modeSetup = setModeBackgroundsMenu;
  modeLoop = ModeBackgroundsMenuLoop;
  modeButtonUp = modeMainMenuButtonUp;
  modeButtonCenter = ModeBackgroundsMenuButtonCenter;
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
  items = 7;
}


void ModeBackgroundsMenuLoop(){
  lcd()->setColorIndex(white);
  lcd()->drawBox(0, 0, 400, 240);

  lcd()->setFont(u8g2_font_10x20_t_cyrillic);  //ok
  lcd()->setColorIndex(black);
  lcd()->setCursor(5, 18); 
  lcd()->print("Заставки");
  
  drawStatusbar(363, 1, true);
  drawMenuLegend();
  
  drawMenuItem(itemModeBackgroundsBack,       draw_ic24_back,       "Вийти",                         false);
  drawMenuItem(itemModeBackgroundsLife,       draw_ic24_life,       "Клітковий автомат \"Життя\"",   firstDraw);
  drawMenuItem(itemModeBackgroundsFire,       draw_ic24_fire,       "Вогонь",                        false);
  drawMenuItem(itemModeBackgroundsLava,       draw_ic24_bubbles,    "Лава Лампа",                    false);
  drawMenuItem(itemModeBackgroundsRandom,     draw_ic24_random,     "Випадковий шум",                false);
  drawMenuItem(itemModeBackgroundsAnt,        draw_ic24_ant2,       "Mypaxa Ленгтона",               firstDraw);
  drawMenuItem(itemModeBackgroundsDots,       draw_ic24_pixels,     "Точки",                         false);
  
  lcd()->sendBuffer();
}

void ModeBackgroundsMenuButtonCenter(){
  if(selected == itemModeBackgroundsBack){
    setModeAppsMenu();
    return;
  }
  if(selected == itemModeBackgroundsLife){
    setModeLife();
    return;
  }
  if(selected == itemModeBackgroundsAnt){
    setModeAnt();
    return;
  }
  if(selected == itemModeBackgroundsFire){
    setModeFire();
    return;
  }
  if(selected == itemModeBackgroundsLava){
    setModeLava();
    return;
  }
  if(selected == itemModeBackgroundsRandom){
    setModeRandom();
    return;
  }
  if(selected == itemModeBackgroundsDots){
    setModeDots();
    return;
  }
}


#endif