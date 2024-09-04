#ifndef ModeBatteryCalibration_H
#define ModeBatteryCalibration_H

/*PROTOTYPES*/
void setModeBatteryCalibration();
void ModeBatteryCalibrationLoop();
void ModeBatteryCalibrationButtonUp();
void ModeBatteryCalibrationButtonCenter();
void ModeBatteryCalibrationButtonDown();

#include <Arduino.h>
#include "Global.h"
#include "AutoSleep.h"
#include "Button.h"
#include "Battery.h"
#include "DrmPreferences.h"

bool calibrationRunning = false;
unsigned long valueAddLastTime = 0;
unsigned long valueAddInterval = 1000*60*5; //5m
//unsigned long valueAddInterval = 1000*10; //10s



void setModeBatteryCalibration(){
  if(modeExit != 0)
    modeExit();
  clearScreenAnimation();
  Serial.println(F("Set mode: Battery Calibration"));
  modeSetup = setModeBatteryCalibration;
  modeLoop = ModeBatteryCalibrationLoop;
  modeButtonUp = ModeBatteryCalibrationButtonUp;
  modeButtonCenter = ModeBatteryCalibrationButtonCenter;
  modeButtonDown = ModeBatteryCalibrationButtonDown;
  modeButtonUpLong = 0;
  modeButtonCenterLong = 0;
  modeButtonDownLong = 0;
  registerAction();
  enableAutoReturn = false;
  enableAutoSleep = false; 
  autoReturnTime = autoReturnDefaultTime;
  autoSleepTime = autoSleepDefaultTime;
  calibrationRunning = false;
  valueAddLastTime = 0;
}

void drawPlot(int x, int y, int w, int h, int16_t* values, int length, int highlightIndex)
{
  //config
  int padding = 15;

  //frame
  lcd()->setColorIndex(white);
  lcd()->drawBox(x, y, w, h);
  lcd()->setColorIndex(black);
  lcd()->drawFrame(x, y, w, h);
  lcd()->drawFrame(x-1, y-1, w+2, h+2);

  if(length == 0)
  {
    lcd()->setFont(u8g2_font_10x20_t_cyrillic); // ok
    strcpy(buffer, L("Немає даних", "No data"));
    int width = lcd()->getUTF8Width(buffer);
    lcd()->setCursor((W - 30 - width) / 2, y+h/2+5);
    lcd()->print(buffer);
    return;
  }
  
  //min and max
  int16_t min = values[0];
  int16_t max = values[0];
  for(int i = 1; i < length; i++){
    if(values[i] < min)
      min = values[i];
    if(values[i] > max)
      max = values[i];
  }
  lcd()->setFont(u8g2_font_unifont_t_cyrillic); //smalll
  lcd()->setCursor(x+2, y+12);
  lcd()->print(max);
  lcd()->setCursor(x+2, y+h-2);
  lcd()->print(min);
  
  //plot line
  float maxPoint = y+padding;
  float minPoint = y+h-padding;
  float leftPoint = x;
  float rightPoint = x+w;
  float lastX = map(0, 0, length, leftPoint, rightPoint);
  float lastY = map(values[0], min, max, minPoint, maxPoint);
  for(int i = 1; i < length; i++){
    float cx = map(i, 0, length, leftPoint, rightPoint);
    float cy = map(values[i], min, max, minPoint, maxPoint);
    lcd()->drawLine(cx, cy, lastX, lastY);
    lcd()->drawLine(cx, cy+1, lastX, lastY+1);
    lastX = cx;
    lastY = cy;
  }

  //highlight mark
  if(highlightIndex >= 0 && highlightIndex < length){
    float cy = map(values[highlightIndex], min, max, minPoint, maxPoint);
    float cx = map(highlightIndex, 0, length, leftPoint, rightPoint);
    drawDashedLine(cx, y+padding, cx, y+h-padding*2-1, 1);
    lcd()->drawDisc(cx, cy, 3);
  }
}

