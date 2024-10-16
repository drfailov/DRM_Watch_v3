#ifndef BACKLIGHT_H
#define BACKLIGHT_H

void backlightOn();
void backlightOff();

#include <Arduino.h>
#include "AutoSleep.h"


unsigned long backlightEnabledTime = 0;
unsigned long backlightTimeoutMs = 25000;

void backlightInit(){
  if(isAwake() && !isOff()){/*NOT periodical wakeup*/ //==================================== BACKLIGHT
    backlightOn();
  }
}

void backlightLoop(){
  if(isAwake()){/*NOT periodical wakeup*/ //==================================== BACKLIGHT
    if(millis()-backlightEnabledTime > backlightTimeoutMs && !dontSleep){ //==================================== BACKLIGHT
      backlightOff();
    }
  }
}

void backlightOn(){
  pinMode(BACKLIGHT_EN, OUTPUT);
  digitalWrite(BACKLIGHT_EN, HIGH);
  backlightEnabledTime = millis();
}

void backlightOff(){
  digitalWrite(BACKLIGHT_EN, LOW);
  pinMode(BACKLIGHT_EN, INPUT);
}


#endif