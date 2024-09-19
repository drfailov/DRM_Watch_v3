#ifndef BATTERY_H
#define BATTERY_H

/*PROTOTYPES*/
float linearInterpolate(float raw, float calibrationData[][2], byte tableLength);
bool isChargerConnected();
bool isBatteryLow();
bool isBatteryCritical();
void onChargerDisconnected();
void onChargerConnected();
int analogSmoothRead(int pin);
int readSensUsbRaw();
int readSensBatteryRaw();
int readSensBatteryRawFiltered(float raw);
int readSensBatteryRawFiltered();
void drawBattery(int x, int y);
float getBatteryVoltage();
float getBatteryVoltage(int raw);
byte getBatteryBars();

#include "Icons.h"
#include "Buzzer.h"
#include <Arduino.h>

byte batteryDefaultCalibrationCnt = 4;
float batteryDefaultCalibration[][2] = {    //raw analog reading to voltage in mv
  {0, 0}, //{mv, raw}
  {3000, 4650}, //{mv, raw}
  {4200, 6550}, //{mv, raw}
  {5000, 8000} //{mv, raw}
};

void drawBattery(int x, int y){
  if(isChargerConnected()){
      draw_ic24_battery_charging(x, y, black);
  }
  else{
    int bars = getBatteryBars();
    if(bars == 4) 
      draw_ic24_battery100(x, y, black);
    if(bars == 3) 
      draw_ic24_battery75(x, y, black);
    if(bars == 2) 
      draw_ic24_battery50(x, y, black);
    if(bars == 1) 
      draw_ic24_battery25(x, y, black);
    if(bars == 0) 
      draw_ic24_battery0(x, y, black);
  }
  
}


float getBatteryVoltage()
{
  return getBatteryVoltage(readSensBatteryRawFiltered());
}
float getBatteryVoltage(int raw)
{
  return linearInterpolate(raw, batteryDefaultCalibration, batteryDefaultCalibrationCnt);
}

RTC_DATA_ATTR int shownBarsPercentLast = -1;
RTC_DATA_ATTR int shownBarsPercentHysteresis = 15;
byte getBatteryBars()
{
  int raw = readSensBatteryRawFiltered();
  
  if(isBatteryCalibrated())
  {
    float percent = batteryCalibrationGetValuePercent(raw);
    float percentChange = abs(shownBarsPercentLast-percent);
    if(shownBarsPercentLast!=-1 && percentChange<shownBarsPercentHysteresis)   //Prevent jitter - ignore small changes
      percent = shownBarsPercentLast;
    shownBarsPercentLast = percent;
    if (percent >= 77) return 4; 
    if (percent >= 55) return 3; 
    if (percent >= 32) return 2; 
    if (percent >= 10) return 1; 
    return 0;
  }

  int voltage = getBatteryVoltage(raw);
  if (voltage >= 3950) return 4; 
  if (voltage >= 3850) return 3; 
  if (voltage >= 3650) return 2; 
  if (voltage >= 3350) return 1; 
  return 0;
}

bool isBatteryLow()
{
  for(int i=0; i<2; i++) ////multi check to prevent glitches
  {
    if(getBatteryBars() > 4)
      return false;
    delay(5);  
  }
  return true;
}

bool isBatteryCritical()
{
  for(int i=0; i<3; i++) ////multi check to prevent glitches
  {
    if(getBatteryBars() > 0)
      return false;
    delay(5);  
  }
  return true;
}

RTC_DATA_ATTR bool previousState = false;
bool isChargerConnected(){
  bool result = readSensUsbRaw() > 7000; //of 8096
  if(!result && previousState)
    onChargerDisconnected();
  if(result && !previousState)
    onChargerConnected();
  previousState = result;
  return result;
}

void onChargerDisconnected(){
  buzPlayChargerDisconnectedTone();
  registerAction();
  saveLastChargedTime();
}

void onChargerConnected(){
  buzPlayChargerConnectedTone();
  registerAction();
}

RTC_DATA_ATTR float battery_rawFilteredSmoothed = -1;
RTC_DATA_ATTR float battery_averageRaw = -1;
RTC_DATA_ATTR float battery_averageDeviation = -1;
int readSensBatteryRawFiltered(float raw)
{
  if(battery_averageRaw == -1)
    battery_averageRaw = raw;
  float deviation = abs(raw-battery_averageRaw);
  if(battery_averageDeviation == -1)
    battery_averageDeviation = deviation;
  float limitDeviation = battery_averageDeviation*2;
  float filtered = raw;
  if(deviation > limitDeviation)
    filtered = battery_averageRaw;

  battery_averageRaw += (raw-battery_averageRaw)*0.1;
  battery_averageDeviation += (deviation-battery_averageDeviation)*0.1;
  
  if(battery_rawFilteredSmoothed == -1)
    battery_rawFilteredSmoothed = filtered;
  battery_rawFilteredSmoothed += (filtered-battery_rawFilteredSmoothed)*0.3;
  return (int)battery_rawFilteredSmoothed;
}
int readSensBatteryRawFiltered()
{
  float raw = (int)readSensBatteryRaw();
  return readSensBatteryRawFiltered(raw);
}

int readSensBatteryRaw(){
  return analogSmoothRead(SENS_BATERY_PIN);
}

int readSensUsbRaw(){
  return analogSmoothRead(SENS_USB_PIN);
}








//----------------------//----------------------//----------------------//----------------------//----------------------//----------------------//----------------------

float linearInterpolate(float raw, float calibrationData[][2], byte tableLength){
  float x0, x1, y0, y1;
  for (int i = 0; i < tableLength-1; i++)
  {
    if (raw > calibrationData[i][1] && raw < calibrationData[i + 1][1])
    {
      y0 = calibrationData[i][1];  //lower bound
      y1 = calibrationData[i + 1][1]; //upper bound
      x0 = calibrationData[i][0];
      x1 = calibrationData[i + 1][0];
      return (x0 + ((x1 - x0) * ((raw - y0) / (y1 - y0))));
    }
  }
  return 0;
}


int analogSmoothRead(int pin)
{
  while(millis()<2); //assuming first few ms of wake is not reliable reading
  int sum=0;
  int cnt=6;
  for(int i=0; i<cnt; i++)
    sum += analogRead(pin);
  return sum/cnt;
}




#endif