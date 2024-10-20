#include <Preferences.h>

//   max name len: 15

Preferences preferencesObject;

void initPreferences(){
  preferencesObject.begin("drm-watch-v3", false);
}

//----------------------//---------------------- TIMER ----------//----------------------//----------------------//----------------------//----------------------

bool saveTimerStartedTime(unsigned long epoch){  //unsigned long    saveTimerStartedTime  saveTimerTime  getTimerStartedTime  getTimerTime  isTimerRunning
  return preferencesObject.putULong64("timerSta", epoch) > 0;
}

bool saveTimerTime(unsigned long epoch){
  return preferencesObject.putULong64("timerTime", epoch) > 0;
}

unsigned long getTimerStartedTime(){
  return preferencesObject.getULong64("timerSta", 0);
}

unsigned long getTimerTime(){
  return preferencesObject.getULong64("timerTime", 0);
}

bool isTimerRunning(){
  return getTimerStartedTime() != 0;
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

int stopwatchHistorySlotCnt(){   
  return 5;
}

bool stopwatchHistorySlotIsEmpty(int slot){ //1 ... stopwatchHistorySlotCnt
  return stopwatchHistorySlotStart(slot) == 0;
}

unsigned long stopwatchHistorySlotStart(int slot){ //1 ... stopwatchHistorySlotCnt
  return preferencesObject.getULong64((String("sws")+slot+"sta").c_str(), 0);
}

unsigned long stopwatchHistorySlotEnd(int slot){ //1 ... stopwatchHistorySlotCnt
  return preferencesObject.getULong64((String("sws")+slot+"end").c_str(), 0);
}

bool saveStopwatchHistorySlotStart(int slot, unsigned long epoch){ //1 ... stopwatchHistorySlotCnt   
  return preferencesObject.putULong64((String("sws")+slot+"sta").c_str(), epoch) > 0;
}

bool saveStopwatchHistorySlotEnd(int slot, unsigned long epoch){ //1 ... stopwatchHistorySlotCnt
  return preferencesObject.putULong64((String("sws")+slot+"end").c_str(), epoch) > 0;
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

//----------------------//---------------------- TIMEZONE ------------//----------------------//----------------------//----------------------//----------------------

long getTimeOffsetSec(){          //123456789012345
  return preferencesObject.getInt("timeOffsetSec", 60*60*2);
}
bool saveTimeOffsetSec(long value){
  return preferencesObject.putInt("timeOffsetSec", value);
}

//----------------------//---------------------- TIME SYNC ------------//----------------------//----------------------//----------------------//----------------------
bool getTimeSyncEnabled(){               //123456789012345
  return preferencesObject.getInt((String("TimeSyncEnable")).c_str() , 0)==1;
}
bool saveTimeSyncEnabled(bool value){
  return preferencesObject.putInt((String("TimeSyncEnable")).c_str(), value?1:0);
}
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
// bool saveTimeCoef(double coef){
//   return preferencesObject.putDouble("timeCorrection1", coef) > 0;
// }
// double getTimeCoef(){
//   return preferencesObject.getDouble("timeCorrection1", 0);
// }

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

bool getEnterAnimationValue(){
  return preferencesObject.getInt("screenInAnim", 1)==1;
}
void saveEnterAnimationValue(bool value){
  preferencesObject.putInt("screenInAnim", value?1:0);
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

//----------------------//---------------------- MUTE -----------------//----------------------//----------------------//----------------------//----------------------

bool getMuteEnabled(){                   //123456789012345
  return preferencesObject.getInt((String("soundMute")).c_str() , 0)==1;
}
bool saveMuteEnabled(bool value){
  return preferencesObject.putInt((String("soundMute")).c_str(), value?1:0);
}

//----------------------//---------------------- ANT SPEED -----------------//----------------------//----------------------//----------------------//----------------------

int getAntSpeed(){  //getAntSpeed   saveAntSpeed
  return preferencesObject.getInt("AntSpeed" , 50);
}
bool saveAntSpeed(int value){
  return preferencesObject.putInt("AntSpeed", value);
}

//----------------------//---------------------- BUTTON SOUND -----------------//----------------------//----------------------//----------------------//----------------------


int getButtonSound(){
  return preferencesObject.getInt("ButtonSound" , 1);
}
bool saveButtonSound(int value){
  int totalSounds = buttonBeepCnt();
  return preferencesObject.putInt("ButtonSound", value%totalSounds);
}

//----------------------//---------------------- WATCHFACE -----------------//----------------------//----------------------//----------------------//----------------------


int getWatchface(){
  return preferencesObject.getInt("Watchface" , 0);
}
bool saveWatchface(int value){
  int total = getWatchfaceCount();
  return preferencesObject.putInt("Watchface", value%total);
}
bool getWatchfaceStatusbarEnabled(){//123456789012345   max length = 15
  return preferencesObject.getInt(   "wtfStatusbarEn", 1)==1;
}
bool saveWatchfaceStatusbarEnabled(bool value){
  return preferencesObject.putInt("wtfStatusbarEn", value?1:0);
}
bool getWatchfaceAnalogEnabled(){     //123456789012345   max length = 15
  return preferencesObject.getInt(     "wtfAnalogEn", 1)==1;
}
bool saveWatchfaceAnalogEnabled(bool value){
  return preferencesObject.putInt("wtfAnalogEn", value?1:0);
}
bool getWatchfaceDigitalEnabled(){     //123456789012345   max length = 15
  return preferencesObject.getInt(      "wtfDigitalEn", 1)==1;
}
bool saveWatchfaceDigitalEnabled(bool value){
  return preferencesObject.putInt("wtfDigitalEn", value?1:0);
}
bool getWatchfaceStatusbarDigitalEnabled(){     //123456789012345   max length = 15
  return preferencesObject.getInt(               "wtfStDigitalEn", 0)==1;
}
bool saveWatchfaceStatusbarDigitalEnabled(bool value){
  return preferencesObject.putInt("wtfStDigitalEn", value?1:0);
}
bool getWatchfaceTemperatureEnabled(){     //123456789012345   max length = 15
  return preferencesObject.getInt(          "wtfTempEn", 1)==1;
}
bool saveWatchfaceTemperatureEnabled(bool value){
  return preferencesObject.putInt("wtfTempEn", value?1:0);
}
bool getWatchfaceDayOfWeekEnabled(){     //123456789012345   max length = 15
  return preferencesObject.getInt(        "wtfDayOfWeekEn", 1)==1;
}
bool saveWatchfaceDayOfWeekEnabled(bool value){
  return preferencesObject.putInt("wtfDayOfWeekEn", value?1:0);
}
bool getWatchfaceDateEnabled(){     //123456789012345   max length = 15
  return preferencesObject.getInt("wtfDateEn", 1)==1;
}
bool saveWatchfaceDateEnabled(bool value){
  return preferencesObject.putInt("wtfDateEn", value?1:0);
}
bool getWatchfaceMonthEnabled(){     //123456789012345   max length = 15
  return preferencesObject.getInt("wtfMonthEn", 0)==1;
}
bool saveWatchfaceMonthEnabled(bool value){
  return preferencesObject.putInt("wtfMonthEn", value?1:0);
}
bool getWatchfaceDjiLogoEnabled(){     //123456789012345   max length = 15
  return preferencesObject.getInt("wtfDjiEn", 0)==1;
}
bool saveWatchfaceDjiLogoEnabled(bool value){
  return preferencesObject.putInt("wtfDjiEn", value?1:0);
}
bool getWatchfaceLifeBackgroundEnabled(){     //123456789012345   max length = 15
  return preferencesObject.getInt("wtfLifeEn", 1)==1;
}
bool saveWatchfaceLifeBackgroundEnabled(bool value){
  return preferencesObject.putInt("wtfLifeEn", value?1:0);
}
bool getWatchfaceAntBackgroundEnabled(){     //123456789012345   max length = 15
  return preferencesObject.getInt("wtfAntEn", 0)==1;
}
bool saveWatchfaceAntBackgroundEnabled(bool value){
  return preferencesObject.putInt("wtfAntEn", value?1:0);
}
bool getWatchfaceRandomBackgroundEnabled(){     //123456789012345   max length = 15
  return preferencesObject.getInt("wtfRndEn", 0)==1;
}
bool saveWatchfaceRandomBackgroundEnabled(bool value){
  return preferencesObject.putInt("wtfRndEn", value?1:0);
}
bool getWatchfaceFireBackgroundEnabled(){     //123456789012345   max length = 15
  return preferencesObject.getInt("wtfFireEn", 0)==1;
}
bool saveWatchfaceFireBackgroundEnabled(bool value){
  return preferencesObject.putInt("wtfFireEn", value?1:0);
}
bool getWatchfaceLavaBackgroundEnabled(){     //123456789012345   max length = 15
  return preferencesObject.getInt("wtfLavaEn", 0)==1;
}
bool saveWatchfaceLavaBackgroundEnabled(bool value){
  return preferencesObject.putInt("wtfLavaEn", value?1:0);
}
bool getWatchfaceDotsBackgroundEnabled(){     //123456789012345   max length = 15
  return preferencesObject.getInt("wtfDotEn", 0)==1;
}
bool saveWatchfaceDotsBackgroundEnabled(bool value){
  return preferencesObject.putInt("wtfDotEn", value?1:0);
}
bool getWatchfaceCalendarEnabled(){     //123456789012345   max length = 15
  return preferencesObject.getInt(       "wtfCalendarEn", 0)==1;
}
bool saveWatchfaceCalendarEnabled(bool value){
  return preferencesObject.putInt("wtfCalendarEn", value?1:0);
}
bool getWatchfaceLegendEnabled(){     //123456789012345   max length = 15
  return preferencesObject.getInt(       "wtfLegendEn", 0)==1;
}
bool saveWatchfaceLegendEnabled(bool value){
  return preferencesObject.putInt("wtfLegendEn", value?1:0);
}

//----------------------//---------------------- ALERT -----------------//----------------------//----------------------//----------------------//----------------------
/*  bool alertIsEnabled = eepromReadAlertEnabled();
    byte alertLastRunDay = eepromReadAlertLastDayRun();
    byte alertTimeHour = eepromReadAlertHour();
    byte alertTimeMinute = eepromReadAlertMinute();
    byte alertMelodyIndex = eepromReadAlertMelodyIndex();*/

int getAlertsNumber(){
  return 6;
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
String getAlertName(int index){
  return preferencesObject.getString((String("alert")+index+"Name").c_str(), (String("Alert ")+index));
}
bool saveAlertName(int index, String newname){ 
  int savedName = preferencesObject.putString((String("alert")+index+"Name").c_str(), newname);
  return savedName>0;
}

  
//----------------------//----------------------//----------------------//----------------------//----------------------//----------------------//----------------------



int getPreferencesFreeSpace(){
  return preferencesObject.freeEntries();
}