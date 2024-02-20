#include "soc/rtc.h"
#include "driver/temp_sensor.h"
#include <DS3231M.h>

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
  //Wire.setWireTimeout(3000, true); //timeout value in uSec - SBWire uses 100 uSec, so 1000 should be OK
  rtcReady = DS3231M.begin();
  DS3231M.pinSquareWave();  // Make INT/SQW pin toggle at 1Hz
  Serial.println(getRtcSrc());
  Serial.print(F("RTC Chip Temp=")); Serial.println(rtcChipTemperature());
}

ESP32Time* _rtcInternal(){
  return &rtcInternal;
}
ESP32Time* _rtcInternalCorrected(){
  unsigned long lastSyncTime = getLastTimeSync(); //s
  unsigned long measuredTime = _rtcInternal()->getEpoch(); //s
  bool lastSyncValid = lastSyncTime > 1609459200;  // 1st Jan 2021 00:00:00;
  bool measuredTimeValid = measuredTime > lastSyncTime;
  if(!lastSyncValid || !measuredTimeValid)
    return &rtcInternal;
  double coefficient = getTimeCoef();
  unsigned long sinceLastSync = measuredTime-lastSyncTime;  //s
  double sinceLastSyncDouble = sinceLastSync;
  double correctionDouble = sinceLastSyncDouble*coefficient;
  double correction = correctionDouble;
  rtcInternalCorrected.offset = correction;
  return &rtcInternalCorrected;
}

unsigned long rtcGetEpoch(){
  if(rtcReady){
    DateTime datetime = DS3231M.now();
    return datetime.unixtime();
  }
  return _rtcInternalCorrected() -> getEpoch();
}
int rtcGetHour(){
  if(rtcReady){
    DateTime datetime = DS3231M.now();
    return datetime.hour();
  }
  return _rtcInternalCorrected() -> getHour(true);
}
int rtcGetMinute(){
  if(rtcReady){
    DateTime datetime = DS3231M.now();
    return datetime.minute();
  }
  return _rtcInternalCorrected() -> getMinute();
}
int rtcGetSecond(){
  if(rtcReady){
    DateTime datetime = DS3231M.now();
    return datetime.second();
  }
  return _rtcInternalCorrected() -> getSecond();
}
int rtcGetDay(){
  if(rtcReady){
    DateTime datetime = DS3231M.now();
    return datetime.day();
  }
  return _rtcInternalCorrected() -> getDay();
}
int rtcGetMonth(){ /*00-11*/
  if(rtcReady){
    DateTime datetime = DS3231M.now();
    int month = datetime.month(); //01-12
    return month-1;
  }
  return _rtcInternalCorrected() -> getMonth();
}
int rtcGetYear(){
  if(rtcReady){
    DateTime datetime = DS3231M.now();
    return datetime.year();
  }
  return _rtcInternalCorrected() -> getYear();
}
int rtcGetDayOfWeek(){
  if(rtcReady){
    DateTime datetime = DS3231M.now();
    return datetime.dayOfTheWeek();
  }
  return _rtcInternalCorrected() -> getDayofWeek();
}
void adjustExternalRtc(unsigned long epoch){
  DateTime now = DateTime(epoch);
  DS3231M.adjust(now);
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

void drawDate(int x, int y){
  lcd()->setFont(u8g2_font_10x20_t_cyrillic);  //ok
  lcd()->setColorIndex(black);
  lcd()->setCursor(x, y); 
  lcd()->print(rtcGetDay());
  switch(rtcGetMonth()){
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
  lcd()->print(rtcGetYear());
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
  lcd()->setFont(u8g2_font_10x20_t_cyrillic); 
  draw_ic24_temperature(x,y, black);
  lcd()->setCursor(x+24, y+18);
  lcd()->print(temperature());
  lcd()->print("°C");
}
