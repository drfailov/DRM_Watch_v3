
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
  unsigned long timerTime = getTimerTime();


  lcd()->setColorIndex(black);
  lcd()->setFont(u8g2_font_logisoso38_tn); //u8g2_font_inr24_t_cyrillic
  lcd()->setCursor(90, 130);
  displayPrintSecondsAsTime(timerTime);
  
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
    timerTime += 60;
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
    if(timerTime > 60)
      timerTime -= 60;
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
