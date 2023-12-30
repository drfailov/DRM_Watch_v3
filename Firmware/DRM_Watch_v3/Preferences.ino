#include <Preferences.h>

Preferences preferencesObject;

void initPreferences(){
  preferencesObject.begin("drm-watch-v3", false);
}

bool wifiSlotIsEmpty(int slot){
  String value=preferencesObject.getString((String("ws")+slot+"n").c_str(), "-empty-");
  return value.equals("-empty-");
}

int getPreferencesFreeSpace(){
  return preferencesObject.freeEntries();
}