byte batteryCalibrationCnt = 4;
float batteryCalibration[][2] = { 
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



float readSensBatteryVoltage(){
  return linearInterpolate((int)readSensBatteryRaw(), batteryCalibration, batteryCalibrationCnt);
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

bool isChargerConnected(){
  return readSensUsbRaw() > 4000;
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

