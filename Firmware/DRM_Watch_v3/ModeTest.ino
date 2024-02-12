

void setModeTest(){
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
  lcd()->setFont(u8g2_font_unifont_t_cyrillic);

  int y=2;
  int x=5;
  int interval = 13;

  y+=interval; lcd()->setCursor(x, y); lcd()->print("Meas: "); lcd()->print(_rtcInternal()->getTime("%d %b %Y %H:%M:%S"));  lcd()->print("  Epoch:"); lcd()->print(_rtcInternal()->getEpoch());
  y+=interval; lcd()->setCursor(x, y); lcd()->print("Corr: "); lcd()->print(_rtcInternalCorrected()->getTime("%d %b %Y %H:%M:%S"));  lcd()->print("  Epoch:"); lcd()->print(_rtcInternalCorrected()->getEpoch());
  y+=interval; lcd()->setCursor(x, y); lcd()->print("SinceLastSync:"); displayPrintSecondsAsTime(rtcGetEpoch()-getLastTimeSync());  lcd()->print(" Last sync:"); lcd()->print(getLastTimeSync());  
  y+=interval; lcd()->setCursor(x, y); lcd()->print("Correcton: "); lcd()->print(getTimeCoef()*(rtcGetEpoch()-getLastTimeSync())); lcd()->print("s,  Time coef: "); lcd()->print(String(getTimeCoef(), 8));   
  y+=interval; lcd()->setCursor(x, y); lcd()->print("RTC CLK SRC: "); lcd()->print(getRtcSrc());
  y+=interval; lcd()->setCursor(x, y); lcd()->print("Wakeup_reason: "); wakeup_reason();
  y+=interval; lcd()->setCursor(x, y); lcd()->print("Millis:"); lcd()->print(millis()); lcd()->print(",   Don't sleep: "); lcd()->print(dontSleep);
  y+=interval; lcd()->setCursor(x, y); lcd()->print("RAW ");  lcd()->print("BATTERY: "); lcd()->print(readSensBatteryRaw()); lcd()->print(", USB:"); lcd()->print(readSensUsbRaw()); lcd()->print(" : "); lcd()->print(isChargerConnected());
  y+=interval; lcd()->setCursor(x, y); lcd()->print("VOLTAGE ");  lcd()->print("BATTERY: "); lcd()->print(readSensBatteryVoltage()); lcd()->print(" ("); lcd()->print(batteryBars()); lcd()->print(" bars)"); 
  y+=interval; lcd()->setCursor(x, y); lcd()->print("BUTTONS "); lcd()->print("TOP:");lcd()->print(isPressed(BUT_UP)); lcd()->print(" CENTER:");lcd()->print(isPressed(BUT_CENTER)); lcd()->print(" BOTTOM:");lcd()->print(isPressed(BUT_DOWN));
  y+=interval; lcd()->setCursor(x, y); lcd()->print("Since last action: "); lcd()->print(sinceLastAction());
  y+=interval; lcd()->setCursor(x, y); lcd()->print("Temperature: "); lcd()->print(temperature());
  y+=interval; lcd()->setCursor(x, y); lcd()->print("Preferences remaining memory: "); lcd()->print(getPreferencesFreeSpace());
  y+=interval; lcd()->setCursor(x, y); lcd()->print("Touch1: "); lcd()->print(touchRead(TOUCH1_PIN));  lcd()->print(", Touch2:"); lcd()->print(touchRead(TOUCH2_PIN));  lcd()->print(", Touch3:"); lcd()->print(touchRead(TOUCH3_PIN));
  y+=interval; lcd()->setCursor(x, y); lcd()->print("SinceLastCharged: "); displayPrintSecondsAsTime(getTimeSinceLastCharged());
  y+=interval; lcd()->setCursor(x, y); lcd()->print("rtcChipTemperature: "); lcd()->print(rtcChipTemperature());

  
  draw_ic24_lock(lx(), ly1(), black);
  draw_ic24_arrow_left(lx(), ly2(), black);
  //draw_ic24_coffee(lx(), ly3(), black);

  lcd()->sendBuffer();
}

void modeTestButtonUp(){
  goToSleep();
}

void modeTestButtonCenter(){
  setModeMainMenu();
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