#ifndef MODESetSoundTone_H
#define MODESetSoundTone_H

/*PROTOTYPES*/
void setmodeSetSoundToneMenu();
void modeSetSoundToneMenuLoop();
void modeSetSoundToneMenuButtonUp();
void modeSetSoundToneMenuButtonCenter();
void modeSetSoundToneMenuButtonDown();

#include "Global.h"
#include "AutoSleep.h"
#include "Button.h"
#include "ModeMainMenu.h"
#include "DrmPreferences.h"
#include "ModeMenuSettingsTime.h"

//int modeSetSoundToneIndex = 0;
//bool modeSetSoundToneEditMode = false;

//const int itemmodeSetSoundToneBack=0;
//const int itemmodeSetSoundToneHour=1;

void setmodeSetSoundToneMenu(){
  clearScreenAnimation();
  Serial.println(F("Set mode: set SoundTone"));
  modeSetup = setmodeSetSoundToneMenu;
  modeLoop = modeSetSoundToneMenuLoop;
  modeButtonUp = modeSetSoundToneMenuButtonUp;
  modeButtonCenter = modeSetSoundToneMenuButtonCenter;
  modeButtonDown = modeSetSoundToneMenuButtonDown;
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


void modeSetSoundToneMenuLoop(){
  lcd()->setColorIndex(white);
  lcd()->drawBox(0, 0, 400, 240);

  lcd()->setFont(u8g2_font_10x20_t_cyrillic);  //ok
  lcd()->setColorIndex(black);
  lcd()->setCursor(5, 18); 
  lcd()->print("Тон звуків");

  drawStatusbar(363, 1, true);
  drawMenuLegend();
  draw_ic16_back(lx(), ly2(), black);
  // if(modeSetSoundToneEditMode){
     draw_ic16_plus(lx(), ly1(), black);
     draw_ic16_minus(lx(), ly3(), black);
  // }
  
  //drawMenuItem(itemmodeSetSoundToneBack, draw_ic24_back, "Назад", firstDraw, 30, 32);

  //lcd()->setFont(u8g2_font_10x20_t_cyrillic);  //ok
  //lcd()->setColorIndex(black);
  //lcd()->setCursor(30, 113); 
  //lcd()->print("Час");
  
  float coef = getSoundCoef();

  String text = "Coef: ";
  text += coef;
  drawTextFrame(/*index*/0, /*text*/text.c_str(), /*name*/"Обрати тон всіх звуків", /*editMode*/false, /*animate*/firstDraw, /*x*/30, /*y*/95, /*width*/308);
  
  lcd()->sendBuffer();
}

void modeSetSoundToneMenuButtonUp(){
  // if(!modeSetSoundToneEditMode){
  //   modeMainMenuButtonUp();
  //   return;
  // }
  // if(selected == itemmodeSetSoundToneHour) 
  //saveTimeOffsetSec(getTimeOffsetSec()+60*30);
  saveSoundCoef(getSoundCoef()*1.1f);
}
void modeSetSoundToneMenuButtonDown(){
  saveSoundCoef(getSoundCoef()*0.9f);
  // if(!modeSetSoundToneEditMode){
  //   modeMainMenuButtonDown();
  //   return;
  // }
  // if(selected == itemmodeSetSoundToneHour) 
  //saveTimeOffsetSec(getTimeOffsetSec()-60*30);
}
void modeSetSoundToneMenuButtonCenter(){
  //if(selected == itemmodeSetSoundToneBack){
    setModeMenuSettingsSound();
//    return;
  //}
  // if(selected==itemmodeSetSoundToneHour){
  //   modeSetSoundToneEditMode = !modeSetSoundToneEditMode;
  //   return;
  // }
}


#endif