
void setModeWatchface(){
  Serial.println(F("Set mode: Watchface"));
  modeSetup = setModeWatchface;
  modeLoop = modeWatchfaceLoop;
  modeButtonUp = modeWatchfaceButtonUp;
  modeButtonCenter = modeWatchfaceButtonCenter;
  modeButtonDown = modeWatchfaceButtonDown;
  //modeButtonUpLong = modeWatchfaceButtonUp;
  modeButtonCenterLong = ledFlashlightToggleTop;
  //modeButtonDownLong = modeWatchfaceButtonUp;
  registerAction();
}

void modeWatchfaceLoop(){ 
  //Serial.println("clear screen...");
  lcd()->setColorIndex(white);
  lcd()->drawBox(0, 0, 400, 240);
  
  
  //Serial.println("draw time...");
  lcd()->setColorIndex(black);
  lcd()->setFont(u8g2_font_logisoso92_tn);
  lcd()->drawStr(60, 160, rtc()->getTime("%H:%M").c_str());

  //Serial.println("draw date...");
  lcd()->setFont(u8g2_font_10x20_t_cyrillic);
  lcd()->setColorIndex(black);
  lcd()->drawStr(5, 25, rtc()->getTime("%d %b %Y").c_str());
  
  //Serial.println("draw temperature...");
  drawTemperature(5, 213);

  //Serial.println("draw battery...");
  drawBattery(371, 214);
  
  //Serial.println("draw legend...");
  draw_ic24_flashlight(lx(), ly2(), black);
  draw_ic24_menu(lx(), ly3(), black);


  if(esp_sleep_get_wakeup_cause() != ESP_SLEEP_WAKEUP_TIMER) //if wake by timer, don't refresh display to keep image static
    lcd()->sendBuffer();

  if(sinceLastAction() > 15000) //auto go to sleep
    goToSleep();
}

void modeWatchfaceButtonUp(){
  
}

void modeWatchfaceButtonCenter(){
  //shortBeep();
}

void modeWatchfaceButtonDown(){
  //shortBeep();
  //setModeTest();
  setModeMainMenu();
}