#include <WiFi.h>

String modeWiFiScannerSelectedNetwork = "";
Runnable modeWiFiScannerOnNetworkSelected = 0;
Runnable modeWiFiScannerOnCancel = 0;

const int modeWiFiScannerStatePreparing = 0;
const int modeWiFiScannerStateSettingUp = 1;
const int modeWiFiScannerStateSettingUpDelay = 2;
const int modeWiFiScannerStateSettingUpScanning = 3;
const int modeWiFiScannerStateSettingUpNoNetworks = 4;
const int modeWiFiScannerStateSettingUpSelectingNetwork = 5;
int modeWiFiScannerState = 0;
long modeWiFiScannerStateChangeTime = 0;

void setModeWiFiScanner(Runnable onSelected, Runnable onCancel){
  modeWiFiScannerOnNetworkSelected = onSelected;
  modeWiFiScannerOnCancel = onCancel;
  enableAutoReturn = true;
  setModeWiFiScanner_();
}
void setModeWiFiScanner(){
  setModeWiFiScanner_();
}
void setModeWiFiScanner_(){
  clearScreenAnimation();
  Serial.println(F("Set mode: WiFiScanner"));
  modeSetup = setModeWiFiScanner_;
  modeLoop = modeWiFiScannerLoop;
  modeButtonUp = modeMainMenuButtonUp;
  modeButtonCenter = modeWiFiScannerButtonCenter;
  modeButtonDown = modeMainMenuButtonDown;
  modeButtonUpLong = 0;
  modeButtonCenterLong = 0;
  modeButtonDownLong = 0;
  registerAction();
  enableAutoReturn = true;
  enableAutoSleep = false; 
  autoReturnTime = autoReturnDefaultTime;
  autoSleepTime = autoSleepDefaultTime;
  
  modeWiFiScannerState = modeWiFiScannerStatePreparing;
  modeWiFiScannerStateChangeTime = millis();
}

void modeWiFiScannerLoop(){
  lcd()->setColorIndex(white);
  lcd()->drawBox(0, 0, 400, 240);

  lcd()->setFont(u8g2_font_10x20_t_cyrillic);  //ok
  lcd()->setColorIndex(black);
  lcd()->setCursor(5, 18); 
  lcd()->print("Знайдені мережі");

  drawStatusbar(363, 1, true);

  drawMenuLegend();
  
  if(modeWiFiScannerState == modeWiFiScannerStatePreparing){
    drawMessage("Підготовка...");
    if(millis()-modeWiFiScannerStateChangeTime > 500){
      modeWiFiScannerState = modeWiFiScannerStateSettingUp;
      modeWiFiScannerStateChangeTime = millis();
    }
  }
  else if(modeWiFiScannerState == modeWiFiScannerStateSettingUp){
    drawMessage("Налаштування Wi-Fi...");
    // Set WiFi to station mode and disconnect from an AP if it was previously connected.
    WiFi.mode(WIFI_STA);
    WiFi.disconnect();
    modeWiFiScannerState = modeWiFiScannerStateSettingUpDelay;
    modeWiFiScannerStateChangeTime = millis();
  }
  else if(modeWiFiScannerState == modeWiFiScannerStateSettingUpDelay){
    drawMessage("Підготовка сканування...");
    if(millis()-modeWiFiScannerStateChangeTime > 500){
      modeWiFiScannerState = modeWiFiScannerStateSettingUpScanning;
      modeWiFiScannerStateChangeTime = millis();
    }
  }
  else if(modeWiFiScannerState == modeWiFiScannerStateSettingUpScanning){
    drawMessage("Cканування...");
    int n = WiFi.scanNetworks();
    if(n==0){
      modeWiFiScannerState = modeWiFiScannerStateSettingUpNoNetworks;
      modeWiFiScannerStateChangeTime = millis();
    }
    else{
      modeWiFiScannerState = modeWiFiScannerStateSettingUpSelectingNetwork;
      modeWiFiScannerStateChangeTime = millis();
      selected = 0;
      items = n+1;
    }
  }
  else if(modeWiFiScannerState == modeWiFiScannerStateSettingUpNoNetworks){
    drawMessage("Мереж не знайдено.");
  }
  else if(modeWiFiScannerState == modeWiFiScannerStateSettingUpSelectingNetwork){
    drawListItem(0, draw_ic24_back, "Повернутись", "Назад", false); 
    for(int i=0; i<items-1; i++){
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
  resetCpuTemperatureSensor();
}

String modeWiFiScannerGetSelectedNetworkName(){
  return modeWiFiScannerSelectedNetwork;
}

void modeWiFiScannerButtonCenter(){
  if(modeWiFiScannerState == modeWiFiScannerStateSettingUpNoNetworks){
      if(modeWiFiScannerOnCancel != 0)
        modeWiFiScannerOnCancel();
      else
        setModeMainMenu();
  }
  if(modeWiFiScannerState == modeWiFiScannerStateSettingUpSelectingNetwork){
    if(selected == 0){
      if(modeWiFiScannerOnCancel != 0)
        modeWiFiScannerOnCancel();
      else
        setModeMainMenu();
    }
    else{
      modeWiFiScannerSelectedNetwork = WiFi.SSID(selected-1);
      if(modeWiFiScannerOnNetworkSelected != 0)
        modeWiFiScannerOnNetworkSelected();
    }
  }
}

bool tryConnectWifi(String ssid, String password, Runnable onConnected, Runnable onFailed){
  drawMessage("Спроба з'єднання..", ssid + " " + password, true);
  WiFi.begin(ssid, password);
  for (long timeStarted = millis(); WiFi.status() != WL_CONNECTED ;) {
    delay(900);
    lcd()->print(".");
    lcd()->sendBuffer();
    if(millis()-timeStarted > 10000){
      drawMessage("З'єднатись не вдалось.");
      delay(500);
      if(onFailed != 0)
        onFailed();
      return false;
    }
  }
  drawMessage("Підключено.");
  delay(500);
  if(onConnected != 0)
    onConnected();
  return true;
}

bool connectToKnownWifi(){
  drawMessage("Налаштування Wi-Fi...");
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  delay(100);
  drawMessage("Cканування...");
  int n = WiFi.scanNetworks();
  drawMessage("Пошук знайомих мереж...");
  for(int i=0; i<n; i++){
    String name = WiFi.SSID(i);
    int index = getWifiSavedIndex(name);
    if(index != -1 && tryConnectWifi(name, wifiSlotPassword(index),0,0))
        return true;
  }
  drawMessage("Відомих мереж не знайдено.", "Додайте одну з доступних мереж.", true);
  delay(2000);
  wifiOff();
  return false;
}

void wifiOff(){
  drawMessage("Вимкнення Wi-Fi...");
  WiFi.disconnect(true);
  WiFi.mode(WIFI_OFF);
  resetCpuTemperatureSensor();
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