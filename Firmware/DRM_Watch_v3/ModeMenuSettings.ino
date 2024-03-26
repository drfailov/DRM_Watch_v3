const int itemModeSettingsBack=0;
const int itemModeSettingsCategoryTime=1;
const int itemModeSettingsCategoryDisplay=2;
const int itemModeSettingsCategoryWiFi=3;
const int itemModeSettingsCategorySound=4;
const int itemModeSettingsReboot=5;

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
  items = 6;
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
  drawListItem(itemModeSettingsCategoryWiFi, draw_ic24_wifi_3, "Налаштування Wi-Fi", "Мережі для доступу до Інтернет", firstDraw); /*draw_ic24_saved*/
  drawListItem(itemModeSettingsCategorySound, draw_ic24_sound_on, "Налаштування звуку", "Налаштування що стосуються звуку", firstDraw);
  drawListItem(itemModeSettingsReboot, draw_ic24_reboot, "Перезавантажити", "Перезавантажити годинник", firstDraw);
  

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
  if(selected==itemModeSettingsCategoryWiFi){
    setModeSavedWiFiList();
    return;
  }
  if(selected==itemModeSettingsCategorySound){
    setModeMenuSettingsSound();
    return;
  }
  if(selected==itemModeSettingsReboot){
    //modeButtonUpLong(); //since this var is not set, it will reboot system
    clearScreenAnimation();
    Wire.flush();
    delay(50);
    Wire.end();
    delay(50);
    esp_restart();
    delay(50);
    return;
  }
}