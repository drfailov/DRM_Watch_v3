
void setModeWatchface(){
  Serial.println(F("Set mode: Watchface"));
  modeSetup = setModeWatchface;
  modeLoop = modeWatchfaceLoop;
  modeButtonUp = modeWatchfaceButtonUp;
  modeButtonCenter = modeWatchfaceButtonCenter;
  modeButtonDown = modeWatchfaceButtonDown;
  //modeButtonUpLong = modeWatchfaceButtonUp;
  //modeButtonCenterLong = modeWatchfaceButtonUp;
  //modeButtonDownLong = modeWatchfaceButtonUp;
}

void modeWatchfaceLoop(){ 
  lcd()->setColorIndex(white);
  lcd()->drawBox(0, 0, 400, 240);

  lcd()->setColorIndex(black);
  //lcd()->setFont(u8g2_font_unifont_t_cyrillic);   //small
  //lcd()->setFont(u8g2_font_cu12_t_cyrillic);  //small
  //lcd()->setFont(u8g2_font_inr24_t_cyrillic);  //big
  lcd()->setFont(u8g2_font_10x20_t_cyrillic);  //ok
  //lcd()->setFont(u8g2_font_cyri);
  
  lcd()->setCursor(10, 32); lcd()->print("Температура:"); lcd()->print(temperature());
  
  lcd()->setColorIndex(black);
  lcd()->setFont(u8g2_font_logisoso92_tn);

  

  lcd()->drawStr(60, 160, rtc()->getTime("%H:%M").c_str());


  drawBattery(371, 214);
  draw_ic24_menu(lx(), ly3());

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
  setModeTest();
}