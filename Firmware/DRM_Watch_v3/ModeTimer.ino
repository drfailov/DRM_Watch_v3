#include <Arduino.h>
void modeTimerButtonUp();


void setModeTimer(){
  clearScreenAnimation();
  Serial.println(F("Set mode: timer"));
  modeSetup = setModeTimer;
  modeLoop = modeTimerLoop;
  modeButtonUp = modeTimerButtonUp;
  modeButtonCenter = modeTimerButtonCenter;
  modeButtonDown = modeTimerButtonDown;
  modeButtonUpLong = 0;
  modeButtonCenterLong = 0;
  modeButtonDownLong = 0;
  registerAction();
  enableAutoReturn = true;
  enableAutoSleep = false; 
  autoReturnTime = autoReturnDefaultTime;
  autoSleepTime = autoSleepDefaultTime;
}

 //unsigned long    saveTimerStartedTime  saveTimerTime  getTimerStartedTime  getTimerTime  isTimerRunning
void modeTimerLoop(){
  lcd()->setColorIndex(white);
  lcd()->drawBox(0, 0, 400, 240);

  lcd()->setFont(u8g2_font_10x20_t_cyrillic);  //ok
  lcd()->setColorIndex(black);
  lcd()->setCursor(5, 18); 
  lcd()->print("Таймер");

  drawStatusbar(363, 1, true);
  //drawMenuLegend();
  //draw_ic16_back(lx(), ly2(), black);
  // if(modeSetTimezoneEditMode){
     //draw_ic16_plus(lx(), ly1(), black);
     //draw_ic16_minus(lx(), ly3(), black);
  // }
  
  //drawMenuItem(itemmodeSetTimezoneBack, draw_ic24_back, "Назад", firstDraw, 30, 32);

  //lcd()->setFont(u8g2_font_10x20_t_cyrillic);  //ok
  //lcd()->setColorIndex(black);
  //lcd()->setCursor(30, 113); 
  //lcd()->print("Час");
  
  //long offset = getTimeOffsetSec();
  //long hours = offset/(60*60);
  //offset -= hours*(60*60);
  //long minutes = offset/60;
  //String text = "UTC ";
  //text += hours>=0?"+":"";
  //text += hours;
  //text += ":";
  //text += minutes;
  //drawTextFrame(/*index*/0, /*text*/text.c_str(), /*name*/"Час таймера", /*editMode*/false, /*animate*/firstDraw, /*x*/30, /*y*/95, /*width*/308);
  unsigned long startedTime = getTimerStartedTime();
  unsigned long now = rtcGetEpoch();
  unsigned long timerTime = getTimerTime();
  unsigned long remainingTime = timerTime;
  if(isTimerRunning()){
    remainingTime = startedTime+timerTime-now;
  }


  lcd()->setColorIndex(black);
  lcd()->setFont(u8g2_font_logisoso38_tn); //u8g2_font_inr24_t_cyrillic
  lcd()->setCursor(90, 130);
  displayPrintSecondsAsTime(remainingTime);
  
  lcd()->setColorIndex(black);
  lcd()->drawBox(369, 0, 2, 260);  //draw_ic16_repeat  draw_ic16_arrow_right  draw_ic16_back
  
  
  if(isTimerRunning()){
    draw_ic16_back(lx(), ly2(), black);
    draw_ic16_repeat(lx(), ly3(), black);
  }
  else{
    if(getTimerTime() == 0){
      draw_ic16_back(lx(), ly2(), black);
    }
    else{
      draw_ic16_arrow_right(lx(), ly2(), black);
    }
    draw_ic16_plus(lx(), ly1(), black);
    draw_ic16_minus(lx(), ly3(), black);
  }
  

  
  lcd()->sendBuffer();
}

