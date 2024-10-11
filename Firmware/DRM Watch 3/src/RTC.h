#ifndef RTC_H
#define RTC_H

/*PROTOTYPES*/
String getRtcSrc();
void adjustExternalRtc(unsigned long epoch);
float rtcChipTemperature();
float cpuTemperature();
int rtcGetHour();
int rtcGetMinute();
int rtcGetSecond();
int rtcGetDay();
int rtcGetMonth();
int rtcGetDayOfWeek();
int rtcGetYear();
bool isTimeValid();

#include "soc/rtc.h"
#include "driver/temp_sensor.h"
#include "DrmPreferences.h"
#include "ModeCalendar.h"
#include <DS3231M.h>
#include <ESP32Time.h>

RTC_DATA_ATTR bool rtcChipFound = false; // fills only on initial run (after reboot)
bool rtcReady = false;
DS3231M_Class DS3231M; // external RTC
ESP32Time rtcInternal; // internal RTC

void initRtc()
{
  if (esp_sleep_get_wakeup_cause() == 0 || rtcChipFound)
  { // normal wakeup try init RTC
    Serial.print(F("DS3231M..."));
    rtcReady = DS3231M.begin(I2C_FAST_MODE); // I2C_STANDARD_MODE (400khz)     I2C_FAST_MODE   (100khz)  //according to documentation, ESP32 has already timeout value 50ms
    if (esp_sleep_get_wakeup_cause() == 0)   // if initial initialization found chip, then consider it present.
      rtcChipFound = rtcReady;
    DS3231M.pinSquareWave(); // Make INT/SQW pin toggle at 1Hz
    Serial.println(getRtcSrc());
    Serial.print(F("RTC Chip Temp="));
    Serial.println(rtcChipTemperature());
  }
}

ESP32Time *_rtcInternal()
{
  return &rtcInternal;
}

