#include <ToneESP32.h>


ToneESP32 buzzer(BUZZER_PIN, /*BUZZER_CHANNEL*/0);


void buzzerInit(){
  if(getMuteEnabled()) return;
  if(esp_sleep_get_wakeup_cause() == 0 && !isBatteryCritical()){ //reboot manually
    buzzer.tone(3000, 50);
    buzzer.tone(2800, 50);
  }
}
int freq = 0;
void buzTone(int _freq){
  freq=_freq;
  buzzer.tone(freq);
}
void buzDownFreq(){
  freq --;
  buzTone(freq);
}
int buzGetFreq(){
  return freq;
}
void buzTone(int _freq, int duration){
  freq=_freq;
  buzzer.tone(freq, duration);
}
void buzNoTone(){
  buzzer.noTone(); 
  freq = 0;
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

int buttonBeepCnt(){return 5;}
void buttonBeep(){
  if(getMuteEnabled()){
    buzNoTone();
    return;
  } 
  if(!isOff()){
    int sound = getButtonSound();
    if(sound == 0){
      delay(40);//no sound
    }
    if(sound == 1){
      buzzer.tone(2000, 50);
    }
    if(sound == 2){
      buzzer.tone(3000, 40);
    }
    if(sound == 3){
      buzzer.tone(2000, 15);
      buzzer.tone(3000, 40);
    }
    if(sound == 4){
      buzzer.tone(2000, 5);
      buzzer.tone(1500, 5);
    }
  }
    
}
void buttonLongBeep(){
  if(getMuteEnabled()) return;
  if(!isOff())
    buzzer.tone(2000, 150);
}



void playInit(){
  if(getMuteEnabled()) return;
  for(byte i=2; i<5; i++){
    buzzer.tone(i*1000, 100);
    buzzer.noTone(); 
    delay(100);
  }
  for(byte i=0; i<6; i++){
    ledFlashlightOnTop();
    buzzer.tone(5000, 30);  
    ledFlashlightOffTop();
    buzzer.noTone(); 
    delay(30);
  }
}





  