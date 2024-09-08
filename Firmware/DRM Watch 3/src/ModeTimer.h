#ifndef MODETIMER_H
#define MODETIMER_H

/*PROTOTYPES*/
void modeTimerLoop();
void setModeTimer();
void modeTimerButtonUp();
void modeTimerButtonCenter();
void modeTimerButtonDown();
int timerStep();
bool timerAlert();
void resetTimer();

#include "Global.h"
#include "AutoSleep.h"
#include "Button.h"
#include <Arduino.h>


void setModeTimer(){
  clearScreenAnimation();
  Serial.println(F("Set mode: timer"));
  modeSetup = setModeTimer;
  modeLoop = modeTimerLoop;
  modeButtonUp = modeTimerButtonUp;
  modeButtonCenter = modeTimerButtonCenter;
  modeButtonDown = modeTimerButtonDown;
  modeButtonUpLong = 0;
  modeButtonCenterLong = 0;
  modeButtonDownLong = 0;
  registerAction();
  enableAutoReturn = true;
  enableAutoSleep = false; 
  autoReturnTime = autoReturnDefaultTime;
  autoSleepTime = autoSleepDefaultTime;
}

void modeTimerLoop(){
  lcd()->setColorIndex(white);
  lcd()->drawBox(0, 0, 400, 240);

  lcd()->setFont(u8g2_font_10x20_t_cyrillic);  //ok
  lcd()->setColorIndex(black);
  lcd()->setCursor(5, 18); 
  lcd()->print(L("Таймер", "Timer"));

  drawStatusbar(363, 1, true);
  unsigned long startedTime = getTimerStartedTime();
  unsigned long now = rtcGetEpoch();
  unsigned long timerTime = getTimerTime();
  unsigned long remainingTime = timerTime;
  if(isTimerRunning()){
    remainingTime = startedTime+timerTime-now;
  }


  lcd()->setColorIndex(black);
  lcd()->setFont(u8g2_font_logisoso38_tn); //u8g2_font_inr24_t_cyrillic
  lcd()->setCursor(90, 130);
  displayPrintSecondsAsTime(remainingTime);
  
  lcd()->setColorIndex(black);
  lcd()->drawBox(369, 0, 2, 260);  //draw_ic16_repeat  draw_ic16_arrow_right  draw_ic16_back
  
  
  if(isTimerRunning()){
    //draw_ic16_back(lx(), ly2(), black);
    draw_ic16_watchface(lx(), ly2(), black);
    draw_ic16_repeat(lx(), ly3(), black);
  }
  else{
    if(getTimerTime() == 0){
      //draw_ic16_back(lx(), ly2(), black);
      draw_ic16_watchface(lx(), ly2(), black);
    }
    else{
      draw_ic16_arrow_right(lx(), ly2(), black);
    }
    draw_ic16_plus(lx(), ly1(), black);
    draw_ic16_minus(lx(), ly3(), black);
  }
  lcd()->sendBuffer();
}

void modeTimerButtonUp(){
  if(isTimerRunning()){

  }
  else{
    unsigned long timerTime = getTimerTime();
    timerTime += timerStep();
    saveTimerTime(timerTime);
  }
}
void modeTimerButtonDown(){
  if(isTimerRunning()){
    resetTimer();
  }
  else{
    unsigned long timerTime = getTimerTime();
    if(timerTime > timerStep())
      timerTime -= timerStep();
    else
      timerTime = 0;
    saveTimerTime(timerTime);
  }
}
void modeTimerButtonCenter(){
  if(isTimerRunning()){
    //setModeAppsMenu(); //exit
    setModeWatchface();
  }
  else{
    if(getTimerTime() == 0){
      //setModeAppsMenu(); //exit
      setModeWatchface();
    }
    else{
      saveTimerStartedTime(rtcGetEpoch());//start
    }
  }
}

void setTimerToMinutes(int minutes){
  saveTimerTime(60*minutes);
  saveTimerStartedTime(rtcGetEpoch());//start
}
void resetTimer(){
  saveTimerStartedTime(0); //reset
  saveTimerTime(0);
}

void timerLoop(){
  if(isTimerRunning()){
    unsigned long startedTime = getTimerStartedTime();
    unsigned long now = rtcGetEpoch();
    unsigned long timerTime = getTimerTime();
    if(now >= startedTime+timerTime){
      bool alertResult = timerAlert();  //return true if was played completely or false if interrupted
      //schedule timer if alert was missed          
      if(alertResult && !isTimerRunning())
        setTimerToMinutes(10);
    }
  }
}

int timerStep(){
  unsigned long timerTime = getTimerTime();
  if(timerTime >= 3*60*60) /*3h*/ return 60*60; /*1h*/
  if(timerTime >= 60*60) /*1h*/ return 30*60; /*30h*/
  if(timerTime >= 30*60) /*30m*/ return 5*60; /*5m*/
  if(timerTime >= 5*60) /*5m*/ return 60; /*1m*/
  return 30; /*30s*/
}


//return true if was played completely or false if interrupted
bool timerAlert(){
  Serial.println(F("Open: timerAlert"));
  clearScreenAnimation();
  wakeup();
  backlightOn();

  resetTimer();
  bool result = shortcutRun(EVENT_TIMER);
  modeSetup(); 
  return result;
}


#endif