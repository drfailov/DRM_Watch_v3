RTC_DATA_ATTR bool deviceOff = false;

void setModeOff(){
  drawMessage("Вимкнення...");
  delay(500);
  deviceOff = true;
  lcdPowerOff();  
  modeSetup = setModeOff;
  modeLoop = modeOffLoop;
  modeButtonUp = 0;
  modeButtonCenter = 0;
  modeButtonDown = 0;
  modeButtonUpLong = 0;
  modeButtonCenterLong = turnOn;
  modeButtonDownLong = 0;
  registerAction();
  enableAutoReturn = false;
}
void modeOffLoop(){
  lcdPowerOff();  
  if(esp_sleep_get_wakeup_cause() == ESP_SLEEP_WAKEUP_EXT0){/*By button*/
    if(sinceLastAction() > 5000) //auto go to watchface
      goToSleep();
  }else{
    goToSleep();
  }
}

bool isOff(){
  return modeSetup == setModeOff;// deviceOff;
}

void turnOn(){
  deviceOff = false;
  ESP.restart();
}