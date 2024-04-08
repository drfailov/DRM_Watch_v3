void setModeAlertsList(){
  clearScreenAnimation();
  Serial.println(F("Set mode: AlertsList"));
  modeSetup = setModeAlertsList;
  modeLoop = modeAlertsListLoop;
  modeButtonUp = modeMainMenuButtonUp;
  modeButtonCenter = modeAlertsListButtonCenter;
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
  items = getAlertsNumber()+1;
}


void modeAlertsListLoop(){
  lcd()->setColorIndex(white);
  lcd()->drawBox(0, 0, 400, 240);


  lcd()->setFont(u8g2_font_10x20_t_cyrillic);  //ok
  lcd()->setColorIndex(black);
  lcd()->setCursor(5, 18); 
  lcd()->print("Список будильників");

  drawMenuLegend();
  drawStatusbar(363, 1, true);

  drawListItem(0, draw_ic24_back, "Повернутись", "В меню програм", firstDraw); //
  for(int i=1; i<items; i++){
      drawListCheckbox(i, draw_ic24_alarm, ((String("Будильник ")+(i-1)).c_str()), getAlertName(i-1).c_str(), getAlertEnabled(i-1), firstDraw); 
  }

  lcd()->sendBuffer();
}

void modeAlertsListButtonCenter(){
  //ModeAlertsListSelectedSlot = selected;
  if(selected == 0){
      setModeAppsMenu(); //setModeSettingsMenu();
  }
  else{
    setModeAlertSettingsMenu(selected-1);
  }
  //  if(wifiSlotIsEmpty(selected)){
  //   setModeWiFiScanner(modeAlertsListOnNetworkNameSelected, setModeAlertsList);
  // }
  // else{
  //   //deleet
  //   questionModeSet("Видалити мережу?", wifiSlotName(selected), modeAlertsListOnDeleteNetwork, setModeAlertsList);
  // }
}

// void modeAlertsListOnDeleteNetwork(){
//   wifiSlotClear(selected);
//   setModeAlertsList();
// }

// void modeAlertsListOnNetworkNameSelected(){
//   setModeKeyboard(String("Пароль:")+modeWiFiScannerGetSelectedNetworkName(), modeAlertsListOnNetworkPasswordSelected, setModeAlertsList); //then String password = getKeybordResult();
// }

// void modeAlertsListOnNetworkPasswordSelected(){
//   String ssid = modeWiFiScannerGetSelectedNetworkName();
//   String password = getKeybordResult();
//   tryConnectWifi(ssid, password, modeAlertsListOnNetworkConnected, modeAlertsListOnNetworkFailed);
// }

// void modeAlertsListOnNetworkConnected(){
//   String ssid = modeWiFiScannerGetSelectedNetworkName();
//   String password = getKeybordResult();
//   int slot = ModeAlertsListSelectedSlot;
//   drawMessage("Збереження...", ssid + " " + password, true);
//   delay(500);
//   if(wifiSlotSave(slot, ssid, password)){
//     drawMessage("Збережено.");
//   }
//   delay(500);
//   wifiOff();
//   setModeAlertsList();
// }

// void modeAlertsListOnNetworkFailed(){
//   setModeAlertsList();
// }


