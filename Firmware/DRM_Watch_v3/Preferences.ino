#include <Preferences.h>

//   max name len: 15

Preferences preferencesObject;

void initPreferences(){
  preferencesObject.begin("drm-watch-v3", false);
}


//----------------------//---------------------- STOPWATCH ----------//----------------------//----------------------//----------------------//----------------------
bool saveStopwatchStartedTime(unsigned long epoch){
  return preferencesObject.putULong64("stopwatch1", epoch) > 0;
}

bool saveStopwatchFinishedTime(unsigned long epoch){
  return preferencesObject.putULong64("stopwatch2", epoch) > 0;
}

unsigned long getStopwatchStartedTime(){
  return preferencesObject.getULong64("stopwatch1", 0);
}

unsigned long getStopwatchFinishedTime(){
  return preferencesObject.getULong64("stopwatch2", 0);
}

bool isStopwatchRunning(){
  return getStopwatchStartedTime() != 0 && getStopwatchFinishedTime() == 0;
}

//----------------------//---------------------- SAVED WI-FI --------//----------------------//----------------------//----------------------//----------------------

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

//----------------------//---------------------- TIME SYNC ------------//----------------------//----------------------//----------------------//----------------------
bool saveLastTryTimeSync(unsigned long epoch){
  return preferencesObject.putULong64("lastTryTimeSync", epoch) > 0;
}
unsigned long getLastTryTimeSync(){
  return preferencesObject.getULong64("lastTryTimeSync", 0);
}
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

//----------------------//---------------------- DISPLAY COLORS -------//----------------------//----------------------//----------------------//----------------------

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

//----------------------//---------------------- LAST CHARGED ------------//----------------------//----------------------//----------------------//----------------------

unsigned long getLastChargedTime(){
  return preferencesObject.getULong64("LastCharged", 0);
}
unsigned long getTimeSinceLastCharged(){
  return rtcGetEpoch() - getLastChargedTime();
}
bool saveLastChargedTime(unsigned long epoch){
  return preferencesObject.putULong64("LastCharged", epoch) > 0;
}
bool saveLastChargedTime(){
  return saveLastChargedTime(rtcGetEpoch());
}

//----------------------//---------------------- ALERT -----------------//----------------------//----------------------//----------------------//----------------------
/*  bool alertIsEnabled = eepromReadAlertEnabled();
    byte alertLastRunDay = eepromReadAlertLastDayRun();
    byte alertTimeHour = eepromReadAlertHour();
    byte alertTimeMinute = eepromReadAlertMinute();
    byte alertMelodyIndex = eepromReadAlertMelodyIndex();*/

int getAlertsNumber(){
  return 1;
}
bool getAnyAlertEnabled(){
  for(int i=0; i<getAlertsNumber(); i++)
    if(getAlertEnabled(i))
      return true;
  return false;
}
bool getAlertEnabled(int index){         //123456789012345
  return preferencesObject.getInt((String("alertEnabled")+index).c_str() , 0)==1;
}
bool saveAlertEnabled(int index, bool value){
  return preferencesObject.putInt((String("alertEnabled")+index).c_str(), value?1:0);
}
int getAlertLastRunDay(int index){       //123456789012345
  return preferencesObject.getInt((String("alertLRD")+index).c_str() , 0);
}
bool saveAlertLastRunDay(int index, int value){
  return preferencesObject.putInt((String("alertLRD")+index).c_str(), value);
}
int getAlertHour(int index){             //123456789012345
  return preferencesObject.getInt((String("alertHour")+index).c_str() , 0);
}
bool saveAlertHour(int index, int value){
  return preferencesObject.putInt((String("alertHour")+index).c_str(), value);
}
int getAlertMinute(int index){             //123456789012345
  return preferencesObject.getInt((String("alertMinute")+index).c_str() , 0);
}
bool saveAlertMinute(int index, int value){
  return preferencesObject.putInt((String("alertMinute")+index).c_str(), value);
}
int getAlertMelody(int index){             //123456789012345
  return preferencesObject.getInt((String("alertMelody")+index).c_str() , 0);
}
bool saveAlertMelody(int index, int value){
  return preferencesObject.putInt((String("alertMelody")+index).c_str(), value);
}

  
//----------------------//----------------------//----------------------//----------------------//----------------------//----------------------//----------------------



int getPreferencesFreeSpace(){
  return preferencesObject.freeEntries();
}