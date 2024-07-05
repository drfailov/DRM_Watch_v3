#ifndef ModeDotsNOIZE_H
#define ModeDotsNOIZE_H

void setModeDots();
void ModeDotsLoop();
void ModeDotsButtonUp();
void ModeDotsButtonCenter();
void ModeDotsButtonDown();
void dotsStep();

#include "Global.h"
#include "AutoSleep.h"
#include "Button.h"
#include "ModeMenuBackgrounds.h"


void setModeDots(){
  clearScreenAnimation();
  Serial.println(F("Set mode: Dots"));
  modeSetup = setModeDots;
  modeLoop = ModeDotsLoop;
  modeButtonUp = ModeDotsButtonUp;
  modeButtonCenter = ModeDotsButtonCenter;
  modeButtonDown = ModeDotsButtonDown;
  modeButtonUpLong = 0;
  modeButtonCenterLong = 0;
  modeButtonDownLong = 0;
  registerAction();
  enableAutoReturn = false;
  enableAutoSleep = false; 
  autoReturnTime = autoReturnDefaultTime;
  autoSleepTime = autoSleepDefaultTime;
  zeroScreenBuffer();
}

void ModeDotsLoop(){ 
  dotsStep();
  drawScreenBuffer();
  lcd()->sendBuffer();
}

void dotsStep(){
  for(int i=0; i<10; i++)
    setScreenBuffer(rand()%BUFF_W, rand()%BUFF_H, true);
}

void ModeDotsButtonUp(){
  setModeBackgroundsMenu();
}

void ModeDotsButtonCenter(){
  setModeBackgroundsMenu();
}

void ModeDotsButtonDown(){
  setModeBackgroundsMenu();
}





#endif