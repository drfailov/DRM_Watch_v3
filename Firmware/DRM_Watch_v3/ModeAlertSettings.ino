int modeAlertSettingsIndex = 0;
const int itemModeAlertSettingsBack=0;
const int itemModeAlertSettingsEnabled=1;
const int itemModeAlertSettingsHour=2;
const int itemModeAlertSettingsMinute=3;
const int itemModeAlertSettingsMelody=4;

void setModeAlertSettingsMenu(){
  Serial.println(F("Set mode: Alert settings Menu"));
  modeSetup = setModeAlertSettingsMenu;
  modeLoop = ModeAlertSettingsMenuLoop;
  modeButtonUp = modeMainMenuButtonUp;
  modeButtonCenter = ModeAlertSettingsMenuButtonCenter;
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
  items = 5;
}


void ModeAlertSettingsMenuLoop(){
  lcd()->setColorIndex(white);
  lcd()->drawBox(0, 0, 400, 240);

  lcd()->setFont(u8g2_font_10x20_t_cyrillic);  //ok
  lcd()->setColorIndex(black);
  lcd()->setCursor(5, 18); 
  lcd()->print("Будильник ");
  lcd()->print(modeAlertSettingsIndex);
  
  drawMenuItem(itemModeAlertSettingsBack, draw_ic24_back, "Назад", false);
  if(getAlertEnabled(modeAlertSettingsIndex))
    drawMenuItem(itemModeAlertSettingsEnabled, draw_ic24_check, "Вимкнути", false);
  else
    drawMenuItem(itemModeAlertSettingsEnabled, draw_ic24_empty, "Увімкнути", false);

  drawMenuItem(itemModeAlertSettingsHour, draw_ic24_music, "Мелодії", false);
  drawMenuItem(itemModeAlertSettingsMinute, draw_ic24_alarm, "Будильник", false);
  drawMenuItem(itemModeAlertSettingsMelody, draw_ic24_meow, "Meow", false);
  

  drawStatusbar(363, 1, true);
  drawMenuLegend();
  lcd()->sendBuffer();
}

void ModeAlertSettingsMenuButtonCenter(){
  if(selected == itemModeAlertSettingsBack){
    setModeMainMenu();
    return;
  }
  if(selected == itemModeAlertSettingsEnabled){
    saveAlertEnabled(modeAlertSettingsIndex, !getAlertEnabled(modeAlertSettingsIndex));
    return;
  }
  if(selected==itemModeAlertSettingsHour){
    
    return;
  }
  if(selected==itemModeAlertSettingsMinute){
    
    return;
  }
  if(selected==itemModeAlertSettingsMelody){
    
    return;
  }
}