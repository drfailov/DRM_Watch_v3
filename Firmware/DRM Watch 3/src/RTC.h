#ifndef RTC_H
#define RTC_H

String getRtcSrc();

#include "soc/rtc.h"
#include "driver/temp_sensor.h"
#include <DS3231M.h>
#include <ESP32Time.h>


bool rtcReady = false;
DS3231M_Class DS3231M;     //external RTC
ESP32Time rtcInternal;          //internal RTC
ESP32Time rtcInternalCorrected; //internal RTC Corrected

void initRtc(){
  //rtc_clk_8m_enable(true, true);
  //rtc_clk_slow_freq_set(RTC_SLOW_FREQ_8MD256);
  //rtc_clk_freq_cal();
  //rtc_clk_apb_freq_update
  Serial.print(F("DS3231M..."));
  //according to documentation, ESP32 has already timeout value 50ms
  //Wire.begin();
  //Wire.setWireTimeout(3000, true); //timeout value in uSec - SBWire uses 100 uSec, so 1000 should be OK
  rtcReady = DS3231M.begin(I2C_FAST_MODE);  //I2C_STANDARD_MODE (400khz)     I2C_FAST_MODE   (100khz)
  DS3231M.pinSquareWave();  // Make INT/SQW pin toggle at 1Hz
  Serial.println(getRtcSrc());
  Serial.print(F("RTC Chip Temp=")); Serial.println(rtcChipTemperature());
}

ESP32Time* _rtcInternal(){
  return &rtcInternal;
}
// ESP32Time* _rtcInternalCorrected(){
//   unsigned long lastSyncTime = getLastTimeSync(); //s
//   unsigned long measuredTime = _rtcInternal()->getEpoch(); //s
//   bool lastSyncValid = lastSyncTime > 1609459200;  // 1st Jan 2021 00:00:00;
//   bool measuredTimeValid = measuredTime > lastSyncTime;
//   if(!lastSyncValid || !measuredTimeValid)
//     return &rtcInternal;
//   double coefficient = getTimeCoef();
//   unsigned long sinceLastSync = measuredTime-lastSyncTime;  //s
//   double sinceLastSyncDouble = sinceLastSync;
//   double correctionDouble = sinceLastSyncDouble*coefficient;
//   double correction = correctionDouble;
//   rtcInternalCorrected.offset = correction;
//   return &rtcInternalCorrected;
// }

void printRtcGetTimeRaw(){
  //"%d %b %Y %H:%M:%S"
  if(!rtcReady){
    lcd()->print("-- NO EXT RTC --");
    return; 
  }
  DateTime datetime = DS3231M.now();
  lcd()->print(datetime.day());
  lcd()->print(".");
  lcd()->print(datetime.month());
  lcd()->print(".");
  lcd()->print(datetime.year());
  lcd()->print(" ");
  lcd()->print(datetime.hour());
  lcd()->print(":");
  lcd()->print(datetime.minute());
  lcd()->print(":");
  lcd()->print(datetime.second());
}

unsigned long rtcGetEpoch(){
  if(rtcReady){
    TimeSpan ts = TimeSpan(getTimeOffsetSec());
    DateTime datetime = DS3231M.now() + ts;
    return datetime.unixtime();
  }
  return _rtcInternal() -> getEpoch()+getTimeOffsetSec();
}
unsigned long rtcGetUtcEpoch(){
  if(rtcReady){
    DateTime datetime = DS3231M.now();
    return datetime.unixtime();
  }
  return _rtcInternal() -> getEpoch();
}
int rtcGetHour(){
  unsigned long epoch = rtcGetEpoch();
  DateTime datetime = DateTime(epoch);
  return datetime.hour();
}
int rtcGetMinute(){
  unsigned long epoch = rtcGetEpoch();
  DateTime datetime = DateTime(epoch);
  return datetime.minute();
}
int rtcGetSecond(){
  unsigned long epoch = rtcGetEpoch();
  DateTime datetime = DateTime(epoch);
  return datetime.second();
}
int rtcGetDay(){
  unsigned long epoch = rtcGetEpoch();
  DateTime datetime = DateTime(epoch);
  return datetime.day();
}
int rtcGetMonth(){ /*00-11*/
  unsigned long epoch = rtcGetEpoch();
  DateTime datetime = DateTime(epoch);
  int month = datetime.month(); //01-12
  return month-1;
}
int rtcGetYear(){
  unsigned long epoch = rtcGetEpoch();
  DateTime datetime = DateTime(epoch);
  return datetime.year();
}
int rtcGetDayOfWeek(){
  unsigned long epoch = rtcGetEpoch();
  DateTime datetime = DateTime(epoch);
  return datetime.dayOfTheWeek();
}

void rtcSetUtcEpoch(unsigned long epoch){
  if(rtcReady){
    adjustExternalRtc(epoch);
  }
  _rtcInternal() -> setTime(epoch);
  saveLastTimeSync(epoch);
}

void adjustExternalRtc(unsigned long epoch){
  if(rtcReady){
    DateTime now = DateTime(epoch);
    DS3231M.adjust(now);
  }
}
String getRtcSrc(){
  if(rtcReady)
    return "DS3231M RTC Chip";
  switch(rtc_clk_slow_freq_get()){
      case 0: return("Internal 90 kHz RC oscillator"); 
      case 1: return("External 32 kHz XTAL"); 
      case 2: return("Internal 8 MHz RC oscillator, divided by 256"); 
    }
    return "N/A";
}
bool isExternalRtcActive(){
  return rtcReady;
}

