const int itemModeStopwatchBack=0;
const int itemModeStopwatchStartStop=1;
const int itemModeStopwatchReset=2;

void setModeStopwatch(){
  clearScreenAnimation();
  Serial.println(F("Set mode: Stopwatch"));
  modeSetup = setModeStopwatch;
  modeLoop = modeStopwatchLoop;
  modeButtonUp = modeMainMenuButtonUp;
  modeButtonCenter = modeStopwatchButtonCenter;
  modeButtonDown = modeMainMenuButtonDown;
  modeButtonUpLong = 0;
  modeButtonCenterLong = 0;
  modeButtonDownLong = 0;
  registerAction();
  enableAutoReturn = false;
  enableAutoSleep = true; 
  autoReturnTime = autoReturnDefaultTime;
  autoSleepTime = autoReturnDefaultTime;
  selected = 1;
  items = 3;
}


void modeStopwatchLoop(){ 
  lcd()->setColorIndex(white);
  lcd()->drawBox(0, 0, 400, 240);
  
  lcd()->setFont(u8g2_font_10x20_t_cyrillic);  //ok
  lcd()->setColorIndex(black);
  lcd()->setCursor(5, 18); 
  lcd()->print("Секундомір");
  drawStatusbar(363, 1, true);  

  drawMenuLegend();
  
  unsigned long now = rtcGetEpoch();
  unsigned long start = getStopwatchStartedTime();
  unsigned long end = getStopwatchFinishedTime();
  if(start != 0 && end == 0) end = now;
  unsigned long dd = end-start;

  
  lcd()->setColorIndex(black);
  lcd()->setFont(u8g2_font_inr24_t_cyrillic);
  lcd()->setCursor(90, 90);
  displayPrintSecondsAsTime(dd);


  
  drawMenuItem(itemModeAppsBack, draw_ic24_back, "Назад", firstDraw, 70, 130);
  if(isStopwatchRunning())
    drawMenuItem(itemModeStopwatchStartStop, draw_ic24_pause, "Пауза", firstDraw, 140, 130);
  else
    drawMenuItem(itemModeStopwatchStartStop, draw_ic24_arrow_right, "Запустити", firstDraw, 140, 130);
  drawMenuItem(itemModeStopwatchReset, draw_ic24_reboot, "Скинути", firstDraw, 210, 130);

  lcd()->sendBuffer();
}

void modeStopwatchButtonCenter(){
  if(selected == itemModeStopwatchBack){
    setModeAppsMenu();
  }
  if(selected == itemModeStopwatchStartStop){
    if(isStopwatchRunning()){ //do PAUSE
      saveStopwatchFinishedTime(rtcGetEpoch());
    }
    else{ //do RESUME
      unsigned long now = rtcGetEpoch();
      unsigned long stopwatchStarted = getStopwatchStartedTime();
      unsigned long stopwatchFinished = getStopwatchFinishedTime();
      unsigned long dt = stopwatchFinished - stopwatchStarted;
      unsigned long newStartedTime = now - dt;
      saveStopwatchStartedTime(newStartedTime);
      saveStopwatchFinishedTime(0);
    }
  }
  if(selected == itemModeStopwatchReset){
    if(isStopwatchRunning()){
      unsigned long now = rtcGetEpoch();
      saveStopwatchStartedTime(now);
    }
    else{
      saveStopwatchStartedTime(0);
      saveStopwatchFinishedTime(0);
    }
  }  
}
