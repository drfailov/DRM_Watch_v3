#ifndef MODERANDOMNOIZE_H
#define MODERANDOMNOIZE_H

void setModeRandom();
void ModeRandomLoop();
void ModeRandomButtonUp();
void ModeRandomButtonCenter();
void ModeRandomButtonDown();

#include "Global.h"
#include "AutoSleep.h"
#include "Button.h"


void setModeRandom(){
  clearScreenAnimation();
  Serial.println(F("Set mode: Random"));
  modeSetup = setModeRandom;
  modeLoop = ModeRandomLoop;
  modeButtonUp = ModeRandomButtonUp;
  modeButtonCenter = ModeRandomButtonCenter;
  modeButtonDown = ModeRandomButtonDown;
  modeButtonUpLong = 0;
  modeButtonCenterLong = 0;
  modeButtonDownLong = 0;
  registerAction();
  enableAutoReturn = false;
  enableAutoSleep = false; 
  autoReturnTime = autoReturnDefaultTime;
  autoSleepTime = autoSleepDefaultTime;
  randScreenBuffer();
}

void ModeRandomLoop(){ 
  drawScreenBuffer();
  lcd()->sendBuffer();
  unsigned long millisStarted = millis();
  randScreenBuffer();
  unsigned long millisEnd = millis();
  Serial.print("Random step: "); Serial.print(millisEnd-millisStarted); Serial.println("ms.");
}

void ModeRandomButtonUp(){
  setModeAppsMenu();
}

void ModeRandomButtonCenter(){
  setModeAppsMenu();
}

void ModeRandomButtonDown(){
  setModeAppsMenu();
}





#endif