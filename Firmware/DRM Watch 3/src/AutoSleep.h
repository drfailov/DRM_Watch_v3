#ifndef AUTOSLEEP_H
#define AUTOSLEEP_H

/*PROTOTYPES*/
long autoReturnTimeMs();
void goToSleep();
long autoSleepTimeMs();
bool isAwake();
long timeToAutoAction();
bool isDontSleep();
void wakeup();


#define NO_SLEEP 99999999

bool dontSleep = false;  //permanent
bool wakeupFromSleep = false; //temporary
const int autoReturnDefaultTime = 180000;//ms
const int autoSleepDefaultTime = 15000;//ms
const int autoSleepDefaultTimeWhenFlashlightOn = 360000;//ms
bool enableAutoReturn = false; //is set when new mode selected
bool enableAutoSleep = false; //is set when new mode selected
int autoReturnTime = autoReturnDefaultTime;//ms
int autoSleepTime = autoSleepDefaultTime;//ms


#include "Lcd.h"
#include "ModeOff.h"
#include "ModeWatchface.h"
#include "AutoSleep.h"
#include "Battery.h"
#include "Backlight.h"


void autoSleepLoop(){
  if(sinceLastAction() > autoReturnTimeMs()) //auto go to watchface
    setModeWatchface();
  
  if(sinceLastAction() > autoSleepTimeMs())
    goToSleep();

  
  if(esp_sleep_get_wakeup_cause() == ESP_SLEEP_WAKEUP_TIMER && !wakeupFromSleep)/*periodical wakeup*/
    goToSleep();
}

long autoReturnTimeMs(){
  if(dontSleep)
    return NO_SLEEP;
  if(enableAutoSleep)
    return NO_SLEEP;
  if(!enableAutoReturn && batteryBars()>1)
    return NO_SLEEP;
  return autoReturnTime;
}

long autoSleepTimeMs(){
  if(!enableAutoSleep)
    return NO_SLEEP;
  if(isChargerConnected())
    return NO_SLEEP;
  if(dontSleep)
    return NO_SLEEP;
  if (isFlashlightOn())
    return autoSleepDefaultTimeWhenFlashlightOn;
  return autoSleepTime;
}

long timeToAutoAction(){
  long autoSleepTime = autoSleepTimeMs();
  long autoReturnTime = autoReturnTimeMs();
  long minTime = min(autoSleepTime,autoReturnTime);
  return minTime - sinceLastAction();
}

//temporary dont go to sleep (wekeup for timer or alarm)
void wakeup()
{ 
  wakeupFromSleep = true;
}

bool isAwake(){//return true only if not sleepong or about to sleep
  if(esp_sleep_get_wakeup_cause() == ESP_SLEEP_WAKEUP_TIMER && !wakeupFromSleep)/*periodical wakeup*/
    return false;
  return sinceLastAction() < autoSleepTimeMs()-1000;
}


void goToSleep(){
  /*
  Battery Capacity: 250mA
  Deep sleep mode: 0.081mA  (Time = 250/0.081 = 3086h = 128d = 4.2month)
  Active mode: 40mA (Time = 250/40 = 6.25h)
  Music mode: 97mA  (Time = 250/97 = 2.57h)
  */
  if(!isOff()){
    lcd()->setColorIndex(0);
    draw_ic16_empty(lx(), ly1(), black);
    draw_ic16_unlock(lx(), ly2(), black);
    draw_ic16_empty(lx(), ly3(), black);
    lcd()->sendBuffer();
    delay(20);
  }

  backlightOff();  
  
  esp_sleep_enable_ext0_wakeup(BUT_CENTER, 0); //1 = High, 0 = Low
  
  if(isOff() || isBatteryCritical())
    esp_sleep_enable_timer_wakeup(600 * 1000000ULL);
  else
    esp_sleep_enable_timer_wakeup(55 * 1000000ULL);
  esp_deep_sleep_start();
}

bool isDontSleep(){
  return dontSleep || (!enableAutoReturn && !enableAutoSleep);
}


#endif