void printRtcGetTimeRaw()
{
  //"%d %b %Y %H:%M:%S"
  if (!rtcReady)
  {
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
bool isTimeValid()
{
  return rtcGetYear() > 2023;
}

unsigned long rtcGetEpoch()
{
  if (rtcReady)
  {
    TimeSpan ts = TimeSpan(getTimeOffsetSec());
    DateTime datetime = DS3231M.now() + ts;
    return datetime.unixtime();
  }
  return _rtcInternal()->getEpoch() + getTimeOffsetSec();
}
unsigned long rtcGetUtcEpoch()
{
  if (rtcReady)
  {
    DateTime datetime = DS3231M.now();
    return datetime.unixtime();
  }
  return _rtcInternal()->getEpoch();
}

int rtcGetHour()
{
  unsigned long epoch = rtcGetEpoch();
  DateTime datetime = DateTime(epoch);
  return datetime.hour();
}

int rtcGetMinute()
{
  unsigned long epoch = rtcGetEpoch();
  DateTime datetime = DateTime(epoch);
  return datetime.minute();
}

int rtcGetSecond()
{
  unsigned long epoch = rtcGetEpoch();
  DateTime datetime = DateTime(epoch);
  return datetime.second();
}

int rtcGetDay()
{
  unsigned long epoch = rtcGetEpoch();
  DateTime datetime = DateTime(epoch);
  return datetime.day();
}

int rtcGetMonth()
{ /*00-11*/
  unsigned long epoch = rtcGetEpoch();
  DateTime datetime = DateTime(epoch);
  int month = datetime.month(); // 01-12
  return month - 1;
}

int rtcGetYear()
{
  unsigned long epoch = rtcGetEpoch();
  DateTime datetime = DateTime(epoch);
  return datetime.year();
}
int rtcGetDayOfWeek()
{
  unsigned long epoch = rtcGetEpoch();
  DateTime datetime = DateTime(epoch);
  return datetime.dayOfTheWeek();
}

void rtcSetUtcEpoch(unsigned long epoch)
{
  if (rtcReady)
  {
    adjustExternalRtc(epoch);
  }
  _rtcInternal()->setTime(epoch);
  saveLastTimeSync(epoch);
}

void adjustExternalRtc(unsigned long epoch)
{
  if (rtcReady)
  {
    DateTime now = DateTime(epoch);
    DS3231M.adjust(now);
  }
}
String getRtcSrc()
{
  if (rtcReady)
    return "DS3231M RTC Chip";
  switch (rtc_clk_slow_freq_get())
  {
  case 0:
    return ("Internal 90 kHz RC oscillator");
  case 1:
    return ("External 32 kHz XTAL");
  case 2:
    return ("Internal 8 MHz RC oscillator, divided by 256");
  }
  return "N/A";
}
bool isExternalRtcActive()
{
  return rtcReady;
}

void drawDayOfWeek(int x_end, int y)
{
  lcd()->setFont(u8g2_font_10x20_t_cyrillic); // ok
  const char* text = days(rtcGetDayOfWeek()-1);
  int w = lcd()->getUTF8Width(text);
  int x = x_end-w;
  lcd()->setColorIndex(white);
  lcd()->drawRBox(x - 3, y - 16, w+6, 20, getRoundness(5));
  lcd()->setColorIndex(black);
  lcd()->drawUTF8(x, y, text);
}

void printDate(unsigned long epoch)
{
  DateTime datetime = DateTime(epoch);
  int day = datetime.day();
  int month = datetime.month(); // 01-12
  int year = datetime.year();
  if (day < 10)
    lcd()->print("0");
  lcd()->print(day);
  lcd()->print(".");
  if (month < 10)
    lcd()->print("0");
  lcd()->print(month);
  lcd()->print(".");
  lcd()->print(year);
}

void drawDate(int x, int y)
{
  lcd()->setColorIndex(white);
  lcd()->drawRBox(x - 3, y - 16, 106, 20, getRoundness(5));
  lcd()->setFont(u8g2_font_10x20_t_cyrillic); // ok
  lcd()->setColorIndex(black);
  lcd()->setCursor(x, y);
  int day = rtcGetDay();
  if (day < 10)
    lcd()->print("0");
  lcd()->print(day);
  int month = rtcGetMonth() + 1; //(rtcGetSecond()%12)+1; //for debug
  lcd()->print(".");
  if (month < 10)
    lcd()->print("0");
  lcd()->print(month);
  lcd()->print(".");
  lcd()->print(rtcGetYear());

  if (getWatchfaceMonthEnabled())
  {
    strcpy(buffer, months(month-1));
    lcd()->setFont(u8g2_font_unifont_t_cyrillic); // ok
    int tw = lcd()->getUTF8Width(buffer);
    lcd()->setColorIndex(white);
    int mx = x + 44 - tw / 2;
    int my = y + 13;
    lcd()->drawRBox(mx - 3, my - 12, tw + 6, 16, getRoundness(5));
    lcd()->setColorIndex(black);
    lcd()->setCursor(mx, my);
    lcd()->print(buffer);
  }
}

unsigned long hoursFromSec(unsigned long seconds)
{ // how mahy hours in seconds
  unsigned long sphour = (60 * 60);
  return seconds / sphour;
}
unsigned long minutesFromSec(unsigned long seconds)
{ // how mahy minutes in seconds (excluding hours)
  unsigned long sphour = (60 * 60);
  unsigned long minutesPart = seconds % sphour;
  unsigned long spminute = 60;
  return minutesPart / spminute;
}

float temperature()
{
  if (rtcReady)
    return rtcChipTemperature();
  else
    return cpuTemperature();
}

float rtcChipTemperature()
{
  if (!rtcReady)
    return 0;
  return DS3231M.temperature() / 100.0;
}
bool temperatureInit = false;
float cpuTemperature()
{
  if (!temperatureInit)
  {
    temp_sensor_config_t temp_sensor = TSENS_CONFIG_DEFAULT();
    temp_sensor.dac_offset = TSENS_DAC_L2; // TSENS_DAC_L2 is default; L4(-40°C ~ 20°C), L2(-10°C ~ 80°C), L1(20°C ~ 100°C), L0(50°C ~ 125°C)
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
void resetCpuTemperatureSensor()
{ // need to reset after Wi-Fi using
  temperatureInit = false;
}

void drawTemperature(int x, int y)
{
  lcd()->setColorIndex(white);
  lcd()->drawRBox(x-4, y - 1, 90, 25, getRoundness(5));

  lcd()->setFont(u8g2_font_10x20_t_cyrillic);
  draw_ic24_temperature(x, y, black);
  lcd()->setCursor(x + 24, y + 18);
  lcd()->print(temperature(), 1);
  lcd()->print("C");
}

#endif