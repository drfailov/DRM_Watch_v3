#ifndef MODESAVEDWIFILIST_H
#define MODESAVEDWIFILIST_H

void setModeSavedWiFiList();
void modeSavedWiFiListLoop();
void modeSavedWiFiListButtonCenter();
void modeSavedWiFiListOnDeleteNetwork();
void modeSavedWiFiListOnNetworkNameSelected();
void modeSavedWiFiListOnNetworkPasswordSelected();
void modeSavedWiFiListOnNetworkConnected();
void modeSavedWiFiListOnNetworkFailed();


#include "../Global.h"
#include "../AutoSleep.h"
#include "../Button.h"
#include "../GlobalMenu.h"
#include "../ModeKeyboard.h"
#include "ModeWiFiScanner.h"
#include "../ModeQuestion.h"


int ModeSavedWiFiListSelectedSlot = 0;

void setModeSavedWiFiList(){
  clearScreenAnimation();
  Serial.println(F("Set mode: SavedWiFiList"));
  modeSetup = setModeSavedWiFiList;
  modeLoop = modeSavedWiFiListLoop;
  modeButtonUp = globalMenuButtonUp;
  modeButtonCenter = modeSavedWiFiListButtonCenter;
  modeButtonDown = globalMenuButtonDown;
  modeButtonUpLong = 0;
  modeButtonCenterLong = 0;
  modeButtonDownLong = 0;
  registerAction();
  enableAutoReturn = true;
  enableAutoSleep = false; 
  autoReturnTime = autoReturnDefaultTime;
  autoSleepTime = autoSleepDefaultTime;
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
  drawStatusbar(363, 1, true);

  drawListItem(0, draw_ic24_back, "Повернутись", "В меню налаштувань", firstDraw); //
  for(int i=1; i<items; i++){
    if(wifiSlotIsEmpty(i))
      drawListItem(i, draw_ic24_wifi_0, "Пусто", "Немає даних про мережу", firstDraw); //
    else
      drawListItem(i, draw_ic24_wifi_3, wifiSlotName(i).c_str(), wifiSlotPassword(i).c_str(), firstDraw); //
  }

  lcd()->sendBuffer();
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
  setModeKeyboard(String("Пароль:")+modeWiFiScannerGetSelectedNetworkName(), modeSavedWiFiListOnNetworkPasswordSelected, setModeSavedWiFiList); //then String password = getKeybordResult();
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
  drawMessage("Збереження...", ssid + " " + password, true);
  delay(500);
  if(wifiSlotSave(slot, ssid, password)){
    drawMessageAnimated("Збережено.");
  }
  delay(500);
  wifiOff();
  setModeSavedWiFiList();
}

void modeSavedWiFiListOnNetworkFailed(){
  //setModeSavedWiFiList();
}


#endif