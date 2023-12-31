int slotToFill=0;

const int modeWiFiListStatePreparing = 0;
const int modeWiFiListStateSettingUp = 1;
const int modeWiFiListStateSettingUpDelay = 2;
const int modeWiFiListStateSettingUpScanning = 3;
const int modeWiFiListStateSettingUpNoNetworks = 4;
const int modeWiFiListStateSettingUpSelectingNetwork = 5;
int modeWiFiListState = 0;
long modeWiFiListStateChangeTime = 0;

void setModeWiFiList(int _slotToFill){
  slotToFill = _slotToFill;
  setModeWiFiList_();
}
void setModeWiFiList(){
  slotToFill = 0;
  setModeWiFiList_();
}
void setModeWiFiList_(){
  Serial.println(F("Set mode: WiFiList"));
  modeSetup = setModeWiFiList_;
  modeLoop = modeWiFiListLoop;
  modeButtonUp = modeMainMenuButtonUp;
  modeButtonCenter = modeWiFiListButtonCenter;
  modeButtonDown = modeMainMenuButtonDown;
  //modeButtonUpLong = modeWatchfaceButtonUp;
  //modeButtonCenterLong = modeWatchfaceButtonUp;
  //modeButtonDownLong = modeWatchfaceButtonUp;
  modeWiFiListState = modeWiFiListStatePreparing;
  modeWiFiListStateChangeTime = millis();
}

void modeWiFiListLoop(){
  lcd()->setColorIndex(white);
  lcd()->drawBox(0, 0, 400, 240);


  drawBattery(339, 1);

  drawMenuLegend();
  
  if(modeWiFiListState == modeWiFiListStatePreparing){
    drawMessage("Підготовка...");
    if(millis()-modeWiFiListStateChangeTime > 500){
      modeWiFiListState = modeWiFiListStateSettingUp;
      modeWiFiListStateChangeTime = millis();
    }
  }
  else if(modeWiFiListState == modeWiFiListStateSettingUp){
    drawMessage("Налаштування Wi-Fi...");
    // Set WiFi to station mode and disconnect from an AP if it was previously connected.
    WiFi.mode(WIFI_STA);
    WiFi.disconnect();
    modeWiFiListState = modeWiFiListStateSettingUpDelay;
    modeWiFiListStateChangeTime = millis();
  }
  else if(modeWiFiListState == modeWiFiListStateSettingUpDelay){
    drawMessage("Підготовка сканування...");
    if(millis()-modeWiFiListStateChangeTime > 500){
      modeWiFiListState = modeWiFiListStateSettingUpScanning;
      modeWiFiListStateChangeTime = millis();
    }
  }
  else if(modeWiFiListState == modeWiFiListStateSettingUpScanning){
    drawMessage("Cканування...");
    int n = WiFi.scanNetworks();
    if(n==0){
      modeWiFiListState = modeWiFiListStateSettingUpNoNetworks;
      modeWiFiListStateChangeTime = millis();
    }
    else{
      modeWiFiListState = modeWiFiListStateSettingUpSelectingNetwork;
      modeWiFiListStateChangeTime = millis();
      selected = 0;
      items = n+1;
    }
  }
  else if(modeWiFiListState == modeWiFiListStateSettingUpNoNetworks){
    drawMessage("Мереж не знайдено.");
  }
  else if(modeWiFiListState == modeWiFiListStateSettingUpSelectingNetwork){
    if(slotToFill == 0)
      drawListItem(0, draw_ic24_arrow_left, "Повернутись", "В меню налаштувань", false); //
    else
      drawListItem(0, draw_ic24_arrow_left, "Повернутись", "До списку збережених мереж", false); //
    for(int i=0; i<items; i++){
      int rssi = WiFi.RSSI(i);
      void (*drawIcon)(int x,int y, bool color);
      if(rssi > -55) drawIcon = draw_ic24_wifi_3;
      else if (rssi > -67) drawIcon = draw_ic24_wifi_2;
      else if (rssi > -70) drawIcon = draw_ic24_wifi_1;
      else if (rssi > -80) drawIcon = draw_ic24_wifi_0;
      drawListItem(i+1, drawIcon, WiFi.SSID(i).c_str(), (String("")+rssi+"dBm " + encryprionType(i)).c_str(), false); //
    }
  }
  
  lcd()->sendBuffer();
  resetTemperatureSensor();
  if(sinceLastAction() > autoReturnTime) //auto go to watchface
    setModeWatchface();
}


void modeWiFiListButtonCenter(){
  if(modeWiFiListState == modeWiFiListStateSettingUpNoNetworks){
      if(slotToFill == 0)
        setModeMainMenu();
      else
        setModeSavedWiFiList();
  }
  if(modeWiFiListState == modeWiFiListStateSettingUpSelectingNetwork){
    if(selected == 0){
        if(slotToFill == 0)
          setModeMainMenu();
        else
          setModeSavedWiFiList();
    }
  }
}

const char* encryprionType(int i){
  switch (WiFi.encryptionType(i))
  {
  case WIFI_AUTH_OPEN:
      return ("Open");
  case WIFI_AUTH_WEP:
      return ("WEP");
  case WIFI_AUTH_WPA_PSK:
      return ("WPA");
  case WIFI_AUTH_WPA2_PSK:
      return  ("WPA2");
  case WIFI_AUTH_WPA_WPA2_PSK:
      return  ("WPA+WPA2");
  case WIFI_AUTH_WPA2_ENTERPRISE:
      return ("WPA2-EAP");
  case WIFI_AUTH_WPA3_PSK:
      return ("WPA3");
  case WIFI_AUTH_WPA2_WPA3_PSK:
      return ("WPA2+WPA3");
  case WIFI_AUTH_WAPI_PSK:
      return ("WAPI");
  default:
      return ("unknown");
  }
}