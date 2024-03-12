const int itemModeAppsBack=0;
const int itemModeAppsStopwatch=1;
const int itemModeAppsMusic=2;
const int itemModeAppsAlarm=3;
const int itemModeAppsMeow=4;
const int itemModeAppsLife=5;
const int itemModeAppsCalendar=6;
const int itemModeAppsDebug=7;

void setModeAppsMenu(){
  clearScreenAnimation();
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
  items = 8;
}


void ModeAppsMenuLoop(){
  lcd()->setColorIndex(white);
  lcd()->drawBox(0, 0, 400, 240);

  lcd()->setFont(u8g2_font_10x20_t_cyrillic);  //ok
  lcd()->setColorIndex(black);
  lcd()->setCursor(5, 18); 
  lcd()->print("Програми");
  
  drawStatusbar(363, 1, true);
  drawMenuLegend();
  
  drawMenuItem(itemModeAppsBack,       draw_ic24_back,       "Назад",                         firstDraw);
  drawMenuItem(itemModeAppsStopwatch,  draw_ic24_stopwatch,  "Секундомір",                    firstDraw);
  drawMenuItem(itemModeAppsMusic,      draw_ic24_music,      "Мелодії",                       firstDraw);
  drawMenuItem(itemModeAppsAlarm,      draw_ic24_alarm,      "Будильник",                     firstDraw);
  drawMenuItem(itemModeAppsMeow,       draw_ic24_meow,       "Meow",                          firstDraw);
  drawMenuItem(itemModeAppsLife,       draw_ic24_life,       "Клітковий автомат \"Життя\"",   firstDraw);
  drawMenuItem(itemModeAppsCalendar,   draw_ic24_calendar,   "Календар",                      firstDraw);
  drawMenuItem(itemModeAppsDebug,      draw_ic24_bug,        "Інженерне меню",                firstDraw);
  
  
  

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
  if(selected == itemModeAppsCalendar){
    setModeCalendar();
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