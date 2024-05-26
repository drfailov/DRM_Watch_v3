unsigned long backlightEnabledTime = 0;
unsigned long backlightTimeoutMs = 25000;

void backlightInit(){
  if(isAwake()){/*NOT periodical wakeup*/ //==================================== BACKLIGHT
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