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
#define LOG_W 45   //SYMBOLS
#define LOG_H 17   //SYMBOLS
uint8_t u8log_buffer[LOG_W*LOG_H];
#define SERIAL_LOG_MODE_ASCII  1
#define SERIAL_LOG_MODE_HEX  2
int serialLogMode = SERIAL_LOG_MODE_ASCII;
bool serialLogPaused = false;
long bytesCounter = 0;
long lastTimeDataReceived = 0;
bool enterAdded = true;


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

  while(Serial.available())
  {
    bytesCounter++;
    lastTimeDataReceived = millis();
    enterAdded = false;
    byte b = Serial.read();
    if(!serialLogPaused){
      if(serialLogMode == SERIAL_LOG_MODE_ASCII){
        u8g2log.print((char)b);
      }
      if(serialLogMode == SERIAL_LOG_MODE_HEX){
        if(b<16) u8g2log.print("0");
        u8g2log.print(b, 16);
        u8g2log.print(" ");
      }
    }
  }
  if(millis()-lastTimeDataReceived>1000 && !enterAdded && !serialLogPaused){
    u8g2log.print("\n");
    enterAdded = true;
  }
    
  
  //FILL WHITE
  lcd()->setColorIndex(white);
  lcd()->drawBox(0, 0, 400, 240);

  //DRAW HEADER
  lcd()->setColorIndex(black);
  lcd()->setFont(u8g2_font_unifont_t_cyrillic); //smalll
  lcd()->setCursor(5, 17);  
  if(serialLogPaused){
    lcd()->print("PAUSED");
  }
  else if(serialLogMode == SERIAL_LOG_MODE_ASCII){
    lcd()->print("ASCII");
  }
  else if(serialLogMode == SERIAL_LOG_MODE_HEX){
    lcd()->print("HEX");
  }
  lcd()->print(" (");
  lcd()->print(bytesCounter);
  lcd()->print(" byte)");
  
  drawStatusbar(363, 1, true);
  lcd()->setColorIndex(black);
  lcd()->drawBox(0, 23, 369, 2);
  
  //DRAW LEGEND
  lcd()->setColorIndex(black);
  lcd()->drawBox(369, 0, 2, 260);
  if(serialLogPaused)
    draw_ic16_arrow_right(lx(), ly1(), black);
  else
    draw_ic16_pause(lx(), ly1(), black);
  draw_ic16_back(lx(), ly2(), black);
  draw_ic16_hashtag(lx(), ly3(), black);
  
  //DRAW LOG
  lcd()->setColorIndex(black);
  lcd()->setFont(u8g2_font_unifont_t_cyrillic); //smalll
  lcd()->drawLog(2, 39, u8g2log);


  lcd()->sendBuffer();
  delay(1);
}

void modeSerialLogButtonUp(){
  serialLogPaused = !serialLogPaused;
}

void modeSerialLogButtonCenter(){
  setModeAppsMenu();
}

void modeSerialLogButtonDown(){
  serialLogMode = serialLogMode == SERIAL_LOG_MODE_ASCII?SERIAL_LOG_MODE_HEX:SERIAL_LOG_MODE_ASCII;
}






//----------------------//----------------------//----------------------//----------------------//----------------------//----------------------//----------------------




#endif
