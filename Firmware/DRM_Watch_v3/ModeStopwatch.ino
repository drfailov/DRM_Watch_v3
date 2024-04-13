const int itemModeStopwatchBack=0;
const int itemModeStopwatchStartStop=1;
const int itemModeStopwatchReset=2;

unsigned long modeStopwatchStartedMillis = 0;

void setModeStopwatch(){
  clearScreenAnimation();
  Serial.println(F("Set mode: Stopwatch"));
  modeSetup = setModeStopwatch;
  modeLoop = modeStopwatchLoop;
  modeButtonUp = modeStopwatchButtonUp;
  modeButtonCenter = modeStopwatchButtonCenter;
  modeButtonDown = modeStopwatchButtonDown;
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

  //drawMenuLegend();
  
  lcd()->setColorIndex(black);
  lcd()->drawBox(369, 0, 2, 260);  //draw_ic16_repeat  draw_ic16_arrow_right  draw_ic16_back
  if(isStopwatchRunning())
    draw_ic16_pause(lx(), ly1(), black); 
  else
    draw_ic16_arrow_right(lx(), ly1(), black); 
  draw_ic16_back(lx(), ly2(), black);
  draw_ic16_repeat(lx(), ly3(), black);
  
  unsigned long now = rtcGetEpoch();
  unsigned long start = getStopwatchStartedTime();
  unsigned long end = getStopwatchFinishedTime();
  if(start != 0 && end == 0) end = now;
  unsigned long dd = end-start;

  
  lcd()->setColorIndex(black);
  lcd()->setFont(u8g2_font_logisoso38_tn); //u8g2_font_inr24_t_cyrillic
  lcd()->setCursor(90, 100);
  displayPrintSecondsAsTime(dd);
  if(isStopwatchRunning()){
    unsigned long sinceStarted = millis()-modeStopwatchStartedMillis;
    lcd()->setFont(u8g2_font_10x20_t_cyrillic);
    lcd()->print(":");
    lcd()->print(sinceStarted%1000);
  }


  
  //drawMenuItem(itemModeAppsBack, draw_ic24_back, "Назад", firstDraw, 50, 140);

  // if(isStopwatchRunning())
  //   drawMenuItem(itemModeStopwatchStartStop, draw_ic24_pause, "Пауза", firstDraw, 140, 140);
  // else
  //   drawMenuItem(itemModeStopwatchStartStop, draw_ic24_arrow_right, "Запустити", firstDraw, 140, 140);
  // drawMenuItem(itemModeStopwatchReset, draw_ic24_reboot, "Скинути", firstDraw, 230, 140);

  lcd()->sendBuffer();
}

void modeStopwatchButtonCenter(){
  setModeAppsMenu(); 
}

void modeStopwatchButtonUp(){
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
    modeStopwatchStartedMillis = millis();
  }
} 

void modeStopwatchButtonDown(){ 
  if(isStopwatchRunning()){  //do RESET
      unsigned long now = rtcGetEpoch();
      saveStopwatchStartedTime(now);
    }
    else{
      saveStopwatchStartedTime(0);
      saveStopwatchFinishedTime(0);
    }
}
