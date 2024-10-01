#ifndef MODESetRoundness_H
#define MODESetRoundness_H

/*PROTOTYPES*/
void setmodeSetRoundness();
void modeSetRoundnessLoop();
void modeSetRoundnessButtonUp();
void modeSetRoundnessButtonCenter();
void modeSetRoundnessButtonDown();

#include "../Global.h"
#include "../AutoSleep.h"
#include "../Button.h"
#include "../GlobalMenu.h"
#include "../DrmPreferences.h"
#include "ModeMenuSettingsTime.h"

void setmodeSetRoundness(){
  clearScreenAnimation();
  Serial.println(F("Set mode: set Roundness"));
  modeSetup = setmodeSetRoundness;
  modeLoop = modeSetRoundnessLoop;
  modeButtonUp = modeSetRoundnessButtonUp;
  modeButtonCenter = modeSetRoundnessButtonCenter;
  modeButtonDown = modeSetRoundnessButtonDown;
  modeButtonUpLong = 0;
  modeButtonCenterLong = 0;
  modeButtonDownLong = 0;
  registerAction();
  enableAutoReturn = true;
  enableAutoSleep = false; 
  autoReturnTime = autoReturnDefaultTime;
  autoSleepTime = autoSleepDefaultTime;
}

void modeSetRoundnessLoop(){
  lcd()->setColorIndex(white);
  lcd()->drawBox(0, 0, 400, 240);

  lcd()->setFont(u8g2_font_10x20_t_cyrillic);  //ok
  lcd()->setColorIndex(black);
  lcd()->setCursor(5, 18); 
  lcd()->print(L("Скруглення всіх меню", "All menu roundness"));

  drawStatusbar(363, 1, true);
  drawMenuLegend();
  draw_ic16_back(lx(), ly2(), black);
  draw_ic16_plus(lx(), ly1(), black);
  draw_ic16_minus(lx(), ly3(), black);
  
  float coef = getRoundness();

  String text = L("Скруглення: ", "Roundness: ");
  text += coef;
  drawTextFrame(/*index*/0, /*text*/text.c_str(), /*name*/L("Обрати скруглення", "Select roundness"), /*editMode*/false, /*animate*/firstDraw, /*x*/30, /*y*/95, /*width*/308);
  
  lcd()->sendBuffer();
}

void modeSetRoundnessButtonUp(){
  if(getRoundness() < 18)
    saveRoundness(getRoundness()+1);
}
void modeSetRoundnessButtonDown(){
  if(getRoundness() > 0)
    saveRoundness(getRoundness()-1);
}
void modeSetRoundnessButtonCenter(){
  setModeMenuSettingsDisplay();
}


#endif