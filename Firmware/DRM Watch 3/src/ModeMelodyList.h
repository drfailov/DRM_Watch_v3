#ifndef MODEMELODYLIST_H
#define MODEMELODYLIST_H

void setModeMelodyListMenu();
void ModeMelodyListMenuLoop();
void ModeMelodyListMenuButtonCenter();

#include "Global.h"
#include "AutoSleep.h"
#include "Button.h"
#include "GlobalMenu.h"
#include "ModeMenuApps.h"
#include "MelodyPlayer.h"

const int itemModeMelodyListBack=0;

void setModeMelodyListMenu(){
  clearScreenAnimation();
  Serial.println(F("Set mode: Melody List"));
  modeSetup = setModeMelodyListMenu;
  modeLoop = ModeMelodyListMenuLoop;
  modeButtonUp = globalMenuButtonUp;
  modeButtonCenter = ModeMelodyListMenuButtonCenter;
  modeButtonDown = globalMenuButtonDown;
  modeButtonUpLong = 0;
  modeButtonCenterLong = 0;
  modeButtonDownLong = 0;
  registerAction();
  enableAutoReturn = true;
  enableAutoSleep = false; 
  autoReturnTime = autoReturnDefaultTime;
  autoSleepTime = autoSleepDefaultTime;
  //selected = 0;
  items = getMelodyCount()+1;
}


void ModeMelodyListMenuLoop(){
  lcd()->setColorIndex(white);
  lcd()->drawBox(0, 0, 400, 240);

  lcd()->setFont(u8g2_font_10x20_t_cyrillic);  //ok
  lcd()->setColorIndex(black);
  lcd()->setCursor(5, 18); 
  lcd()->print(L("Список Мелодій", "Melodies list"));
  
  drawStatusbar(363, 1, true);
  drawMenuLegend();
  
  drawMenuItem(itemModeMelodyListBack, draw_ic24_back, L("Назад", "Back"), false);
  for(int i=0; i<getMelodyCount(); i++){
    drawMenuItem(i+1, draw_ic24_music, getMelodyName(i).c_str(), firstDraw?(i+2)%4==0:false);
  }

  lcd()->sendBuffer();
}

void ModeMelodyListMenuButtonCenter(){
  if(selected == itemBack){
    setModeAppsMenu();
    return;
  }
  else{
    melodyPlayerSetMelodyName(getMelodyName(selected-1));
    melodyPlayerPlayMelody(getMelodyData(selected-1), false);
    return;
  }
  
}


#endif