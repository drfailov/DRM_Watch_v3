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
  modeButtonUp = globalMenuButtonUp;
  modeButtonCenter = ModeBackgroundsMenuButtonCenter;
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
  items = 7;
}


void ModeBackgroundsMenuLoop(){
  lcd()->setColorIndex(white);
  lcd()->drawBox(0, 0, 400, 240);

  lcd()->setFont(u8g2_font_10x20_t_cyrillic);  //ok
  lcd()->setColorIndex(black);
  lcd()->setCursor(5, 18); 
  lcd()->print(L("Заставки", "Backgrounds"));
  
  drawStatusbar(363, 1, true);
  drawMenuLegend();
  
  drawMenuItem(itemModeBackgroundsBack,       draw_ic24_back,       L("Вийти", "Back"),                                        false);
  drawMenuItem(itemModeBackgroundsLife,       draw_ic24_life,       L("Клітковий автомат \"Життя\"", "Convey's Game Of Life"), false);
  drawMenuItem(itemModeBackgroundsFire,       draw_ic24_fire,       L("Вогонь", "Fire"),                                       false);
  drawMenuItem(itemModeBackgroundsLava,       draw_ic24_bubbles,    L("Лава Лампа", "Lava lamp"),                              firstDraw);
  drawMenuItem(itemModeBackgroundsRandom,     draw_ic24_random,     L("Випадковий шум", "Noize"),                              false);
  drawMenuItem(itemModeBackgroundsAnt,        draw_ic24_ant2,       L("Mypaxa Ленгтона", "Lengton`s ant"),                     false);
  drawMenuItem(itemModeBackgroundsDots,       draw_ic24_pixels,     L("Точки", "Dots"),                                        false);
  
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