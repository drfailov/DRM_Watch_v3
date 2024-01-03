const int itemModeAppsBack=0;
const int itemModeAppsAlarm=1;
const int itemModeAppsMusic=2;

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
  selected = 0;
  items = 3;
}


void ModeAppsMenuLoop(){
  lcd()->setColorIndex(white);
  lcd()->drawBox(0, 0, 400, 240);

  lcd()->setFont(u8g2_font_10x20_t_cyrillic);  //ok
  lcd()->setColorIndex(black);
  lcd()->setCursor(5, 18); 
  lcd()->print("Програми");
  
  drawMenuItem(itemModeAppsBack, draw_ic24_arrow_left, "Назад", false);
  drawMenuItem(itemModeAppsAlarm, draw_ic24_alarm, "Будильник", false);
  drawMenuItem(itemModeAppsMusic, draw_ic24_music, "Мелодії", false);

  drawStatusbar(363, 1, true);
  drawMenuLegend();
  lcd()->sendBuffer();
  
  if(sinceLastAction() > autoReturnTime) //auto go to watchface
    setModeWatchface();
}

void ModeAppsMenuButtonCenter(){
  if(selected == itemBack){
    setModeMainMenu();
    return;
  }
  if(selected == itemModeAppsAlarm){
    drawMessage("В процесі розробки.");
    return;
  }
  if(selected==itemModeAppsMusic){
    //playMelody();
    //melodyPlayerPlayMelody(getMelodyGroovyBlue());
    
    //melodyPlayerPlayMelody(getMelodyData(0));
    setModeMelodyListMenu();
    
    return;
  }
  
}