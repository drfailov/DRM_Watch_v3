#include "soc/rtc.h"
ESP32Time rtcObj;
ESP32Time rtcObjCorrected;

void initRtc(){
  //rtc_clk_8m_enable(true, true);
  //rtc_clk_slow_freq_set(RTC_SLOW_FREQ_8MD256);
  //rtc_clk_freq_cal();
  //rtc_clk_apb_freq_update
  Serial.println(getRtcSlk());
}

ESP32Time* rtc(){
  return &rtcObj;
}
ESP32Time* rtcCorrected(){
  unsigned long lastSyncTime = getLastTimeSync(); //s
  unsigned long measuredTime = rtc()->getEpoch(); //s
  bool lastSyncValid = lastSyncTime > 1609459200;  // 1st Jan 2021 00:00:00;
  bool measuredTimeValid = measuredTime > lastSyncTime;
  if(!lastSyncValid || !measuredTimeValid)
    return &rtcObj;
  double coefficient = getTimeCoef();
  unsigned long sinceLastSync = measuredTime-lastSyncTime;  //s
  double sinceLastSyncDouble = sinceLastSync;
  double correctionDouble = sinceLastSyncDouble*coefficient;
  double correction = correctionDouble;
  rtcObjCorrected.offset = correction;
  return &rtcObjCorrected;
}


String getRtcSlk(){
  switch(rtc_clk_slow_freq_get()){
      case 0: return("Internal 90 kHz RC oscillator"); 
      case 1: return("External 32 kHz XTAL"); 
      case 2: return("Internal 8 MHz RC oscillator, divided by 256"); 
    }
    return "N/A";
}