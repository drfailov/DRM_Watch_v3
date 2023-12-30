
void setModeSavedWiFiList(){
  Serial.println(F("Set mode: SavedWiFiList"));
  modeSetup = setModeSavedWiFiList;
  modeLoop = modeSavedWiFiListLoop;
  modeButtonUp = modeMainMenuButtonUp;
  modeButtonCenter = modeSavedWiFiListButtonCenter;
  modeButtonDown = modeMainMenuButtonDown;
  //modeButtonUpLong = modeWatchfaceButtonUp;
  //modeButtonCenterLong = modeWatchfaceButtonUp;
  //modeButtonDownLong = modeWatchfaceButtonUp;
  selected = 0;
  items = 6;
}


void modeSavedWiFiListLoop(){
  lcd()->setColorIndex(white);
  lcd()->drawBox(0, 0, 400, 240);
  drawMenuLegend();
  drawBattery(339, 1);

  drawListItem(0, draw_ic24_arrow_left, "Повернутись", "В меню налаштувань", false); //
  for(int i=1; i<items; i++){
    if(wifiSlotIsEmpty(i))
      drawListItem(i, draw_ic24_wifi_0, "Пусто", "Немає даних про мережу", false); //
  }

  lcd()->sendBuffer();
}

void modeSavedWiFiListButtonCenter(){
  if(selected == 0){
      setModeMainMenu();
  }
  else if(wifiSlotIsEmpty(selected)){
    setModeWiFiList(selected);
  }
}