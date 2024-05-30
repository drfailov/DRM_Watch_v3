#ifndef BATTERY_H
#define BATTERY_H

/*PROTOTYPES*/
float linearInterpolate(float raw, float calibrationData[][2], byte tableLength);
bool isChargerConnected();
void onChargerDisconnected();
void onChargerConnected();
int analogSmoothRead(int pin);
int readSensUsbRaw();
int readSensBatteryRaw();
void drawBattery(int x, int y);
float readSensBatteryVoltage();
byte batteryBars();


#include "Icons.h"
#include "Buzzer.h"
#include <Arduino.h>

byte batteryCalibrationCnt = 4;
float batteryCalibration[][2] = {    //raw analog reading to voltage in mv
  {0, 0}, //{mv, raw}
  {3000, 4650}, //{mv, raw}
  {4200, 6550}, //{mv, raw}
  {5000, 8000} //{mv, raw}
};

void drawBattery(int x, int y){
  if(isChargerConnected()){
    float voltage = readSensBatteryVoltage();
    if(voltage > 4110)
      draw_ic24_battery_full(x, y, black);
    else
      draw_ic24_battery_charging(x, y, black);
  }
  else{
    int bars = batteryBars();
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


RTC_DATA_ATTR float previousVoltage = 3600; //mv
float readSensBatteryVoltage(){
  float currentVoltage = linearInterpolate((int)readSensBatteryRaw(), batteryCalibration, batteryCalibrationCnt);
  previousVoltage += (currentVoltage-previousVoltage) * 0.1;  //smoothing to prevent jumps
  return previousVoltage;
}

byte batteryBars(){
  int voltage = readSensBatteryVoltage();
  byte level = 0;
  if (voltage >= 3350) level = 1;
  if (voltage >= 3650) level = 2;
  if (voltage >= 3850) level = 3;
  if (voltage >= 3950) level = 4;
  return level;
}

bool isBatteryCritical(){
  return batteryBars() == 0;
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


int analogSmoothRead(int pin){
  int sum=0;
  int cnt=5;
  for(int i=0; i<cnt; i++)
    sum += analogRead(pin);
  return sum/cnt;
}




#endif