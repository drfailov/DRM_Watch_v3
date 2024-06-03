#ifndef MODESETTIMEZONE_H
#define MODESETTIMEZONE_H

/*PROTOTYPES*/
void setmodeSetTimezoneMenu();
void modeSetTimezoneMenuLoop();
void modeSetTimezoneMenuButtonUp();
void modeSetTimezoneMenuButtonCenter();
void modeSetTimezoneMenuButtonDown();

#include "Global.h"
#include "AutoSleep.h"
#include "Button.h"
#include "ModeMainMenu.h"
#include "DrmPreferences.h"
#include "ModeMenuSettingsTime.h"

//int modeSetTimezoneIndex = 0;
//bool modeSetTimezoneEditMode = false;

//const int itemmodeSetTimezoneBack=0;
//const int itemmodeSetTimezoneHour=1;

void setmodeSetTimezoneMenu(){
  clearScreenAnimation();
  Serial.println(F("Set mode: set timezone"));
  modeSetup = setmodeSetTimezoneMenu;
  modeLoop = modeSetTimezoneMenuLoop;
  modeButtonUp = modeSetTimezoneMenuButtonUp;
  modeButtonCenter = modeSetTimezoneMenuButtonCenter;
  modeButtonDown = modeSetTimezoneMenuButtonDown;
  modeButtonUpLong = 0;
  modeButtonCenterLong = 0;
  modeButtonDownLong = 0;
  registerAction();
  enableAutoReturn = true;
  enableAutoSleep = false; 
  autoReturnTime = autoReturnDefaultTime;
  autoSleepTime = autoSleepDefaultTime;
  selected = 0;
  items = 2;
}


void modeSetTimezoneMenuLoop(){
  lcd()->setColorIndex(white);
  lcd()->drawBox(0, 0, 400, 240);

  lcd()->setFont(u8g2_font_10x20_t_cyrillic);  //ok
  lcd()->setColorIndex(black);
  lcd()->setCursor(5, 18); 
  lcd()->print("Часовий пояс");

  drawStatusbar(363, 1, true);
  drawMenuLegend();
  draw_ic16_back(lx(), ly2(), black);
  // if(modeSetTimezoneEditMode){
     draw_ic16_plus(lx(), ly1(), black);
     draw_ic16_minus(lx(), ly3(), black);
  // }
  
  //drawMenuItem(itemmodeSetTimezoneBack, draw_ic24_back, "Назад", firstDraw, 30, 32);

  //lcd()->setFont(u8g2_font_10x20_t_cyrillic);  //ok
  //lcd()->setColorIndex(black);
  //lcd()->setCursor(30, 113); 
  //lcd()->print("Час");
  
  long offset = getTimeOffsetSec();
  long hours = offset/(60*60);
  offset -= hours*(60*60);
  long minutes = offset/60;
  String text = "UTC ";
  text += hours>=0?"+":"";
  text += hours;
  text += ":";
  text += minutes;
  drawTextFrame(/*index*/0, /*text*/text.c_str(), /*name*/"Обрати часовий пояс", /*editMode*/false, /*animate*/firstDraw, /*x*/30, /*y*/95, /*width*/308);
  
  lcd()->sendBuffer();
}

void modeSetTimezoneMenuButtonUp(){
  // if(!modeSetTimezoneEditMode){
  //   modeMainMenuButtonUp();
  //   return;
  // }
  // if(selected == itemmodeSetTimezoneHour) 
  saveTimeOffsetSec(getTimeOffsetSec()+60*30);
}
void modeSetTimezoneMenuButtonDown(){
  // if(!modeSetTimezoneEditMode){
  //   modeMainMenuButtonDown();
  //   return;
  // }
  // if(selected == itemmodeSetTimezoneHour) 
  saveTimeOffsetSec(getTimeOffsetSec()-60*30);
}
void modeSetTimezoneMenuButtonCenter(){
  //if(selected == itemmodeSetTimezoneBack){
    setModeMenuSettingsTime();
//    return;
  //}
  // if(selected==itemmodeSetTimezoneHour){
  //   modeSetTimezoneEditMode = !modeSetTimezoneEditMode;
  //   return;
  // }
}


#endif