void modeTimerButtonUp(){
  if(isTimerRunning()){

  }
  else{
    unsigned long timerTime = getTimerTime();
    timerTime += timerStep();
    saveTimerTime(timerTime);
  }
  // if(!modeSetTimezoneEditMode){
  //   modeMainMenuButtonUp();
  //   return;
  // }
  // if(selected == itemmodeSetTimezoneHour) 
  //saveTimeOffsetSec(getTimeOffsetSec()+60*30);
}
void modeTimerButtonDown(){
  if(isTimerRunning()){
    saveTimerStartedTime(0); //reset
    saveTimerTime(0);
  }
  else{
    unsigned long timerTime = getTimerTime();
    if(timerTime > timerStep())
      timerTime -= timerStep();
    else
      timerTime = 0;
    saveTimerTime(timerTime);
  }
  // if(!modeSetTimezoneEditMode){
  //   modeMainMenuButtonDown();
  //   return;
  // }
  // if(selected == itemmodeSetTimezoneHour) 
  ///saveTimeOffsetSec(getTimeOffsetSec()-60*30);
}
void modeTimerButtonCenter(){
  if(isTimerRunning()){
    setModeAppsMenu(); //exit
  }
  else{
    if(getTimerTime() == 0){
      setModeAppsMenu(); //exit
    }
    else{
      saveTimerStartedTime(rtcGetEpoch());//start
    }
  }
  //if(selected == itemmodeSetTimezoneBack){
    
//    return;
  //}
  // if(selected==itemmodeSetTimezoneHour){
  //   modeSetTimezoneEditMode = !modeSetTimezoneEditMode;
  //   return;
  // }
}

void timerLoop(){
  if(isTimerRunning()){
    unsigned long startedTime = getTimerStartedTime();
    unsigned long now = rtcGetEpoch();
    unsigned long timerTime = getTimerTime();
    if(now >= startedTime+timerTime){
      timerAlert();
      saveTimerStartedTime(0); //reset
      saveTimerTime(0);
    }
  }
}
int timerStep(){
  unsigned long timerTime = getTimerTime();
  if(timerTime >= 3*60*60) /*3h*/ return 60*60; /*1h*/
  if(timerTime >= 60*60) /*1h*/ return 30*60; /*30h*/
  if(timerTime >= 30*60) /*30m*/ return 5*60; /*5m*/
  if(timerTime >= 5*60) /*5m*/ return 60; /*1m*/
  return 30; /*30s*/
}

void timerAlert(){
  Serial.println(F("Open: timerAlert"));
  clearScreenAnimation();
  backlightOn();
  unsigned long started = rtcGetEpoch();
  unsigned long timerTime = getTimerTime();
  unsigned long endTime = started+60*2; //2min
  int freq = 1900;
  while(rtcGetEpoch() < endTime){
    
    lcd()->setColorIndex(white);
    lcd()->drawBox(0, 0, 400, 240);

    drawStatusbar(363, 1, true);

    displayDrawVector(getPathZubat(), 130, 45, 3.0, 3, false, black);

    
    lcd()->setCursor(100, 230); 
    lcd()->setFont(u8g2_font_10x20_t_cyrillic);
    lcd()->print("Таймер (");
    displayPrintSecondsAsTime(timerTime);
    lcd()->print(")");


    lcd()->drawBox(369, 0, 2, 260);  //draw_ic16_repeat  draw_ic16_arrow_right  draw_ic16_back
    draw_ic16_back(lx(), ly2(), black);

    lcd()->sendBuffer();


    for(int i=0; i<4; i++){
      buzTone(freq);
      for(long st=millis(); millis()<st+50;)if(isButtonCenterPressed()){buttonBeep(); modeSetup(); return;}
      buzNoTone(); 
      for(long st=millis(); millis()<st+70;)if(isButtonCenterPressed()){buttonBeep(); modeSetup(); return;}
    }
    for(long st=millis(); millis()<st+500;)if(isButtonCenterPressed()){buttonBeep(); modeSetup(); return;}
  }
  buzNoTone(); 
  //backlightOff();
  clearScreenAnimation();
  modeSetup(); 
}
