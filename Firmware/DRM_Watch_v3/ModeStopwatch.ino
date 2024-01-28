void setModeStopwatch(){
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
}


void modeStopwatchLoop(){ 
  lcd()->setColorIndex(white);
  lcd()->drawBox(0, 0, 400, 240);



  lcd()->setFont(u8g2_font_10x20_t_cyrillic);  //ok
  lcd()->setColorIndex(black);
  lcd()->setCursor(5, 18); 
  lcd()->print("Секундомір");

  drawStatusbar(363, 1, true);  

  lcd()->setColorIndex(black);
  lcd()->setFont(u8g2_font_inr24_t_cyrillic);
  lcd()->setCursor(50, 60);
  lcd()->print("00:00:00"); 

  lcd()->setColorIndex(black);
  lcd()->drawLine(369, 0, 369, 260);
  lcd()->drawLine(370, 0, 370, 260);
  if(isStopwatchRunning())
    draw_ic16_cancel(lx(), ly1(), black);
  else
    draw_ic16_arrow_right(lx(), ly1(), black);
  draw_ic16_cancel(lx(), ly2(), black);
  draw_ic16_repeat(lx(), ly3(), black);

  lcd()->sendBuffer();
}

void modeStopwatchButtonUp(){
  if(isStopwatchRunning()){
    saveStopwatchFinishedTime(rtcGetEpoch());
  }
  else{
    saveStopwatchStartedTime(rtcGetEpoch());
    saveStopwatchFinishedTime(0);
  }
}

void modeStopwatchButtonCenter(){
  setModeAppsMenu();
}

void modeStopwatchButtonDown(){
}