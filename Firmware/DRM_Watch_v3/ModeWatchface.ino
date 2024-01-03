
void setModeWatchface(){
  Serial.println(F("Set mode: Watchface"));
  modeSetup = setModeWatchface;
  modeLoop = modeWatchfaceLoop;
  modeButtonUp = modeWatchfaceButtonUp;
  modeButtonCenter = modeWatchfaceButtonCenter;
  modeButtonDown = modeWatchfaceButtonDown;
  modeButtonUpLong = switchDontSleep;
  modeButtonCenterLong = switchDontSleep;
  modeButtonDownLong = 0;
  registerAction();
}

void modeWatchfaceLoop(){ 
  //Serial.println("clear screen...");
  lcd()->setColorIndex(white);
  lcd()->drawBox(0, 0, 400, 240);
  
  
  //Serial.println("draw time...");
  lcd()->setColorIndex(black);
  lcd()->setFont(u8g2_font_logisoso92_tn);
  lcd()->drawStr(60, 160, rtcCorrected()->getTime("%H:%M").c_str());

  //Serial.println("draw date...");
  lcd()->setFont(u8g2_font_10x20_t_cyrillic);
  lcd()->setColorIndex(black);
  lcd()->drawStr(5, 25, rtcCorrected()->getTime("%d %b %Y").c_str());
  
  //Serial.println("draw temperature...");
  drawTemperature(5, 213);

  drawStatusbar(395, 214, false);
  
  //Serial.println("draw legend...");
  draw_ic16_flashlight(lx(), ly1(), black);
  draw_ic16_flashlight(lx(), ly2(), black);
  draw_ic16_menu(lx(), ly3(), black);


  if(esp_sleep_get_wakeup_cause() != ESP_SLEEP_WAKEUP_TIMER) //if wake by timer, don't refresh display to keep image static
    lcd()->sendBuffer();

  if(!dontSleep){
    int sleepTimeout = isFlashlightOn()?autoSleepTimeFlashlightOn:autoSleepTime;
    if(sinceLastAction() > sleepTimeout) //auto go to sleep
      goToSleep();
  }
}

void modeWatchfaceButtonUp(){
  ledFlashlightToggleTop();
}

void modeWatchfaceButtonCenter(){
  ledFlashlightToggleBottom();
}

void modeWatchfaceButtonDown(){
  setModeMainMenu();
}

void switchDontSleep(){
  dontSleep=!dontSleep;
  if(dontSleep)
    buttonLongBeep();
}

void drawStatusbar(int x, int y, bool drawTime){
  int interval = 5;
  if(drawTime){
    lcd()->setColorIndex(black);
    lcd()->setFont(u8g2_font_10x20_t_cyrillic);
    String time = rtcCorrected()->getTime("%H:%M");
    int width = lcd()->getStrWidth(time.c_str());
    x-=width;
    lcd()->setCursor(x, y+18); 
    lcd()->print(time);
    x-=interval;
  }
  //Serial.println("draw battery...");
  {
    x -= 24;
    drawBattery(x, y);
    x-=interval;
  }
  if(isFlashlightOn()){
    x -= 16;
    draw_ic16_flashlight(x, y+4, black);
    x-=interval;
  }
  if(dontSleep){
    x -= 16;
    draw_ic16_coffee(x, y+4, black);
    x-=interval;
  }
}