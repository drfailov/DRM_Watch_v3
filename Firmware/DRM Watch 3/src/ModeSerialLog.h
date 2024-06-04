#ifndef modeSerialLog_H
#define modeSerialLog_H

/*PROTOTYPES*/
void setmodeSerialLog();
void modeSerialLogLoop();
void modeSerialLogButtonUp();
void modeSerialLogButtonCenter();
void modeSerialLogButtonDown();
void wakeup_reason();


#include <Arduino.h>
#include "Global.h"
#include "AutoSleep.h"
#include "Button.h"
#include "Battery.h"
#include "DrmPreferences.h"


U8G2LOG u8g2log;
#define LOG_W 49   //SYMBOLS
#define LOG_H 17   //SYMBOLS
uint8_t u8log_buffer[LOG_W*LOG_H];


void setmodeSerialLog(){
  clearScreenAnimation();
  Serial.println(F("Set mode: SerialLOG"));
  modeSetup = setmodeSerialLog;
  modeLoop = modeSerialLogLoop;
  modeButtonUp = modeSerialLogButtonUp;
  modeButtonCenter = modeSerialLogButtonCenter;
  modeButtonDown = modeSerialLogButtonDown;
  modeButtonUpLong = 0;
  modeButtonCenterLong = 0;
  modeButtonDownLong = 0;
  registerAction();
  enableAutoReturn = false;
  enableAutoSleep = false; 
  autoReturnTime = autoReturnDefaultTime;
  autoSleepTime = autoSleepDefaultTime;
  u8g2log.begin(LOG_W, LOG_H, u8log_buffer);
  u8g2log.setLineHeightOffset(0);
  
}

void modeSerialLogLoop(){ 
  //u8g2log.print(millis());
  //u8g2log.print("-");

  while(Serial.available()){
    u8g2log.print((char)Serial.read());
  }

  lcd()->setColorIndex(white);
  lcd()->drawBox(0, 0, 400, 240);

  lcd()->setColorIndex(black);
  lcd()->setFont(u8g2_font_unifont_t_cyrillic); //smalll
  lcd()->setCursor(5, 17);  lcd()->print("ASCII Mode");

  drawStatusbar(392, 0, true);
  
  lcd()->setColorIndex(black);
  lcd()->drawBox(0, 23, 400, 2);

  lcd()->setColorIndex(black);
  lcd()->setFont(u8g2_font_unifont_t_cyrillic); //smalll
  lcd()->drawLog(2, 39, u8g2log);
  //lcd()->print("_");
  lcd()->sendBuffer();
  delay(10);
}

void modeSerialLogButtonUp(){
  //goToSleep();
  switchDontSleep();
}

void modeSerialLogButtonCenter(){
  setModeAppsMenu();
}

void modeSerialLogButtonDown(){
}






//----------------------//----------------------//----------------------//----------------------//----------------------//----------------------//----------------------




#endif
