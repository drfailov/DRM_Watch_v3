const int itemModeSettingsBack=0;
const int itemModeSettingsCategoryTime=1;
const int itemModeSettingsCategoryDisplay=2;
const int itemModeSettingsSavedNetworks=3;
const int itemModeSettingsButtonSound=4;
const int itemModeSettingsMute=5;
const int itemModeSettingsReboot=6;

void setModeSettingsMenu(){
  clearScreenAnimation();
  Serial.println(F("Set mode: Settings Menu"));
  modeSetup = setModeSettingsMenu;
  modeLoop = modeSettingsMenuLoop;
  modeButtonUp = modeMainMenuButtonUp;
  modeButtonCenter = modeSettingsMenuButtonCenter;
  modeButtonDown = modeMainMenuButtonDown;
  modeButtonUpLong = 0;
  modeButtonCenterLong = 0;
  modeButtonDownLong = 0;
  registerAction();
  enableAutoReturn = true;
  enableAutoSleep = false; 
  autoReturnTime = autoReturnDefaultTime;
  autoSleepTime = autoSleepDefaultTime;
  selected = 0;
  items = 7;
}


void modeSettingsMenuLoop(){
  lcd()->setColorIndex(white);
  lcd()->drawBox(0, 0, 400, 240);

  lcd()->setFont(u8g2_font_10x20_t_cyrillic);  //ok
  lcd()->setColorIndex(black);
  lcd()->setCursor(5, 18); 
  lcd()->print("Налаштування");

  drawStatusbar(363, 1, true);  
  drawMenuLegend();

  drawListItem(itemModeSettingsBack, draw_ic24_back, "Назад", "Повернутись до головного меню", firstDraw);
  drawListItem(itemModeSettingsCategoryTime, draw_ic24_clock, "Налаштування часу", "Налаштування що стосуються часу", firstDraw);
  drawListItem(itemModeSettingsCategoryDisplay, draw_ic24_display, "Налаштування дисплея", "Налаштування що стосуються дисплея", firstDraw);
  drawListItem(itemModeSettingsSavedNetworks, draw_ic24_wifi_3, "Список збережениx мереж Wi-Fi", "Мережі для доступу до Інтернет", firstDraw); /*draw_ic24_saved*/
  drawListItem(itemModeSettingsReboot, draw_ic24_reboot, "Перезавантажити", "Перезавантажити годинник", firstDraw);
  if(getMuteEnabled()) drawListItem(itemModeSettingsMute, draw_ic24_sound_mute, "Тихий режим", "Зараз звук вимкнено", firstDraw);
  else drawListItem(itemModeSettingsMute, draw_ic24_sound_on, "Тихий режим", "Зараз звук увімкнено", firstDraw);
  drawListItem(itemModeSettingsButtonSound, draw_ic24_buttonsound, "Звук кнопок", "Обрати звук кнопок", firstDraw);
  

  lcd()->sendBuffer();
}

void modeSettingsMenuButtonCenter(){
  if(selected == itemBack){
    setModeMainMenu();
    return;
  }
  if(selected == itemModeSettingsCategoryTime){
    setModeMenuSettingsTime();
    return;
  }
  if(selected == itemModeSettingsCategoryDisplay){
    setModeMenuSettingsDisplay();
    return;
  }
  if(selected==itemModeSettingsSavedNetworks){
    setModeSavedWiFiList();
    return;
  }
  // if(selected==itemModeSettingsSynchronizeTime){
  //   timeSync();
  //   return;
  // }
  // if(selected==itemModeSettingsSynchronizeTimeEnable){
  //   saveTimeSyncEnabled(!getTimeSyncEnabled());
  //   return;
  // }
  // if(selected==itemModeSettingsSetTime){
  //   drawMessage("В процесі розробки.");
  //   return;
  // }
  // if(selected==itemModeSettingsSetTimeZone){
  //   ();
  //   return;
  // }
  if(selected==itemModeSettingsReboot){
    modeButtonUpLong();
    return;
  }
  // if(selected==itemModeSettingsInvert){
  //   saveInversionValue(!getInversionValue());
  //   black = getBlackValue();
  //   white = getWhiteValue();
  //   return;
  // }
  if(selected==itemModeSettingsMute){
    saveMuteEnabled(!getMuteEnabled());
    return;
  }
  if(selected == itemModeSettingsButtonSound){
    int sound = getButtonSound();
    saveButtonSound(sound+1);
    delay(500);
    buttonBeep();
    return;
  }
  
  
}