#ifndef ModeTestBatteryAnalysis_H
#define ModeTestBatteryAnalysis_H

/*PROTOTYPES*/
void setModeTestBatteryAnalysis();
void ModeTestBatteryAnalysisLoop();
void ModeTestBatteryAnalysisButtonUp();
void ModeTestBatteryAnalysisButtonCenter();
void ModeTestBatteryAnalysisButtonDown();

#include <Arduino.h>
#include "Global.h"
#include "AutoSleep.h"
#include "Button.h"
#include "Battery.h"
#include "DrmPreferences.h"
#include "ModeTest.h"



void setModeTestBatteryAnalysis()
{
  if (modeExit != 0)
    modeExit();
  clearScreenAnimation();
  Serial.println(F("Set mode: TestBatteryAnalysis"));
  modeSetup = setModeTestBatteryAnalysis;
  modeLoop = ModeTestBatteryAnalysisLoop;
  modeButtonUp = ModeTestBatteryAnalysisButtonUp;
  modeButtonCenter = ModeTestBatteryAnalysisButtonCenter;
  modeButtonDown = ModeTestBatteryAnalysisButtonDown;
  modeButtonUpLong = 0;
  modeButtonCenterLong = 0;
  modeButtonDownLong = 0;
  registerAction();
  enableAutoReturn = false;
  enableAutoSleep = false;
  autoReturnTime = autoReturnDefaultTime;
  autoSleepTime = autoSleepDefaultTime;
}

#define ModeTestBatteryAnalysisHistorySize 100
RTC_DATA_ATTR int16_t ModeTestBatteryAnalysisHistoryRaw[ModeTestBatteryAnalysisHistorySize];
RTC_DATA_ATTR int16_t ModeTestBatteryAnalysisHistoryDeviation[ModeTestBatteryAnalysisHistorySize];
RTC_DATA_ATTR int16_t ModeTestBatteryAnalysisHistoryLimitDeviation[ModeTestBatteryAnalysisHistorySize];
RTC_DATA_ATTR int16_t ModeTestBatteryAnalysisHistoryFiltered[ModeTestBatteryAnalysisHistorySize];
RTC_DATA_ATTR bool    ModeTestBatteryAnalysisTestSleep = false;

void ModeTestBatteryAnalysisLoop()
{
  // this mechanism is DUPLICATED here.
  float raw = readSensBatteryRaw();
  float volt = getBatteryVoltage(raw);
  float deviation = abs(raw - battery_averageRaw);
  float limitDeviation = battery_averageDeviation * 2;
  float filtered = readSensBatteryRawFiltered(raw);

  for (int i = 1; i < ModeTestBatteryAnalysisHistorySize; i++)
  {
    ModeTestBatteryAnalysisHistoryRaw[i - 1] = ModeTestBatteryAnalysisHistoryRaw[i];
    ModeTestBatteryAnalysisHistoryDeviation[i - 1] = ModeTestBatteryAnalysisHistoryDeviation[i];
    ModeTestBatteryAnalysisHistoryLimitDeviation[i - 1] = ModeTestBatteryAnalysisHistoryLimitDeviation[i];
    ModeTestBatteryAnalysisHistoryFiltered[i - 1] = ModeTestBatteryAnalysisHistoryFiltered[i];
  }
  ModeTestBatteryAnalysisHistoryRaw[ModeTestBatteryAnalysisHistorySize - 1] = raw;
  ModeTestBatteryAnalysisHistoryDeviation[ModeTestBatteryAnalysisHistorySize - 1] = deviation;
  ModeTestBatteryAnalysisHistoryLimitDeviation[ModeTestBatteryAnalysisHistorySize - 1] = limitDeviation;
  ModeTestBatteryAnalysisHistoryFiltered[ModeTestBatteryAnalysisHistorySize - 1] = filtered;

  lcd()->setColorIndex(white);
  lcd()->drawBox(0, 0, 400, 240);

  lcd()->setColorIndex(black);
  lcd()->setFont(u8g2_font_10x20_t_cyrillic); // ok
  lcd()->drawUTF8(10, 16, L("Акумулятор", "Battery"));

  lcd()->setCursor(130, 16);
  lcd()->setFont(u8g2_font_unifont_t_cyrillic); // smalll
  lcd()->print("RAW:");
  lcd()->print((int)raw);
  lcd()->print(", AVG:");
  lcd()->print((int)battery_averageRaw);
  lcd()->print(", ");
  lcd()->print(((float)volt) / 1000.0);
  lcd()->print("v, ");
  lcd()->print((int)batteryCalibrationGetValuePercent(raw));
  lcd()->print("%");

  lcd()->setFont(u8g2_font_unifont_t_cyrillic); // smalll
  lcd()->drawUTF8(10, 38, "Deviation / Limit deviation");
  drawPlot<int16_t>(/*x*/ 10, /*y*/ 41, /*w*/ 360, /*h*/ 85, /*thickness*/ 2, /*legend*/ true, /*rangeValues*/ ModeTestBatteryAnalysisHistoryDeviation, /*values*/ ModeTestBatteryAnalysisHistoryDeviation, /*length*/ ModeTestBatteryAnalysisHistorySize, /*highlight*/ -1);
  drawPlot<int16_t>(/*x*/ 10, /*y*/ 41, /*w*/ 360, /*h*/ 85, /*thickness*/ 0, /*legend*/ false, /*rangeValues*/ ModeTestBatteryAnalysisHistoryDeviation, /*values*/ ModeTestBatteryAnalysisHistoryLimitDeviation, /*length*/ ModeTestBatteryAnalysisHistorySize, /*highlight*/ -1);

  lcd()->drawUTF8(10, 148, "Filtered RAW / Non-filtered RAW");
  drawPlot<int16_t>(/*x*/ 10, /*y*/ 151, /*w*/ 360, /*h*/ 85, /*thickness*/ 3, /*legend*/ true, /*rangeValues*/ ModeTestBatteryAnalysisHistoryRaw, /*values*/ ModeTestBatteryAnalysisHistoryFiltered, /*length*/ ModeTestBatteryAnalysisHistorySize, /*highlight*/ -1);
  drawPlot<int16_t>(/*x*/ 10, /*y*/ 151, /*w*/ 360, /*h*/ 85, /*thickness*/ 0, /*legend*/ false, /*rangeValues*/ ModeTestBatteryAnalysisHistoryRaw, /*values*/ ModeTestBatteryAnalysisHistoryRaw, /*length*/ ModeTestBatteryAnalysisHistorySize, /*highlight*/ -1);

  draw_ic16_arrow_up(lx(), ly1(), black);
  draw_ic16_back(lx(), ly2(), black);
  if(ModeTestBatteryAnalysisTestSleep)
    draw_ic16_check(lx(), ly3(), black);
  else
    draw_ic16_unlock(lx(), ly3(), black);
  
  lcd()->sendBuffer();

  if(ModeTestBatteryAnalysisTestSleep)
  {
    if(isButtonDownPressed()){
      ModeTestBatteryAnalysisTestSleep = false;
      return;
    }
    esp_sleep_enable_timer_wakeup(1 * 1000000ULL);
    esp_deep_sleep_start();
  }
}

void ModeTestBatteryAnalysisButtonUp()
{
  setModeTest();
}

void ModeTestBatteryAnalysisButtonCenter()
{
  setModeAppsMenu();
}

void ModeTestBatteryAnalysisButtonDown()
{
  ModeTestBatteryAnalysisTestSleep = true;
}

#endif
