

void setModeTest(){
  Serial.println(F("Set mode: Test"));
  modeSetup = setModeTest;
  modeLoop = modeTestLoop;
  modeButtonUp = modeTestButtonUp;
  modeButtonCenter = modeTestButtonCenter;
  modeButtonDown = modeTestButtonDown;
  //modeButtonUpLong = modeWatchfaceButtonUp;
  //modeButtonCenterLong = modeWatchfaceButtonUp;
  //modeButtonDownLong = modeWatchfaceButtonUp;
}

void modeTestLoop(){ 
  lcd()->setColorIndex(white);
  lcd()->drawBox(0, 0, 400, 240);
  
  lcd()->setColorIndex(black);
  lcd()->setFont(u8g2_font_10x20_tf);

  int y=0;
  int interval = 16;

  y+=interval; lcd()->drawStr(10, y, rtc()->getTime("%B %d %Y %H:%M:%S").c_str());
  y+=interval; lcd()->setCursor(10, y); lcd()->print("Millis:"); lcd()->print(millis());
  y+=interval; lcd()->setCursor(10, y); lcd()->print("RAW ");  lcd()->print("BATTERY: "); lcd()->print(readSensBatteryRaw()); lcd()->print(", USB:"); lcd()->print(readSensUsbRaw());
  y+=interval; lcd()->setCursor(10, y); lcd()->print("VOLTAGE ");  lcd()->print("BATTERY: "); lcd()->print(readSensBatteryVoltage()); lcd()->print(" ("); lcd()->print(batteryBars()); lcd()->print(" bars)"); 
  y+=interval; lcd()->setCursor(10, y); lcd()->print("BUTTONS "); lcd()->print("TOP:");lcd()->print(isPressed(BUT_UP)); lcd()->print(" CENTER:");lcd()->print(isPressed(BUT_CENTER)); lcd()->print(" BOTTOM:");lcd()->print(isPressed(BUT_DOWN));
  y+=interval; lcd()->setCursor(10, y); wakeup_reason();
  y+=interval; lcd()->setCursor(10, y); lcd()->print("Since last action:"); lcd()->print(sinceLastAction());
  y+=interval; lcd()->setCursor(10, y); lcd()->print("Temperature:"); lcd()->print(temperature());
  
  
  draw_ic24_lock(lx(), ly1());
  draw_ic24_menu(lx(), ly3());

  lcd()->sendBuffer();

  if(sinceLastAction() > 90000)
    goToSleep();
}

void modeTestButtonUp(){
  goToSleep();
}

void modeTestButtonCenter(){
}

void modeTestButtonDown(){
  setModeWatchface();
  //rtc()->setTime(/*s*/0, /*m*/9, /*h*/0,   /*d*/9, /*M*/10, /*y*/2023);  // 17th Jan 2021 15:24:30
  //playMelody();
}






//----------------------//----------------------//----------------------//----------------------//----------------------//----------------------//----------------------

void wakeup_reason() {
  /*
  0 = first start
  ESP_SLEEP_WAKEUP_TIMER = Wakeup periodically 

  */
  esp_sleep_wakeup_cause_t wakeup_reason = esp_sleep_get_wakeup_cause();
  switch (wakeup_reason)
  {
    case ESP_SLEEP_WAKEUP_EXT0 : lcd()->print("Wakeup external signal RTC_IO"); break;
    case ESP_SLEEP_WAKEUP_EXT1 : lcd()->print("Wakeup external signal RTC_CNTL"); break;
    case ESP_SLEEP_WAKEUP_TIMER : lcd()->print("Wakeup caused by timer"); break;
    case ESP_SLEEP_WAKEUP_TOUCHPAD : lcd()->print("Wakeup caused by touchpad"); break;
    case ESP_SLEEP_WAKEUP_ULP : lcd()->print("Wakeup caused by ULP program"); break;
    default :
      lcd()->print("not deep sleep:");
      lcd()->print(wakeup_reason);
      //rtc.setTime(30, 24, 15, 17, 1, 2021);  // 17th Jan 2021 15:24:30
      //rtc.setTime(1609459200);  // 1st Jan 2021 00:00:00
      //rtc.offset = 7200; // change offset value

      break;
  }
}