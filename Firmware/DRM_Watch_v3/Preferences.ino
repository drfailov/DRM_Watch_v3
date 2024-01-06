#include <Preferences.h>

Preferences preferencesObject;

void initPreferences(){
  preferencesObject.begin("drm-watch-v3", false);
}

int wifiSlotCnt(){
  return 7;
}

bool wifiSlotIsEmpty(int slot){ //1 ... wifiSlotCnt
  return wifiSlotName(slot).equals("-empty-");
}

String wifiSlotName(int slot){ //1 ... wifiSlotCnt
  return preferencesObject.getString((String("ws")+slot+"n").c_str(), "-empty-");
}

String wifiSlotPassword(int slot){ //1 ... wifiSlotCnt
  return preferencesObject.getString((String("ws")+slot+"p").c_str(), "-empty-");
}

int getWifiSavedIndex(String name){
  for(int i=1; i<wifiSlotCnt(); i++){
    if(wifiSlotName(i) == name)
      return i;
  }
  return -1;
}

bool wifiSlotSave(int slot, String ssid, String password){ //1 ... wifiSlotCnt
  int savedSsid = preferencesObject.putString((String("ws")+slot+"n").c_str(), ssid);
  int savedPassword = preferencesObject.putString((String("ws")+slot+"p").c_str(), password);
  return savedSsid>0 && savedPassword>0;
}

bool wifiSlotClear(int slot){ //1 ... wifiSlotCnt
  return wifiSlotSave(slot, "-empty-", "-empty-");
}

//----------------------//----------------------//----------------------//----------------------//----------------------//----------------------//----------------------

bool saveLastTimeSync(unsigned long epoch){
  return preferencesObject.putULong64("lastTimeSync1", epoch) > 0;
}
unsigned long getLastTimeSync(){
  return preferencesObject.getULong64("lastTimeSync1", 0);
}
bool saveTimeCoef(double coef){
  return preferencesObject.putDouble("timeCorrection1", coef) > 0;
}
double getTimeCoef(){
  return preferencesObject.getDouble("timeCorrection1", 0);
}

//----------------------//----------------------//----------------------//----------------------//----------------------//----------------------//----------------------

bool getWhiteValue(){
  return !getInversionValue();
}
bool getBlackValue(){
  return getInversionValue();
}
bool getInversionValue(){
  return preferencesObject.getInt("screenInverse", 0)==1;
}
void saveInversionValue(bool value){
  preferencesObject.putInt("screenInverse", value?1:0);
}

//----------------------//----------------------//----------------------//----------------------//----------------------//----------------------//----------------------

int getPreferencesFreeSpace(){
  return preferencesObject.freeEntries();
}