#include <Preferences.h>

Preferences preferencesObject;

void initPreferences(){
  preferencesObject.begin("drm-watch-v3", false);
}

bool wifiSlotIsEmpty(int slot){
  return wifiSlotName(slot).equals("-empty-");
}

String wifiSlotName(int slot){
  return preferencesObject.getString((String("ws")+slot+"n").c_str(), "-empty-");
}

String wifiSlotPassword(int slot){
  return preferencesObject.getString((String("ws")+slot+"p").c_str(), "-empty-");
}

bool wifiSlotSave(int slot, String ssid, String password){
  int savedSsid = preferencesObject.putString((String("ws")+slot+"n").c_str(), ssid);
  int savedPassword = preferencesObject.putString((String("ws")+slot+"p").c_str(), password);
  return savedSsid>0 && savedPassword>0;
}

int getPreferencesFreeSpace(){
  return preferencesObject.freeEntries();
}