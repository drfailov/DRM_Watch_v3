const int ModeMenuSettingsDisplayItemBack = 0;
const int ModeMenuSettingsDisplayItemSelectWatchface = 1;
const int ModeMenuSettingsDisplayItemSelectWatchfaceContent = 2;
const int ModeMenuSettingsDisplayItemInvertDisplay = 3;

void setModeMenuSettingsDisplay(){
  clearScreenAnimation();
  Serial.println(F("Set mode: ModeMenuSettingsDisplay"));
  modeSetup = setModeMenuSettingsDisplay;
  modeLoop = ModeMenuSettingsDisplayLoop;
  modeButtonUp = modeMainMenuButtonUp;
  modeButtonCenter = ModeMenuSettingsDisplayButtonCenter;
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
  items = 4;
}


void ModeMenuSettingsDisplayLoop(){
  lcd()->setColorIndex(white);
  lcd()->drawBox(0, 0, 400, 240);

  lcd()->setFont(u8g2_font_10x20_t_cyrillic);  //ok
  lcd()->setColorIndex(black);
  lcd()->setCursor(5, 18); 
  lcd()->print("Параметри дисплея");

  drawMenuLegend();
  drawStatusbar(363, 1, true);

  drawListItem(ModeMenuSettingsDisplayItemBack, draw_ic24_back, "Повернутись", "В меню налаштувань", firstDraw); //
  drawListItem(ModeMenuSettingsDisplayItemSelectWatchface, draw_ic24_watchface, "Обрати циферблат", "Дизайн відображення часу", firstDraw); //
  drawListItem(ModeMenuSettingsDisplayItemSelectWatchfaceContent, draw_ic24_watchface, "Вміст циферблату", "Чи показувати елементи циферблатів", firstDraw); //
  drawListCheckbox(ModeMenuSettingsDisplayItemInvertDisplay, draw_ic24_invert, "Інвертувати екран", "Поміняти місцями чорне i біле", getInversionValue(), firstDraw); //

  lcd()->sendBuffer();
}

void ModeMenuSettingsDisplayButtonCenter(){
  if(selected == ModeMenuSettingsDisplayItemBack){
      setModeSettingsMenu();
  }
  if(selected == ModeMenuSettingsDisplayItemSelectWatchface){
    setModeSetWatchface();
    return;
  }
  if(selected == ModeMenuSettingsDisplayItemSelectWatchfaceContent){
    setModeMenuSettingsWatchfaceContent();
    return;
  }
  if(selected == ModeMenuSettingsDisplayItemInvertDisplay){
    saveInversionValue(!getInversionValue());
    black = getBlackValue();
    white = getWhiteValue();
    return;
  }  
}


