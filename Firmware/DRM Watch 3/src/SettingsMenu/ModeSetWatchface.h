#ifndef MODESETWATCHFACE_H
#define MODESETWATCHFACE_H

void setModeSetWatchface();
void ModeSetWatchfaceLoop();
void ModeSetWatchfaceButtonUp();
void ModeSetWatchfaceButtonCenter();
void ModeSetWatchfaceButtonDown();

#include "../Global.h"
#include "../AutoSleep.h"
#include "../Button.h"

void setModeSetWatchface(){
  clearScreenAnimation();
  Serial.println(F("Set mode: SetWatchface"));
  modeSetup = setModeSetWatchface;
  modeLoop = ModeSetWatchfaceLoop;
  modeButtonUp = ModeSetWatchfaceButtonUp;
  modeButtonCenter = ModeSetWatchfaceButtonCenter;
  modeButtonDown = ModeSetWatchfaceButtonDown;
  modeButtonUpLong = 0;
  modeButtonCenterLong = 0;
  modeButtonDownLong = 0;
  registerAction();
  enableAutoReturn = true;
  enableAutoSleep = false; 
  autoReturnTime = autoReturnDefaultTime;
  autoSleepTime = autoSleepDefaultTime;
}

void ModeSetWatchfaceLoop(){ 
  int index = getWatchface();
  getWatchfaceDrawable(index)(false);

  String name = getWatchfaceName(index);
  drawMessage("Вибір циферблату", name.c_str());

  draw_ic16_arrow_up(lx(), ly1(), black);
  draw_ic16_back(lx(), ly2(), black);
  draw_ic16_arrow_down(lx(), ly3(), black);

  lcd()->sendBuffer();
}

void ModeSetWatchfaceButtonUp(){
  int index = getWatchface();
  index++;
  saveWatchface(index);
}

void ModeSetWatchfaceButtonCenter(){
  setModeMenuSettingsDisplay();
}

void ModeSetWatchfaceButtonDown(){
  int index = getWatchface();
  int total = getWatchfaceCount();
  index--;
  if(index<0) index=total-1;
  saveWatchface(index);
}


//----------------------//----------------------//----------------------//----------------------//----------------------//----------------------//----------------------

#endif
