#ifndef MODESetSoundTone_H
#define MODESetSoundTone_H

/*PROTOTYPES*/
void setmodeSetSoundToneMenu();
void modeSetSoundToneMenuLoop();
void modeSetSoundToneMenuButtonUp();
void modeSetSoundToneMenuButtonCenter();
void modeSetSoundToneMenuButtonDown();

#include "../Global.h"
#include "../AutoSleep.h"
#include "../Button.h"
#include "../GlobalMenu.h"
#include "../DrmPreferences.h"
#include "ModeMenuSettingsTime.h"

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
}

void modeSetSoundToneMenuLoop(){
  lcd()->setColorIndex(white);
  lcd()->drawBox(0, 0, 400, 240);

  lcd()->setFont(u8g2_font_10x20_t_cyrillic);  //ok
  lcd()->setColorIndex(black);
  lcd()->setCursor(5, 18); 
  lcd()->print(L("Тон звуків", "All sounds tone"));

  drawStatusbar(363, 1, true);
  drawMenuLegend();
  draw_ic16_back(lx(), ly2(), black);
  draw_ic16_plus(lx(), ly1(), black);
  draw_ic16_minus(lx(), ly3(), black);
  
  float coef = getSoundCoef();

  String text = "Coef: ";
  text += coef;
  drawTextFrame(/*index*/0, /*text*/text.c_str(), /*name*/L("Обрати тон всіх звуків", "Select tone of all sounds"), /*editMode*/false, /*animate*/firstDraw, /*x*/30, /*y*/95, /*width*/308);
  
  lcd()->sendBuffer();
}

void modeSetSoundToneMenuButtonUp(){
  saveSoundCoef(getSoundCoef()*1.1f);
}
void modeSetSoundToneMenuButtonDown(){
  saveSoundCoef(getSoundCoef()*0.9f);
}
void modeSetSoundToneMenuButtonCenter(){
  setModeMenuSettingsSound();
}


#endif