void drawDayOfWeek (int x, int y){
  lcd()->setColorIndex(white);
  lcd()->drawBox(x-3, y-16, 25, 20);
  lcd()->setFont(u8g2_font_10x20_t_cyrillic);  //ok
  lcd()->setColorIndex(black);
  lcd()->setCursor(x, y); 
  switch(rtcGetDayOfWeek()){
    case 0: lcd()->print("Нд"); break;
    case 1: lcd()->print("Пн"); break;
    case 2: lcd()->print("Вт"); break;
    case 3: lcd()->print("Ср"); break;
    case 4: lcd()->print("Чт"); break;
    case 5: lcd()->print("Пт"); break;
    case 6: lcd()->print("Сб"); break;
    case 7: lcd()->print("Нд"); break;
  }
}

void printDate(unsigned long epoch){
  DateTime datetime = DateTime(epoch);
  int day = datetime.day();
  int month = datetime.month(); //01-12
  int year = datetime.year();
  if(day<10) lcd()->print("0");
  lcd()->print(day);
  lcd()->print(".");
  if(month<10) lcd()->print("0");
  lcd()->print(month);
  lcd()->print(".");
  lcd()->print(year);
}

void drawDate(int x, int y){
  lcd()->setColorIndex(white);
  lcd()->drawBox(x-3, y-16, 106, 20);
  lcd()->setFont(u8g2_font_10x20_t_cyrillic);  //ok
  lcd()->setColorIndex(black);
  lcd()->setCursor(x, y); 
  int day = rtcGetDay();
  if(day<10) lcd()->print("0");
  lcd()->print(day);
  int month = rtcGetMonth()+1; //(rtcGetSecond()%12)+1; //for debug
  lcd()->print(".");
  if(month<10) lcd()->print("0");
  lcd()->print(month);
  lcd()->print(".");
  lcd()->print(rtcGetYear());

  if(getWatchfaceMonthEnabled()){
    switch(month){
      case 1: strcpy(buffer, "Січень"); break;
      case 2: strcpy(buffer, "Лютий"); break;
      case 3: strcpy(buffer, "Березень"); break;
      case 4: strcpy(buffer, "Квітень"); break;
      case 5: strcpy(buffer, "Травень"); break;
      case 6: strcpy(buffer, "Червень"); break;
      case 7: strcpy(buffer, "Липень"); break;
      case 8: strcpy(buffer, "Серпень"); break;
      case 9: strcpy(buffer, "Вересень"); break;
      case 10: strcpy(buffer, "Жовтень"); break;
      case 11:strcpy(buffer, "Листопад"); break;
      case 12:strcpy(buffer, "Грудень"); break;
    }
    lcd()->setFont(u8g2_font_unifont_t_cyrillic);  //ok
    int tw = lcd()->getUTF8Width(buffer);
    lcd()->setColorIndex(white);
    int mx = x+44 - tw/2;
    int my = y+13;
    lcd()->drawBox(mx-3, my-12, tw+6, 16);
    lcd()->setColorIndex(black);
    lcd()->setCursor(mx, my); 
    lcd()->print(buffer);
  }
}

unsigned long hoursFromSec(unsigned long seconds){ //how mahy hours in seconds
  unsigned long sphour = (60*60);
  return seconds / sphour;
}
unsigned long minutesFromSec(unsigned long seconds){ //how mahy minutes in seconds (excluding hours)
  unsigned long sphour = (60*60);
  unsigned long minutesPart = seconds%sphour;
  unsigned long spminute = 60;
  return minutesPart / spminute;
}


float temperature(){
  if(rtcReady)
    return rtcChipTemperature();
  else
    return cpuTemperature();
}

float rtcChipTemperature(){
  if(!rtcReady)
    return 0;
  return DS3231M.temperature() / 100.0;
}
bool temperatureInit = false;
float cpuTemperature(){
  if(!temperatureInit){
    temp_sensor_config_t temp_sensor = TSENS_CONFIG_DEFAULT();
    temp_sensor.dac_offset = TSENS_DAC_L2;  // TSENS_DAC_L2 is default; L4(-40°C ~ 20°C), L2(-10°C ~ 80°C), L1(20°C ~ 100°C), L0(50°C ~ 125°C)
    temp_sensor_set_config(temp_sensor);
    temp_sensor_start();
    float result = 0;
    delay(1);
    temp_sensor_read_celsius(&result);
    temperatureInit = true;
  }

  float result = 0;
  temp_sensor_read_celsius(&result);
  return result;
}
void resetCpuTemperatureSensor(){ //need to reset after Wi-Fi using
  temperatureInit = false;
}


void drawTemperature(int x, int y){
  
  lcd()->setColorIndex(white);
  lcd()->drawBox(x, y-1, 90, 24);

  lcd()->setFont(u8g2_font_10x20_t_cyrillic); 
  draw_ic24_temperature(x,y, black);
  lcd()->setCursor(x+24, y+18);
  lcd()->print(temperature(),1);
  lcd()->print("°C");
}


#endif