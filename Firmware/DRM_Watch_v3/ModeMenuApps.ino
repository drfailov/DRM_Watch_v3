const int itemModeAppsBack=0;
const int itemModeAppsStopwatch=1;
const int itemModeAppsMusic=2;
const int itemModeAppsAlarm=3;
const int itemModeAppsMeow=4;
const int itemModeAppsDebug=5;
const int itemModeAppsLife=6;

void setModeAppsMenu(){
  Serial.println(F("Set mode: Apps Menu"));
  modeSetup = setModeAppsMenu;
  modeLoop = ModeAppsMenuLoop;
  modeButtonUp = modeMainMenuButtonUp;
  modeButtonCenter = ModeAppsMenuButtonCenter;
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


void ModeAppsMenuLoop(){
  lcd()->setColorIndex(white);
  lcd()->drawBox(0, 0, 400, 240);

  lcd()->setFont(u8g2_font_10x20_t_cyrillic);  //ok
  lcd()->setColorIndex(black);
  lcd()->setCursor(5, 18); 
  lcd()->print("Програми");
  
  drawMenuItem(itemModeAppsBack, draw_ic24_back, "Назад", false);
  drawMenuItem(itemModeAppsStopwatch, draw_ic24_stopwatch, "Секундомір", false);
  drawMenuItem(itemModeAppsMusic, draw_ic24_music, "Мелодії", false);
  drawMenuItem(itemModeAppsAlarm, draw_ic24_alarm, "Будильник", false);
  drawMenuItem(itemModeAppsMeow, draw_ic24_meow, "Meow", false);
  drawMenuItem(itemModeAppsDebug, draw_ic24_bug, "Інженерне меню", false);
  drawMenuItem(itemModeAppsLife, draw_ic24_life, "Клітковий автомат \"Життя\"", false);
  
  

  drawStatusbar(363, 1, true);
  drawMenuLegend();
  lcd()->sendBuffer();
}

void ModeAppsMenuButtonCenter(){
  if(selected == itemModeAppsBack){
    setModeMainMenu();
    return;
  }
  if(selected == itemModeAppsAlarm){
    setModeAlertSettingsMenu();
    return;
  }
  if(selected==itemModeAppsMusic){
    setModeMelodyListMenu();
    selected = 0;
    return;
  }
  if(selected==itemModeAppsStopwatch){
    setModeStopwatch();
    return;
  }
  if(selected==itemModeAppsMeow){
    setModeMeow();
    return;
  }
  if(selected == itemModeAppsDebug){
    setModeTest();
    return;
  }
  if(selected == itemModeAppsLife){
    setModeLife();
    return;
  }
  
}