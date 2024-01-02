int ModeSavedWiFiListSelectedSlot = 0;

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
  items = wifiSlotCnt()+1;
}


void modeSavedWiFiListLoop(){
  lcd()->setColorIndex(white);
  lcd()->drawBox(0, 0, 400, 240);


  lcd()->setFont(u8g2_font_10x20_t_cyrillic);  //ok
  lcd()->setColorIndex(black);
  lcd()->setCursor(5, 18); 
  lcd()->print("Збережені мережі");

  drawMenuLegend();
  drawBattery(339, 1);

  drawListItem(0, draw_ic24_arrow_left, "Повернутись", "В меню налаштувань", false); //
  for(int i=1; i<items; i++){
    if(wifiSlotIsEmpty(i))
      drawListItem(i, draw_ic24_wifi_0, "Пусто", "Немає даних про мережу", false); //
    else
      drawListItem(i, draw_ic24_wifi_3, wifiSlotName(i).c_str(), wifiSlotPassword(i).c_str(), false); //
  }

  lcd()->sendBuffer();
  
  if(sinceLastAction() > autoReturnTime) //auto go to watchface
    setModeWatchface();
}

void modeSavedWiFiListButtonCenter(){
  ModeSavedWiFiListSelectedSlot = selected;
  if(selected == 0){
      setModeSettingsMenu();
  }
  else if(wifiSlotIsEmpty(selected)){
    setModeWiFiScanner(modeSavedWiFiListOnNetworkNameSelected, setModeSavedWiFiList);
  }
  else{
    //deleet
    questionModeSet("Видалити мережу?", wifiSlotName(selected), modeSavedWiFiListOnDeleteNetwork, setModeSavedWiFiList);
  }
}

void modeSavedWiFiListOnDeleteNetwork(){
  wifiSlotClear(selected);
  setModeSavedWiFiList();
}

void modeSavedWiFiListOnNetworkNameSelected(){
  setModeKeyboard(String("Введіть пароль для ")+modeWiFiScannerGetSelectedNetworkName(), modeSavedWiFiListOnNetworkPasswordSelected, setModeSavedWiFiList);
}

void modeSavedWiFiListOnNetworkPasswordSelected(){
  String ssid = modeWiFiScannerGetSelectedNetworkName();
  String password = getKeybordResult();
  tryConnectWifi(ssid, password, modeSavedWiFiListOnNetworkConnected, modeSavedWiFiListOnNetworkFailed);
}

void modeSavedWiFiListOnNetworkConnected(){
  String ssid = modeWiFiScannerGetSelectedNetworkName();
  String password = getKeybordResult();
  int slot = ModeSavedWiFiListSelectedSlot;
  drawMessage("Збереження...", ssid + " " + password);
  delay(500);
  if(wifiSlotSave(slot, ssid, password)){
    drawMessage("Збережено.");
  }
  delay(500);
  wifiOff();
  setModeSavedWiFiList();
}

void modeSavedWiFiListOnNetworkFailed(){
  setModeSavedWiFiList();
}


