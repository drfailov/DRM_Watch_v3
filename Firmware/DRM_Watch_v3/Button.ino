const int longClickDelay = 700;//ms 
const int firstClickDelay = 500;//ms 
const int nextClickDelay = 60;//ms 
//const Runnable buttonQueue[3] = {0,0,0};  // 0 is next to execute

unsigned long lastActionTime = 0;
hw_timer_t *Button_Timer_Cfg;

  

void IRAM_ATTR Buttons_ISR()
{
  checkReady(BUT_UP);
  checkReady(BUT_CENTER);
  checkReady(BUT_DOWN);
}

void initButtons(){
  pinMode(BUT_UP, INPUT_PULLUP);
  pinMode(BUT_CENTER, INPUT_PULLUP);
  pinMode(BUT_DOWN, INPUT_PULLUP);
  lastActionTime = millis();

  Button_Timer_Cfg = timerBegin(1, 80, true);  //// Timer1, делитель 80 (для 1MHz тактової частоти), лічильник вгору
  timerAttachInterrupt(Button_Timer_Cfg, &Buttons_ISR, true);
  timerAlarmWrite(Button_Timer_Cfg, 3000, true);  //  // Встановлення періоду таймера в мікросекундах
  timerAlarmEnable(Button_Timer_Cfg);
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

bool buttonReady[3] = {false,false,false}; //size need to fit max pin number used as button!!   
#define doClick() {registerAction();  buttonBeep();      if((*onPressed) != 0)(*onPressed)();          if(modeLoop != 0) modeLoop();  firstDraw = false;  Serial.println("CLICK");}
#define doLong()  {registerAction();  buttonLongBeep();  if((*onLongPressed) != 0)(*onLongPressed)();  if(modeLoop != 0) modeLoop();  firstDraw = false;  Serial.println("LONG"); }
#define longDefined ((*onLongPressed)!=0)

void checkReady(gpio_num_t pin){
  if(!isPressed(pin))
    buttonReady[pin] = true;
}

void processButton(gpio_num_t pin, Runnable *onPressed, Runnable *onLongPressed){
  checkReady(pin);
  if((*onPressed) == 0 && (*onLongPressed) == 0)
    return;
  if(!buttonReady[pin] || !isPressed(pin))
    return;
  unsigned long pressStarted = millis();
  unsigned long lastAction = 0;
  
  if(!longDefined){ //process pattern for multiple presses at once
    doClick();  //first click
    while(isPressed(pin)){
      if(lastAction==0 && millis()-pressStarted>firstClickDelay && !longDefined){  //user pressed button and hold it long
        doClick();
        lastAction=millis();
      }
      else if(lastAction!=0 && millis()-lastAction>nextClickDelay && !longDefined){  //user pressed button and hold it more long
        doClick();
        lastAction=millis();
      }
    }
  }
  else if(longDefined){   //process pattern for long press button
    while(true){
      if(isPressed(pin)){
        if(lastAction==0 && millis()-pressStarted>longClickDelay){  //user pressed button and hold it long
          doLong();
          lastAction=millis();
        }
      }
      else{
        if(lastAction==0)
          doClick();
        break;
      }
    }
  }
  buttonReady[pin] = false;
}



