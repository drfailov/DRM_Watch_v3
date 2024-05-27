#ifndef _TONE_ESP32_h
#define _TONE_ESP32_h

#include "Arduino.h"
#include "Notes.h"

#define PWM_Res   8

class ToneESP32 { 
  private:
    int pin; 
    int channel;
  public:    
    ToneESP32(int pin, int channel);      
    void tone(int note, int duree);   
    void tone(int note);
    void noTone();    
};

#endif



