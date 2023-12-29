const int longClickDelay = 700;//ms 
const int firstClickDelay = 700;//ms 
const int nextClickDelay = 150;//ms 


unsigned long lastActionTime = 0;
bool buttonReady[3] = {false,false,false}; //size need to fit max pin number used as button!!   

  
void initButtons(){
  pinMode(BUT_UP, INPUT_PULLUP);
  pinMode(BUT_CENTER, INPUT_PULLUP);
  pinMode(BUT_DOWN, INPUT_PULLUP);
  lastActionTime = millis();
}


void buttonsLoop(){
  processButton(BUT_UP, modeButtonUp, modeButtonUpLong);
  processButton(BUT_CENTER, modeButtonCenter, modeButtonCenterLong);
  processButton(BUT_DOWN, modeButtonDown, modeButtonDownLong);
}


bool isPressed(gpio_num_t pin){
  for(int i=0; i<5; i++)
    if(digitalRead(pin) == HIGH) //if not pressed
      return false;
  return true;
}

void registerAction(){
  lastActionTime=millis();
}

unsigned long sinceLastAction(){
  return millis() - lastActionTime;
}


//----------------------//----------------------//----------------------//----------------------//----------------------//----------------------//----------------------


void processButton(gpio_num_t pin, Runnable onPressed, Runnable onLongPressed){
  if(!isPressed(pin))
    buttonReady[pin] = true;
  if(buttonReady[pin] && isPressed(pin)){
    for(unsigned long pressStarted = millis(); isPressed(pin);){  
      if(onPressed != 0 && lastActionTime < pressStarted){//first click
        lastActionTime = millis();
        buttonBeep();
        onPressed();
        //if(modeLoop != 0) modeLoop();
      }
      else if(onPressed != 0 && onLongPressed == 0 && lastActionTime >= pressStarted && millis()-pressStarted>firstClickDelay && millis()-lastActionTime>nextClickDelay){//next clicks while user holding button
        lastActionTime = millis();
        buttonBeep();
        onPressed();
        //if(modeLoop != 0) modeLoop();
      }
      else if(onLongPressed != 0 && millis()-pressStarted>firstClickDelay){
        lastActionTime = millis();
        buttonLongBeep();
        onLongPressed();
        //if(modeLoop != 0) modeLoop();
        break;
      }
    }
    buttonReady[pin] = false;
  }
}



