const int ModeMenuSettingsSoundItemBack = 0;
const int itemModeSettingsButtonSound=1;
const int itemModeSettingsMute=2;

void setModeMenuSettingsSound(){
  clearScreenAnimation();
  Serial.println(F("Set mode: ModeMenuSettingsSound"));
  modeSetup = setModeMenuSettingsSound;
  modeLoop = ModeMenuSettingsSoundLoop;
  modeButtonUp = modeMainMenuButtonUp;
  modeButtonCenter = ModeMenuSettingsSoundButtonCenter;
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
  items = 3;
}


void ModeMenuSettingsSoundLoop(){
  lcd()->setColorIndex(white);
  lcd()->drawBox(0, 0, 400, 240);


  lcd()->setFont(u8g2_font_10x20_t_cyrillic);  //ok
  lcd()->setColorIndex(black);
  lcd()->setCursor(5, 18); 
  lcd()->print("Параметри звуку");

  drawMenuLegend();
  drawStatusbar(363, 1, true);

  drawListItem(ModeMenuSettingsSoundItemBack, draw_ic24_back, "Повернутись", "В меню налаштувань", firstDraw); //
  drawListItem(itemModeSettingsButtonSound, draw_ic24_buttonsound, "Звук кнопок", "Обрати звук кнопок", firstDraw);
  if(getMuteEnabled()) drawListItem(itemModeSettingsMute, draw_ic24_sound_mute, "Тихий режим", "Зараз звук вимкнено", firstDraw);
  else drawListItem(itemModeSettingsMute, draw_ic24_sound_on, "Тихий режим", "Зараз звук увімкнено", firstDraw);

  lcd()->sendBuffer();
}

void ModeMenuSettingsSoundButtonCenter(){
  if(selected == ModeMenuSettingsSoundItemBack){
      setModeSettingsMenu();
  }
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





