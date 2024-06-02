#ifndef MODEWATCHFACE_H
#define MODEWATCHFACE_H

/*PROTOTYPES*/
int getWatchfaceCount();
String getWatchfaceName(int index);
WatchfaceDrawable getWatchfaceDrawable(int index);
int drawStatusbar(int x, int y, bool drawTime);

void setModeWatchface();
void modeWatchfaceLoop();
void modeWatchfaceButtonUp();
void modeWatchfaceButtonCenter();
void modeWatchfaceButtonDown();
void modeWatchfaceButtonUpLong();
void modeWatchfaceButtonCenterLong();

#include "Global.h"
#include "AutoSleep.h"
#include "Button.h"
#include "WatchfaceFullscreen.h"
#include "WatchfaceSide.h"

struct Watchface {             
  String name;   
  WatchfaceDrawable watchface;         
  Watchface(String _name, WatchfaceDrawable _wf):name(_name), watchface(_wf) {}
};    

Watchface watchfaces[]={
  {"Боковий", drawWatchfaceSide},
  {"Повноекранний", drawWatchfaceFullscreen}
};

int getWatchfaceCount(){
   return sizeof(watchfaces)/sizeof(watchfaces[0]);
}

String getWatchfaceName(int index){
   return watchfaces[index].name;
}

WatchfaceDrawable getWatchfaceDrawable(int index){
   return watchfaces[index].watchface;
}




void setModeWatchface(){
  clearScreenAnimation();
  Serial.println(F("Set mode: Watchface"));
  modeSetup = setModeWatchface;
  modeLoop = modeWatchfaceLoop;
  modeButtonUp = modeWatchfaceButtonUp;
  modeButtonCenter = modeWatchfaceButtonCenter;
  modeButtonDown = modeWatchfaceButtonDown;
  modeButtonUpLong = modeWatchfaceButtonUpLong;
  modeButtonCenterLong = modeWatchfaceButtonCenterLong;
  modeButtonDownLong = 0;
  enableAutoReturn = false;
  enableAutoSleep = true; 
  autoReturnTime = autoReturnDefaultTime;
  autoSleepTime = autoSleepDefaultTime;
  registerAction();
}

void modeWatchfaceLoop(){ 
  if(esp_sleep_get_wakeup_cause() == ESP_SLEEP_WAKEUP_TIMER) //make auto sync only when watch is sleeping and not interrupt user
    loopTimeAutoSync();  

  //drawWatchfaceBig(firstDraw);
  //drawWatchfaceLife(firstDraw);  
  //drawWatchfaceArrows(firstDraw);
  getWatchfaceDrawable(getWatchface())(firstDraw);

  if(esp_sleep_get_wakeup_cause() != ESP_SLEEP_WAKEUP_TIMER) //if wake by timer, don't refresh display to keep image static, image will refresh when go to lock screen and drawing lock icon
    lcd()->sendBuffer();

}

void modeWatchfaceButtonUp(){
  //ledFlashlightToggleTop();
  setModeStopwatch();
}
void modeWatchfaceButtonUpLong(){
  setModeTimer();
}

void modeWatchfaceButtonCenter(){
  //ledFlashlightToggleBottom();
  ledFlashlightToggle();
}
void modeWatchfaceButtonCenterLong(){
  setModeOff();
}

void modeWatchfaceButtonDown(){
  setModeAppsMenu();
}

void switchDontSleep(){
  dontSleep=!dontSleep;
  if(dontSleep)
    buttonLongBeep();
}


