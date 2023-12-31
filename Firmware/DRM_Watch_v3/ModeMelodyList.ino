const int itemModeMelodyListBack=0;

void setModeMelodyListMenu(){
  Serial.println(F("Set mode: Melody List"));
  modeSetup = setModeMelodyListMenu;
  modeLoop = ModeMelodyListMenuLoop;
  modeButtonUp = modeMainMenuButtonUp;
  modeButtonCenter = ModeMelodyListMenuButtonCenter;
  modeButtonDown = modeMainMenuButtonDown;
  modeButtonUpLong = 0;
  modeButtonCenterLong = 0;
  modeButtonDownLong = 0;
  registerAction();
  enableAutoReturn = true;
  //selected = 0;
  items = getMelodyCount()+1;
}


void ModeMelodyListMenuLoop(){
  lcd()->setColorIndex(white);
  lcd()->drawBox(0, 0, 400, 240);

  lcd()->setFont(u8g2_font_10x20_t_cyrillic);  //ok
  lcd()->setColorIndex(black);
  lcd()->setCursor(5, 18); 
  lcd()->print("Список Мелодій");
  
  drawMenuItem(itemModeMelodyListBack, draw_ic24_arrow_left, "Назад", false);
  for(int i=0; i<getMelodyCount(); i++){
    drawMenuItem(i+1, draw_ic24_music, getMelodyName(i).c_str(), false);
  }

  drawStatusbar(363, 1, true);
  drawMenuLegend();
  lcd()->sendBuffer();
}

void ModeMelodyListMenuButtonCenter(){
  if(selected == itemBack){
    setModeAppsMenu();
    return;
  }
  else{
    melodyPlayerSetMelodyName(getMelodyName(selected-1));
    melodyPlayerPlayMelody(getMelodyData(selected-1));
    return;
  }
  
}