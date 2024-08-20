#ifndef MODEABOUT_H
#define MODEABOUT_H

/*PROTOTYPES*/
void modeAboutSetup();
void modeAboutLoop();
void modeAboutButton();


#include "Lcd.h"
#include "Global.h"
#include "AutoSleep.h"
#include "Button.h"
#include "ModeWatchface.h"
#include "Icons.h"
#include "SettingsMenu/ModeMenuSettings.h"


int modeAboutCounter = 0;
void modeAboutSetup(){
  if(modeExit != 0)
    modeExit();
  clearScreenAnimation();
  Serial.println(F("Set mode: About"));
  modeSetup = modeAboutSetup;
  modeLoop = modeAboutLoop;
  modeButtonUp = modeAboutButton;
  modeButtonCenter = modeAboutButton;
  modeButtonDown = modeAboutButton;
  modeButtonUpLong = 0;
  modeButtonCenterLong = 0;
  modeButtonDownLong = 0;
  registerAction();
  enableAutoReturn = true;
  enableAutoSleep = false; 
  autoReturnTime = autoReturnDefaultTime;
  autoSleepTime = autoSleepDefaultTime;
  modeAboutCounter = 0;
}

void modeAboutLoop(){
  lcd()->setColorIndex(white);
  lcd()->drawBox(0, 0, 400, 240);
  lcd()->setFont(u8g2_font_10x20_t_cyrillic);  //ok
  lcd()->setColorIndex(black);
  lcd()->setCursor(5, 17);  lcd()->print("Розробник: Dr.Failov");
  drawStatusbar(395, 1, true);
  lcd()->setFont(u8g2_font_10x20_t_cyrillic);  //ok
  lcd()->setColorIndex(black);
  lcd()->setCursor(5, 234);  lcd()->print(version);
  draw_ic33_qrcodex1(355, 180, black);
  #ifdef HW_REV_1
  lcd()->setCursor(5, 216);  lcd()->print("HW:Rev1");
  #endif
  #ifdef HW_REV_2
  lcd()->setCursor(5, 216);  lcd()->print("HW:Rev2");
  #endif
  #ifdef HW_REV_3
  lcd()->setCursor(5, 216);  lcd()->print("HW:Rev3");
  #endif
  #ifdef HW_REV_4
  lcd()->setCursor(5, 216);  lcd()->print("HW:Rev4");
  #endif
  lcd()->setCursor(105, 234);  lcd()->print("Розроблено в Україні");
  lcd()->setCursor(355, 234);  lcd()->print("2024");

  if(modeAboutCounter == 0){   //initial draw with animation
    displayDrawVector(getPathZubat(), 40, 60, 3.0, 3, 3, black);
    displayDrawVector(getPathDrmWatch(), 190, 60, 3.0, 2, 3, black);
    lcd()->setCursor(340, 164); lcd()->setFont(u8g2_font_logisoso38_tn);  lcd()->print("3");
  }
  else if(modeAboutCounter < 10){  //hold without animatioon for 10 frames
    displayDrawVector(getPathZubat(), 40, 60, 3.0, 3, 0, black);
    displayDrawVector(getPathDrmWatch(), 190, 60, 3.0, 2, 0, black);
    lcd()->setCursor(340, 164); lcd()->setFont(u8g2_font_logisoso38_tn);  lcd()->print("3");
  }
  else if(modeAboutCounter%2==0){  //draw and animate erasing
    displayDrawVector(getPathZubat(), 40, 60, 3.0, 3, 0, black);
    displayDrawVector(getPathDrmWatch(), 190, 60, 3.0, 2, 0, black);
    lcd()->setCursor(340, 164); lcd()->setFont(u8g2_font_logisoso38_tn);  lcd()->print("3");
    displayDrawVector(getPathDrmWatch(), 190, 60, 3.0, 2, 5, white);
  }
  else{     //animate appearing
    displayDrawVector(getPathZubat(), 40, 60, 3.0, 3, 0, black);
    displayDrawVector(getPathDrmWatch(), 190, 60, 3.0, 2, 3, black);
    lcd()->setCursor(340, 164); lcd()->setFont(u8g2_font_logisoso38_tn);  lcd()->print("3");
    lcd()->sendBuffer();
    for(unsigned long started = millis(); millis()-started<300; ) if(isAnyButtonPressed()) break;
  }
  lcd()->sendBuffer();
  modeAboutCounter++;
}

void modeAboutButton(){
  setModeSettingsMenu();
}



#endif