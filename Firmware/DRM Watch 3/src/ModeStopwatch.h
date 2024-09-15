#ifndef MODESTOPWATCH_H
#define MODESTOPWATCH_H
//CTRL+K CTRL+0    (COLLAPSE)                 //CTRL+K   CTRL+J  (EXPAND)

/*PROTOTYPES*/
void setModeStopwatch();
void modeStopwatchButtonUp();
void modeStopwatchButtonCenter();
void modeStopwatchLoop();
void modeStopwatchButtonDown();


#include "Global.h"
#include "AutoSleep.h"
#include "Button.h"
#include "RTC.h"

const int itemModeStopwatchBack=0;
const int itemModeStopwatchStartStop=1;
const int itemModeStopwatchReset=2;

unsigned long modeStopwatchStartedMillis = 0;

void setModeStopwatch(){
  clearScreenAnimation();
  Serial.println(F("Set mode: Stopwatch"));
  modeSetup = setModeStopwatch;
  modeLoop = modeStopwatchLoop;
  modeButtonUp = modeStopwatchButtonUp;
  modeButtonCenter = modeStopwatchButtonCenter;
  modeButtonDown = modeStopwatchButtonDown;
  modeButtonUpLong = 0;
  modeButtonCenterLong = 0;
  modeButtonDownLong = 0;
  registerAction();
  enableAutoReturn = false;
  enableAutoSleep = true; 
  autoReturnTime = autoReturnDefaultTime;
  autoSleepTime = autoReturnDefaultTime;
  selected = 1;
  items = 3;
}


void modeStopwatchLoop(){ 
  lcd()->setColorIndex(white);
  lcd()->drawBox(0, 0, 400, 240);
  
  lcd()->setFont(u8g2_font_10x20_t_cyrillic);  //ok
  lcd()->setColorIndex(black);
  lcd()->setCursor(5, 18); 
  lcd()->print(L("Секундомір", "Stopwatch"));
  drawStatusbar(363, 1, true);  
  
  lcd()->setColorIndex(black);
  lcd()->drawBox(369, 0, 2, 260);  //draw_ic16_repeat  draw_ic16_arrow_right  draw_ic16_back
  if(isStopwatchRunning())
    draw_ic16_pause(lx(), ly1(), black); 
  else
    draw_ic16_arrow_right(lx(), ly1(), black); 
  //draw_ic16_back(lx(), ly2(), black);
  draw_ic16_watchface(lx(), ly2(), black);
  draw_ic16_repeat(lx(), ly3(), black);
  
  {
    unsigned long now = rtcGetEpoch();
    unsigned long start = getStopwatchStartedTime();
    unsigned long end = getStopwatchFinishedTime();
    if(start != 0 && end == 0) end = now;
    unsigned long dd = end-start;

    
    lcd()->setColorIndex(black);
    lcd()->setFont(u8g2_font_logisoso38_tn); //u8g2_font_inr24_t_cyrillic
    lcd()->setCursor(90, 80);
    displayPrintSecondsAsTime(dd);
    if(isStopwatchRunning()){
      unsigned long sinceStarted = millis()-modeStopwatchStartedMillis;
      lcd()->setFont(u8g2_font_10x20_t_cyrillic);
      lcd()->print(":");
      lcd()->print(sinceStarted%1000);
    }
  }

  //draw history
  for(int i=1; i<=stopwatchHistorySlotCnt(); i++){
    int x = 45;
    int y = 75 + 27*i;
    lcd()->setColorIndex(black);
    lcd()->drawFrame(x, y, 270, 24);
    if(!stopwatchHistorySlotIsEmpty(i))
    {
      unsigned long start = stopwatchHistorySlotStart(i);
      unsigned long end = stopwatchHistorySlotEnd(i);
      unsigned long dd = end-start;
      lcd()->setFont(u8g2_font_10x20_t_cyrillic);  //ok
      lcd()->setCursor(x + 10, y+18);
      displayPrintSecondsAsTime(dd);
      lcd()->setFont(u8g2_font_unifont_t_cyrillic);
      lcd()->setCursor(x + 180, y+17);
      printDate(start);
    }    
  }

  lcd()->sendBuffer();
}

void modeStopwatchButtonCenter(){
  //setModeAppsMenu(); 
  setModeWatchface();
}

void modeStopwatchButtonUp(){
  if(isStopwatchRunning()){ //do PAUSE
    saveStopwatchFinishedTime(rtcGetEpoch());
  }
  else{ //do RESUME
    unsigned long now = rtcGetEpoch();
    unsigned long stopwatchStarted = getStopwatchStartedTime();
    unsigned long stopwatchFinished = getStopwatchFinishedTime();
    unsigned long dt = stopwatchFinished - stopwatchStarted;
    unsigned long newStartedTime = now - dt;
    saveStopwatchStartedTime(newStartedTime);
    saveStopwatchFinishedTime(0);
    modeStopwatchStartedMillis = millis();
  }
} 

void modeStopwatchButtonDown(){ 
  //do nothing if zeros
  unsigned long start = getStopwatchStartedTime();
  unsigned long end = getStopwatchFinishedTime();
  unsigned long dd = end-start;
  if(dd == 0)
    return;

  //shift history
  for(int i=stopwatchHistorySlotCnt(); i>=2; i--){
     saveStopwatchHistorySlotStart(i, stopwatchHistorySlotStart(i-1));
     saveStopwatchHistorySlotEnd(i, stopwatchHistorySlotEnd(i-1));
  }

  if(isStopwatchRunning()){  //save history and do RESET
    unsigned long now = rtcGetEpoch();
    saveStopwatchHistorySlotStart(1, getStopwatchStartedTime());
    saveStopwatchHistorySlotEnd(1, now);
    //saveStopwatchStartedTime(now);
  }
  else
  {   //save history and clear values
    saveStopwatchHistorySlotStart(1, getStopwatchStartedTime());
    saveStopwatchHistorySlotEnd(1, getStopwatchFinishedTime());
  }
  saveStopwatchStartedTime(0);
  saveStopwatchFinishedTime(0);
}


#endif
