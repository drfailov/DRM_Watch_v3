

void setModeTest(){
  clearScreenAnimation();
  Serial.println(F("Set mode: Test"));
  modeSetup = setModeTest;
  modeLoop = modeTestLoop;
  modeButtonUp = modeTestButtonUp;
  modeButtonCenter = modeTestButtonCenter;
  modeButtonDown = modeTestButtonDown;
  modeButtonUpLong = 0;
  modeButtonCenterLong = 0;
  modeButtonDownLong = 0;
  registerAction();
  enableAutoReturn = false;
  enableAutoSleep = true; 
  autoReturnTime = autoReturnDefaultTime;
  autoSleepTime = autoSleepDefaultTime;
}

void modeTestLoop(){ 
  lcd()->setColorIndex(white);
  lcd()->drawBox(0, 0, 400, 240);
  
  lcd()->setColorIndex(black);
  lcd()->setFont(u8g2_font_unifont_t_cyrillic); //smalll

  int y=2;
  int x=5;
  int interval = 13;

  y+=interval; lcd()->setCursor(x, y); lcd()->print("Internal RTC: "); lcd()->print(_rtcInternal()->getTime("%d %b %Y %H:%M:%S"));  lcd()->print(" ("); lcd()->print(_rtcInternal()->getEpoch());   lcd()->print(")");
  y+=interval; lcd()->setCursor(x, y); lcd()->print("Corr Int RTC: "); lcd()->print(_rtcInternalCorrected()->getTime("%d %b %Y %H:%M:%S"));  lcd()->print("  ("); lcd()->print(_rtcInternalCorrected()->getEpoch()); lcd()->print(")");
  y+=interval; lcd()->setCursor(x, y); lcd()->print("Correcton: "); lcd()->print(getTimeCoef()*(rtcGetUtcEpoch()-getLastTimeSync())); lcd()->print("s,  Time coef: "); lcd()->print(String(getTimeCoef(), 8));   
  y+=interval; lcd()->setCursor(x, y); lcd()->print("External RTC: "); printRtcGetTimeRaw();   
  y+=interval; lcd()->setCursor(x, y); lcd()->print("SinceLastSync:"); displayPrintSecondsAsTime(rtcGetUtcEpoch()-getLastTimeSync());  lcd()->print(" Last sync:"); lcd()->print(getLastTimeSync());   
  y+=interval; lcd()->setCursor(x, y); lcd()->print("RTC CLK SRC: "); lcd()->print(getRtcSrc());

  y+=interval; lcd()->setCursor(x, y); lcd()->print("Wakeup_reason: "); wakeup_reason();
  y+=interval; lcd()->setCursor(x, y); lcd()->print("Millis:"); lcd()->print(millis()); lcd()->print(",   Don't sleep: "); lcd()->print(dontSleep);

  y+=interval; lcd()->setCursor(x, y); lcd()->print("RAW ");  lcd()->print("BATTERY: "); lcd()->print(readSensBatteryRaw()); lcd()->print(", USB:"); lcd()->print(readSensUsbRaw()); lcd()->print(" : "); lcd()->print(isChargerConnected());
  y+=interval; lcd()->setCursor(x, y); lcd()->print("VOLTAGE ");  lcd()->print("BATTERY: "); lcd()->print(readSensBatteryVoltage()); lcd()->print(" ("); lcd()->print(batteryBars()); lcd()->print(" bars)"); 
  y+=interval; lcd()->setCursor(x, y); lcd()->print("SinceLastCharged: "); displayPrintSecondsAsTime(getTimeSinceLastCharged());

  y+=interval; lcd()->setCursor(x, y); lcd()->print("BUTTONS "); lcd()->print("TOP:");lcd()->print(isPressed(BUT_UP)); lcd()->print(" CENTER:");lcd()->print(isPressed(BUT_CENTER)); lcd()->print(" BOTTOM:");lcd()->print(isPressed(BUT_DOWN));
  y+=interval; lcd()->setCursor(x, y); lcd()->print("Since last action: "); lcd()->print(sinceLastAction());

  y+=interval; lcd()->setCursor(x, y); lcd()->print("Temperature: "); lcd()->print(temperature());
  y+=interval; lcd()->setCursor(x, y); lcd()->print("rtcChipTemperature: "); lcd()->print(rtcChipTemperature());

  y+=interval; lcd()->setCursor(x, y); lcd()->print("Preferences remaining memory: "); lcd()->print(getPreferencesFreeSpace());
  y+=interval; lcd()->setCursor(x, y); lcd()->print("RAM State: "); lcd()->print(esp_get_free_heap_size());  lcd()->print(",  ");  lcd()->print(ESP.getFreeHeap()); //RAM diagnosis

  y+=interval; lcd()->setCursor(x, y); lcd()->print("SPI Clock: "); lcd()->print(lcd()->getBusClock());
  //uint32_t getBusClock(void);
  //void setBusClock(uint32_t clock_speed);
  
  //draw_ic24_lock(lx(), ly1(), black);
  draw_ic16_back(lx(), ly2(), black);

if(esp_sleep_get_wakeup_cause() != ESP_SLEEP_WAKEUP_TIMER) //if wake by timer, don't refresh display to keep image static, image will refresh when go to lock screen and drawing lock icon
    lcd()->sendBuffer();
}

void modeTestButtonUp(){
  //goToSleep();
}

void modeTestButtonCenter(){
  setModeAppsMenu();
}

void modeTestButtonDown(){
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