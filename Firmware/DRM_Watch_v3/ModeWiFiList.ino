const int modeWiFiListStatePreparing = 0;
const int modeWiFiListStateSettingUp = 1;
const int modeWiFiListStateSettingUpDelay = 2;
const int modeWiFiListStateSettingUpScanning = 3;
const int modeWiFiListStateSettingUpNoNetworks = 4;
const int modeWiFiListStateSettingUpSelectingNetwork = 5;
int modeWiFiListState = 0;
long modeWiFiListStateChangeTime = 0;

void setModeWiFiList(){
  Serial.println(F("Set mode: WiFiList"));
  modeSetup = setModeWiFiList;
  modeLoop = modeWiFiListLoop;
  modeButtonUp = modeMainMenuButtonUp;
  modeButtonCenter = modeTestButtonCenter;
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

  lcd()->setColorIndex(black);
  lcd()->drawLine(369, 0, 369, 260);
  lcd()->drawLine(370, 0, 370, 260);

  drawBattery(339, 1);
  draw_ic24_arrow_up(lx(), ly1(), black);
  draw_ic24_check2(lx(), ly2(), black);
  draw_ic24_arrow_down(lx(), ly3(), black);

  
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
    drawMessage("Підготовка до сканування...");
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
      items = n;
    }
  }
  else if(modeWiFiListState == modeWiFiListStateSettingUpNoNetworks){
    drawMessage("Мереж не знайдено");
  }
  else if(modeWiFiListState == modeWiFiListStateSettingUpSelectingNetwork){
    for(int i=0; i<items; i++){
      int rssi = WiFi.RSSI(i);
      void (*drawIcon)(int x,int y, bool color);
      if(rssi > -55) drawIcon = draw_ic24_wifi_3;
      else if (rssi > -67) drawIcon = draw_ic24_wifi_2;
      else if (rssi > -70) drawIcon = draw_ic24_wifi_1;
      else if (rssi > -80) drawIcon = draw_ic24_wifi_0;
      drawListItem(i, drawIcon, WiFi.SSID(i).c_str(), (String("")+rssi+"dBm " + encryprionType(i)).c_str(), false); //
    }
  }
  
  lcd()->sendBuffer();
  resetTemperatureSensor();
}

char* encryprionType(int i){
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