void ModeBatteryCalibrationLoop(){
  int raw = readSensBatteryRaw();
  int total = batteryCalibrationLength();
  int inx = batteryCalibrationGetIndexOfValue(raw);
  unsigned long timeSinceLastAdded = millis()-valueAddLastTime;
  unsigned long calibrationTime = 0;
  if(calibrationRunning)
    calibrationTime = total*valueAddInterval+timeSinceLastAdded;

  lcd()->setColorIndex(white);
  lcd()->drawBox(0, 0, 400, 240);
  lcd()->setColorIndex(black);
  if(calibrationRunning)
  {
    lcd()->setFont(u8g2_font_10x20_t_cyrillic); // ok
    lcd()->drawUTF8(10, 20, L("Триває калібрування батареї...", "Battery calibration in progress..."));
    lcd()->drawUTF8(10, 40, L("Не чіпайте поки не розрядиться!", "Don't touch until turnoff!"));
    lcd()->setFont(u8g2_font_unifont_t_cyrillic); //smalll
    
    lcd()->setCursor(10, 60); 
    lcd()->print(L("Протягом ", "During ")); 
    displayPrintSecondsAsTime(calibrationTime/1000);
    lcd()->print(L(" записано ", " added "));
    lcd()->print(total);
    lcd()->print(L(" значень.", " values."));

    lcd()->setCursor(10, 75); 
    lcd()->print(timeSinceLastAdded / 1000);
    lcd()->print(L("сек / ", "s / "));
    lcd()->print(valueAddInterval / 1000);
    lcd()->print(L("сек.", "s."));
  }
  else
  {
    lcd()->setFont(u8g2_font_10x20_t_cyrillic); // ok
    lcd()->drawUTF8(10, 20, L("Повністю зарядіть перед калібруванням!", "Fully charge before calibration!"));
    lcd()->drawUTF8(10, 40, L("Калібрування займе кілька годин.", "Calibration will take few hours")); 
    lcd()->setFont(u8g2_font_unifont_t_cyrillic); //smalll
    lcd()->setCursor(10, 60); 
    if(isBatteryCalibrated())
    {
      lcd()->print(L("Протягом ", "During ")); 
      displayPrintSecondsAsTime((total*valueAddInterval)/1000);
      lcd()->print(L(" було записано ", " were added "));
      lcd()->print(total);
      lcd()->print(L(" значень.", " values."));
    }
    else
    {
      lcd()->print(L("Каліброка не проводилась.", "No calibration data."));
    }
    //lcd()->print("isBatteryCalibrated(): "); lcd()->print(isBatteryCalibrated());
    //lcd()->print(", Total: "); lcd()->print(total);
  }
  
  lcd()->setFont(u8g2_font_unifont_t_cyrillic); //smalll

  //lcd()->setCursor(10, 50); 
  //lcd()->print("isBatteryCalibrated(): "); lcd()->print(isBatteryCalibrated());
  //lcd()->print(", Total: "); lcd()->print(total);

  //lcd()->setCursor(10, 65);
  //lcd()->print("RAW: "); lcd()->print(raw);
  //lcd()->print(", index: "); lcd()->print(inx);
  
  //lcd()->setCursor(10, 80); 
  //lcd()->print("timeSinceLastAdded: "); lcd()->print(timeSinceLastAdded);
  
  //lcd()->setCursor(10, 95); 
  //lcd()->print("calibrationTime: "); displayPrintSecondsAsTime(calibrationTime/1000);

  drawPlot(10, 100, 360, 125, batteryCalibration, batteryCalibrationLength(), /*highlight*/inx);

  // y+=interval; lcd()->setCursor(x, y); lcd()->print("Internal RTC: "); lcd()->print(_rtcInternal()->getTime("%d %b %Y %H:%M:%S"));  lcd()->print(" ("); lcd()->print(_rtcInternal()->getEpoch());   lcd()->print(")");
  // y+=interval; lcd()->setCursor(x, y); lcd()->print("External RTC: "); printRtcGetTimeRaw();   
  // y+=interval; lcd()->setCursor(x, y); lcd()->print("SinceLastSync :"); displayPrintSecondsAsTime(rtcGetUtcEpoch()-getLastTimeSync());  lcd()->print(" Last sync:"); lcd()->print(getLastTimeSync());   
  // y+=interval; lcd()->setCursor(x, y); lcd()->print("RTC CLK SRC: "); lcd()->print(getRtcSrc());

  // y+=interval; lcd()->setCursor(x, y); lcd()->print("Wakeup_reason: "); wakeup_reason();
  // y+=interval; lcd()->setCursor(x, y); lcd()->print("Millis:"); lcd()->print(millis()); lcd()->print(",   Don't sleep: "); lcd()->print(dontSleep);

  // y+=interval; lcd()->setCursor(x, y); lcd()->print("RAW ");  lcd()->print("BATTERY: "); lcd()->print(readSensBatteryRaw()); lcd()->print(", USB:"); lcd()->print(readSensUsbRaw()); lcd()->print(" : "); lcd()->print(isChargerConnected());
  // y+=interval; lcd()->setCursor(x, y); lcd()->print("VOLTAGE ");  lcd()->print("BATTERY: "); lcd()->print(readSensBatteryVoltage()); lcd()->print(" ("); lcd()->print(batteryBars()); lcd()->print(" bars)"); 
  // y+=interval; lcd()->setCursor(x, y); lcd()->print("MIN voltage: ");  lcd()->print(getBatteryMinVoltage());  lcd()->print(", MAX voltage: ");  lcd()->print(getBatteryMaxVoltage()); 
  // y+=interval; lcd()->setCursor(x, y); lcd()->print("SinceLastCharged: "); displayPrintSecondsAsTime(getTimeSinceLastCharged());

  // y+=interval; lcd()->setCursor(x, y); lcd()->print("Preferences remaining memory: "); lcd()->print(getPreferencesFreeSpace());
  // y+=interval; lcd()->setCursor(x, y); lcd()->print("RAM State: "); lcd()->print(esp_get_free_heap_size());  lcd()->print(",  ");  lcd()->print(ESP.getFreeHeap()); //RAM diagnosis

  //y+=interval; lcd()->setCursor(x, y); lcd()->print("MAC: "); printMacAddress();
  //uint32_t getBusClock(void);
  //void setBusClock(uint32_t clock_speed);
  
  if(!calibrationRunning)
  {
    draw_ic16_arrow_right(lx(), ly1(), black);
    draw_ic16_back(lx(), ly2(), black);
    draw_ic16_repeat(lx(), ly3(), black);
  }
  lcd()->sendBuffer();

  if(calibrationRunning){
    if(isChargerConnected()){
      calibrationRunning = false;
      //resetBatteryCalibration();
      drawDim();
      drawMessage(L("Калібровка була припинена!", "Calibration stopped!"), L("Відключіть зарядний пристрій.", "Disconnect charger."), true);
      waitOk();

      return;
    }
   if(timeSinceLastAdded > valueAddInterval){
      valueAddLastTime = millis();
      batteryCalibrationAddValue(raw);
    }
  }
  

}


