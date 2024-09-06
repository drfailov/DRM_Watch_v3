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
int lastAddedValue = -1;



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


void ModeBatteryCalibrationLoop(){
  int raw = readSensBatteryRawFiltered();
  float volt = readSensBatteryVoltage();
  int total = batteryCalibrationLength();
  int inx = batteryCalibrationGetIndexOfValue(raw);
  unsigned long timeSinceLastAdded = millis()-valueAddLastTime;
  unsigned long calibrationTime = 0;
  if(calibrationRunning)
    calibrationTime = total*valueAddInterval+timeSinceLastAdded;

  lcd()->setColorIndex(white);
  lcd()->drawBox(0, 0, 400, 240);
  
  if(calibrationRunning)
    drawWarningFrame();

  if(calibrationRunning)
  {
    lcd()->setColorIndex(black);
    lcd()->setFont(u8g2_font_10x20_t_cyrillic); // ok
    drawCentered(L("Триває калібрування батареї...", "Battery calibration in progress..."), 200, 30);
    drawCentered(L("Не чіпайте поки не розрядиться!", "Don't touch until turnoff!"), 200, 50);
    
    lcd()->setFont(u8g2_font_unifont_t_cyrillic); //smalll
    lcd()->setCursor(10, 65); 
    lcd()->print(L("Протягом ", "During ")); 
    displayPrintSecondsAsTime(calibrationTime/1000);
    lcd()->print(L(" записано ", " added "));
    lcd()->print(total);
    lcd()->print(L(" значень.", " values."));

    lcd()->setFont(u8g2_font_unifont_t_cyrillic); //smalll
    lcd()->setCursor(10, 80); 
    lcd()->print(timeSinceLastAdded / 1000);
    lcd()->print(L("сек / ", "s / "));
    lcd()->print(valueAddInterval / 1000);
    lcd()->print(L("сек.", "s."));
  }
  else
  {
    lcd()->setColorIndex(black);
    lcd()->setFont(u8g2_font_10x20_t_cyrillic); // ok
    lcd()->drawUTF8(10, 20, L("Повністю зарядіть перед калібруванням!", "Fully charge before calibration!"));
    lcd()->drawUTF8(10, 40, L("Калібрування займе кілька годин.", "Calibration will take few hours")); 

    if(isBatteryCalibrated())
    {
      lcd()->setCursor(10, 60);
      lcd()->setFont(u8g2_font_unifont_t_cyrillic); //smalll
      lcd()->print(L("Протягом ", "During ")); 
      displayPrintSecondsAsTime((total*valueAddInterval)/1000);
      lcd()->print(L(" було записано ", " were added "));
      lcd()->print(total);
      lcd()->print(L(" значень.", " values."));
    }
    else
    {
      lcd()->setCursor(10, 60);
      lcd()->setFont(u8g2_font_unifont_t_cyrillic); //smalll
      lcd()->print(L("Каліброка не проводилась.", "No calibration data."));
    }
  }
  lcd()->setColorIndex(black);
  lcd()->setCursor(10, 95);
  lcd()->setFont(u8g2_font_unifont_t_cyrillic); //smalll
  lcd()->print("RAW: ");
  lcd()->print(raw);
  lcd()->print(", ");
  lcd()->print(((float)volt)/1000.0);
  lcd()->print("v");

  if(calibrationRunning)
    drawPlot (/*x*/10, /*y*/100, /*w*/380, /*h*/115, /*thickness*/2, /*legend*/true,  /*rangeValues*/batteryCalibration, /*values*/batteryCalibration, /*length*/batteryCalibrationLength(), /*highlight*/inx);
  else 
    drawPlot (/*x*/10, /*y*/100, /*w*/360, /*h*/125, /*thickness*/2, /*legend*/true,  /*rangeValues*/batteryCalibration, /*values*/batteryCalibration, /*length*/batteryCalibrationLength(), /*highlight*/inx);
  
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
    if(timeSinceLastAdded > valueAddInterval) // || total == 0
    {
      if(raw<lastAddedValue || lastAddedValue == -1)
      {
        valueAddLastTime = millis();
        lastAddedValue = raw;
        batteryCalibrationAddValue(raw);
      }  
    }
  }
  

}


void ModeBatteryCalibrationSelectedStartBatteryCalibration(){
  resetBatteryCalibration();
  setModeBatteryCalibration();
  calibrationRunning = true;
  backlightOff();
  drawDim();
  drawMessage(L("Почато калібровку!", "Calibration started!"), L("Не чіпайте поки не вимкнеться.", "Don't touch until turnoff."), true);
  while(calibrationRunning){
    ModeBatteryCalibrationLoop();
  }
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
