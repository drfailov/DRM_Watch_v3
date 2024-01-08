const int itemModeSettingsBack=0;
const int itemModeSettingsSelectWatchface=1;
const int itemModeSettingsSavedNetworks=2;
const int itemModeSettingsSynchronizeTime=3;
const int itemModeSettingsSetTime=4;
const int itemModeSettingsSetTimeZone=5;
const int itemModeSettingsReboot=6;
const int itemModeSettingsInvert=7;

void setModeSettingsMenu(){
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
  selected = 0;
  items = 8;
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

  drawMenuItem(itemModeSettingsBack, draw_ic24_arrow_left, "Назад", false);
  drawMenuItem(itemModeSettingsSelectWatchface, draw_ic24_watchface, "Обрати циферблат", false);
  drawMenuItem(itemModeSettingsSavedNetworks, draw_ic24_saved, "Список збережениx мереж Wi-Fi", false);
  drawMenuItem(itemModeSettingsSynchronizeTime, draw_ic24_sync, "Синхронізувати час з сервером", false);
  drawMenuItem(itemModeSettingsSetTime, draw_ic24_clock, "Задати час", false);
  drawMenuItem(itemModeSettingsSetTimeZone, draw_ic24_timezone, "Обрати часовий пояс", false);
  drawMenuItem(itemModeSettingsReboot, draw_ic24_reboot, "Перезавантажити", false);
  drawMenuItem(itemModeSettingsInvert, draw_ic24_invert, "Інвертувати екран", false);

  lcd()->sendBuffer();
}

void modeSettingsMenuButtonCenter(){
  if(selected == itemBack){
    setModeMainMenu();
    return;
  }
  if(selected == itemModeSettingsSelectWatchface){
    drawMessage("В процесі розробки.");
    return;
  }
  if(selected==itemModeSettingsSavedNetworks){
    setModeSavedWiFiList();
    return;
  }
  if(selected==itemModeSettingsSynchronizeTime){
    timeSync();
    return;
  }
  if(selected==itemModeSettingsSetTime){
    drawMessage("В процесі розробки.");
    return;
  }
  if(selected==itemModeSettingsSetTimeZone){
    drawMessage("В процесі розробки.");
    return;
  }
  if(selected==itemModeSettingsReboot){
    modeButtonUpLong();
    return;
  }
  if(selected==itemModeSettingsInvert){
    saveInversionValue(!getInversionValue());
    black = getBlackValue();
    white = getWhiteValue();
    return;
  }
  
  
}