int drawStatusbar(int x, int y, bool drawTime){
  return drawStatusbar(x,y,drawTime,false);
}
int drawStatusbar(int x, int y, bool drawTime, bool simulate){ //simulate is draw nothing, only measure 
  int interval = 5;
  
  if(!simulate){
    int xLeft = drawStatusbar(x,y,drawTime,true);
    int width = x-xLeft;
    int height = 30;
    lcd()->setColorIndex(white);
    lcd()->drawBox(xLeft, y, width+interval, height);
  }

  if(drawTime){
    lcd()->setColorIndex(black);
    lcd()->setFont(u8g2_font_10x20_t_cyrillic);
    int h = rtcGetHour();
    int m = rtcGetMinute();
    sprintf(buffer, "%02d:%02d", h,m);
    int width = lcd()->getStrWidth(buffer);
    x-=width;
    lcd()->setCursor(x, y+18); 
    if(!simulate)lcd()->print(buffer);
    x-=interval;
  }
  { //Serial.println("draw battery...");
    x -= 24;
    if(!simulate)drawBattery(x, y);
    x-=interval;
  }
  if(isFlashlightOn()){
    x -= 16;
    if(!simulate)draw_ic16_flashlight(x, y+4, black);
    x-=interval;
  }
  if(dontSleep){
    x -= 16;
    if(!simulate)draw_ic16_coffee(x, y+4, black);
    x-=interval;
  }
  if(isTimerRunning()){ //draw timer remaining
    unsigned long startedTime = getTimerStartedTime();
    unsigned long now = rtcGetEpoch();
    unsigned long timerTime = getTimerTime();
    unsigned long remainingTime = startedTime+timerTime-now;
    if(startedTime+timerTime<now) remainingTime = 0;
    unsigned long h = hoursFromSec(remainingTime);
    unsigned long m = minutesFromSec(remainingTime);
    lcd()->setColorIndex(black);
    lcd()->setFont(u8g2_font_unifont_t_cyrillic);
    sprintf(buffer, "%02d:%02d", h,m);
    int width = lcd()->getStrWidth(buffer);
    x-=width;
    lcd()->setCursor(x, y+17); 
    if(!simulate)lcd()->print(buffer);
    
    x -= 15;
    if(!simulate)draw_ic16_timer(x, y+4, black);
    x-=interval;
  }
  if(isStopwatchRunning()){
    unsigned long now = rtcGetEpoch();
    unsigned long start = getStopwatchStartedTime();
    unsigned long end = getStopwatchFinishedTime();
    if(start != 0 && end == 0) end = now;
    unsigned long dd = end-start;
    unsigned long h = hoursFromSec(dd);
    unsigned long m = minutesFromSec(dd);
    lcd()->setColorIndex(black);
    lcd()->setFont(u8g2_font_unifont_t_cyrillic);
    sprintf(buffer, "%02d:%02d", h,m);
    int width = lcd()->getStrWidth(buffer);
    x-=width;
    lcd()->setCursor(x, y+17); 
    if(!simulate)lcd()->print(buffer);

    x -= 16;
    if(!simulate)draw_ic16_stopwatch(x, y+4, black);
    x-=interval;
  }
  if(getAnyAlertEnabled()){
    x -= 16;
    if(!simulate)draw_ic16_alert(x, y+4, black);
    x-=interval;
  }
  if(getMuteEnabled()){
    x -= 16;
    if(!simulate)draw_ic16_sound_mute(x, y+4, black);
    x-=interval;
  }
  if(!isExternalRtcActive()){
    x -= 16;
    if(!simulate)draw_ic16_warning(x, y+4, black);
    x-=interval;
  }
  
  if(isAwake()){
    long timeToAction = timeToAutoAction();
    long s = timeToAction/1000;
    if(timeToAction < 60000) {
      if(enableAutoSleep){  //Якщо цей режим спить - показувати таймер до сну
        String text = String("")+s;
        lcd()->setColorIndex(black);
        lcd()->setFont(u8g2_font_unifont_t_cyrillic);
        int width = lcd()->getStrWidth(text.c_str());
        int margin = 4;
        x -= width+margin*2;
        if(!simulate)lcd()->drawRBox(/*x*/x, /*y*/y+4, /*w*/width+margin*2, /*h*/ 16, /*r*/3);
        lcd()->setColorIndex(white);
        lcd()->setCursor(x+margin, y+17); 
        if(!simulate)lcd()->print(text);
        x-=interval;
      }
      else{    // Якщо цей режим НЕ спить - показувати таймер до повернення на циферблат
        lcd()->setColorIndex(black);
        lcd()->setFont(u8g2_font_unifont_t_cyrillic);
        sprintf(buffer, "%02ds", s);
        int width = lcd()->getStrWidth(buffer);
        x-=width;
        lcd()->setCursor(x, y+17); 
        if(!simulate)lcd()->print(buffer);
        x -= 16;
        if(!simulate)draw_ic16_watchface(x, y+4, black);
        x-=interval;
      }
    }
  }


  return x;
}


#endif