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

void drawDayOfWeek (int x, int y){
  lcd()->setFont(u8g2_font_10x20_t_cyrillic);  //ok
  lcd()->setColorIndex(black);
  lcd()->setCursor(x, y); 
  switch(rtcCorrected()->getDayofWeek()){
    case 0: lcd()->print("Нд"); break;
    case 1: lcd()->print("Пн"); break;
    case 2: lcd()->print("Вт"); break;
    case 3: lcd()->print("Ср"); break;
    case 4: lcd()->print("Чт"); break;
    case 5: lcd()->print("Пт"); break;
    case 6: lcd()->print("Сб"); break;
  }
}

void drawDate (int x, int y){
  lcd()->setFont(u8g2_font_10x20_t_cyrillic);  //ok
  lcd()->setColorIndex(black);
  lcd()->setCursor(x, y); 
  lcd()->print(rtcCorrected()->getDay());
  switch(rtcCorrected()->getMonth()){
    case 0: lcd()->print(" Січ "); break;
    case 1: lcd()->print(" Лют "); break;
    case 2: lcd()->print(" Бер "); break;
    case 3: lcd()->print(" Кві "); break;
    case 4: lcd()->print(" Тра "); break;
    case 5: lcd()->print(" Чер "); break;
    case 6: lcd()->print(" Лип "); break;
    case 7: lcd()->print(" Сер "); break;
    case 8: lcd()->print(" Вер "); break;
    case 9: lcd()->print(" Жов "); break;
    case 10:lcd()->print(" Лис "); break;
    case 11:lcd()->print(" Гру "); break;
  }
  lcd()->print(rtcCorrected()->getYear());
}