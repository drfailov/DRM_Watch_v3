#include "ToneESP32.h"

ToneESP32::ToneESP32(int pin, int channel) {	
  this->pin = pin;
  this->channel = channel;  
  ledcAttachPin( pin, channel );
}



void ToneESP32::tone(int note, int duree ) {     
	ledcSetup( channel, note, PWM_Res );
	ledcWrite( channel, 127 );                            
    delay(duree);
    ledcWrite( channel, 0 );
}


void ToneESP32::noTone() {
    ledcWrite(channel, 0);
}