void ModeBatteryCalibrationSelectedStartBatteryCalibration(){
  resetBatteryCalibration();
  setModeBatteryCalibration();
  calibrationRunning = true;
  drawDim();
  drawMessage(L("Почато калібровку!", "Calibration started!"), L("Не чіпайте поки не вимкнеться.", "Don't touch until turnoff."), true);
}
void ModeBatteryCalibrationButtonUp(){
  if(calibrationRunning)
    return;
  //goToSleep();
  //switchDontSleep();
  //batteryCalibrationAddValue(readSensBatteryRaw());
  if(isChargerConnected()){
    drawDim();
    drawMessageAnimated(L("Спершу відключіть зарядку.", "Disconnect charger first."));
    waitOk();
    return;
  }
  questionModeSet(L("Почати калібровку?", "Start battery calibration?"), L("Цей процей триватиме кілька годин!","Calibration will take few hours!"), ModeBatteryCalibrationSelectedStartBatteryCalibration, setModeBatteryCalibration);
}

void ModeBatteryCalibrationButtonCenter(){
  if(calibrationRunning)
    return;
  //setModeAppsMenu();
  setModeMenuSettingsDisplay();
  //wifiOff();
}

void ModeBatteryCalibrationSelectedResetBatteryCalibration(){
  resetBatteryCalibration();
  drawDim();
  drawMessageAnimated(L("Калібровку батареї скинуто.", "Battery calibration resetted."));
  waitOk();
  setModeBatteryCalibration();
}
void ModeBatteryCalibrationButtonDown(){
  if(calibrationRunning)
    return;
  questionModeSet(L("Скинути калібровку?", "Reset calibration data?"), L("Буде необхідно перекалібрувати!","Calibration will be required!"), ModeBatteryCalibrationSelectedResetBatteryCalibration, setModeBatteryCalibration);
}






//----------------------//----------------------//----------------------//----------------------//----------------------//----------------------//----------------------



#endif
