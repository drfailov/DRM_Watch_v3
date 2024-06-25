#ifndef BUZZER_H
#define BUZZER_H

/*PROTOTYPES*/
void buzNoTone();
void buzTone(int _freq);
void buzTone(int _freq, int duration);
void buzPlayChargerConnectedTone();
void buzPlayChargerDisconnectedTone();
void playInit();
void buttonBeep();
void buttonLongBeep();


#include <ToneESP32.h>
#include <Arduino.h>
#include "DrmPreferences.h"
#include "ModeOff.h"
#include "Led.h"


ToneESP32 buzzer(BUZZER_PIN, /*BUZZER_CHANNEL*/0);


void buzzerInit(){
  if(getMuteEnabled()) return;
  if(esp_sleep_get_wakeup_cause() == 0 && !isBatteryCritical() && !isOff()){ //reboot manually
    buzTone(3000, 50);
    buzTone(2800, 50);
  }
}
void buzTone(int freq){
  float coef = getSoundCoef();
  freq *= coef;
  buzzer.tone(freq);
}
void buzTone(int freq, int duration){
  float coef = getSoundCoef();
  freq *= coef;
  buzzer.tone(freq, duration);
}
void buzNoTone(){
  buzzer.noTone(); 
}

void buzPlayChargerConnectedTone(){
  if(getMuteEnabled()) return;
  if(isOff())
    return;
  for(int i=0; i<10; i++){
    buzTone(2000+i*20, 10);
    delay(5);
  }
}
void buzPlayChargerDisconnectedTone(){
  if(getMuteEnabled()) return;
  if(isOff())
    return;
  for(int i=0; i<10; i++){
    buzTone(2000-i*20, 10);
    delay(5);
  }
}

int buttonBeepCnt(){return 7;}
void buttonBeep(){
  if(getMuteEnabled()){
    buzNoTone();
    return;
  } 
  if(!isOff()){
    int sound = getButtonSound();
    if(sound == 0){
      delay(10);//no sound
      buzNoTone();
    }
    if(sound == 1){
      buzTone(2000, 50);
    }
    if(sound == 2){
      buzTone(3000, 40);
    }
    if(sound == 3){
      buzTone(2000, 15);
      buzTone(3000, 40);
    }
    if(sound == 4){
      buzTone(2000, 5);
      buzTone(1500, 5);
    }
    if(sound == 5){
      buzTone(4100, 10);
    }
    if(sound == 6){
      buzTone(1000, 70);
    }
  }
    
}
void buttonLongBeep(){
  if(getMuteEnabled()) return;
  if(!isOff())
    buzTone(2000, 150);
}



void playInit(){
  if(getMuteEnabled()) return;
  for(byte i=2; i<5; i++){
    buzTone(i*1000, 100);
    buzNoTone(); 
    delay(100);
  }
  for(byte i=0; i<6; i++){
    ledFlashlightOnTop();
    buzTone(5000, 30);  
    ledFlashlightOffTop();
    buzNoTone(); 
    delay(30);
  }
}





  


#endif