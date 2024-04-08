const int longClickDelay = 700;//ms 
const int firstClickDelay = 500;//ms 
const int nextClickDelay = 60;//ms 


unsigned long lastActionTime = 0;
bool buttonReady[3] = {false,false,false}; //size need to fit max pin number used as button!!   

  
void initButtons(){
  pinMode(BUT_UP, INPUT_PULLUP);
  pinMode(BUT_CENTER, INPUT_PULLUP);
  pinMode(BUT_DOWN, INPUT_PULLUP);
  lastActionTime = millis();
}


void buttonsLoop(){
  processButton(BUT_UP, &modeButtonUp, &modeButtonUpLong);
  processButton(BUT_CENTER, &modeButtonCenter, &modeButtonCenterLong);
  processButton(BUT_DOWN, &modeButtonDown, &modeButtonDownLong);
}


bool isPressed(gpio_num_t pin){
  for(int i=0; i<5; i++)
    if(digitalRead(pin) == HIGH) //if not pressed
      return false;
  return true;
}

bool isButtonUpPressed(){
  return isPressed(BUT_UP);
}

bool isButtonCenterPressed(){
  return isPressed(BUT_CENTER);
}

bool isButtonDownPressed(){
  return isPressed(BUT_DOWN);
}

bool isAnyButtonPressed(){
  return isButtonUpPressed() || isButtonCenterPressed() || isButtonDownPressed();
}

void registerAction(){
  lastActionTime=millis();
  if(esp_sleep_get_wakeup_cause() != ESP_SLEEP_WAKEUP_TIMER && !isOff()){/*NOT periodical wakeup*/ //==================================== BACKLIGHT
    backlightOn();
  }
}

int sinceLastAction(){
  return millis() - lastActionTime;
}


//----------------------//----------------------//----------------------//----------------------//----------------------//----------------------//----------------------


void processButton(gpio_num_t pin, Runnable *onPressed, Runnable *onLongPressed){
  if(!isPressed(pin))
    buttonReady[pin] = true;
  if(buttonReady[pin] && isPressed(pin)){
    for(unsigned long pressStarted = millis(); isPressed(pin);){  
      if((*onPressed) != 0 && lastActionTime < pressStarted){//first click
        registerAction();
        buttonBeep();
        (*onPressed)();
        if(modeLoop != 0) modeLoop();
        firstDraw = false;
      }
      else if((*onPressed) != 0 && (*onLongPressed) == 0 && lastActionTime >= pressStarted && millis()-pressStarted>firstClickDelay && millis()-lastActionTime>nextClickDelay){//next clicks while user holding button
        registerAction();
        buttonBeep();
        (*onPressed)();
        if(modeLoop != 0) modeLoop();
        firstDraw = false;
      }
      else if((*onLongPressed) != 0 && millis()-pressStarted>firstClickDelay){  //long click
        registerAction();
        buttonLongBeep();
        (*onLongPressed)();
        if(modeLoop != 0) modeLoop();
        firstDraw = false;
        break;
      }
    }
    buttonReady[pin] = false;
  }
}



