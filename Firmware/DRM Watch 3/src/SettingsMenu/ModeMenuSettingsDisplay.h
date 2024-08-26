#ifndef MODEMENUSETTINGSDISPLAY_H
#define MODEMENUSETTINGSDISPLAY_H

void setModeMenuSettingsDisplay();
void ModeMenuSettingsDisplayLoop();
void ModeMenuSettingsDisplayExit();
void ModeMenuSettingsDisplayButtonCenter();

#include "../Lcd.h"
#include "../Global.h"
#include "../AutoSleep.h"
#include "../Button.h"
#include "../Icons.h"
#include "ModeMenuSettings.h"
#include "ModeMenuSettingsWatchfaceContent.h"
#include "ModeSetLcdFrequency.h"
#include "ModeSetWatchface.h"
#include "../ModeMainMenu.h"
#include "../DrmPreferences.h"
#include "../ModeListSelection.h"

const int ModeMenuSettingsDisplayItemBack = 0;
const int ModeMenuSettingsDisplayItemSelectWatchface = 1;
const int ModeMenuSettingsDisplayItemSelectWatchfaceContent = 2;
const int ModeMenuSettingsDisplayItemLcdFrequency = 3;
const int ModeMenuSettingsDisplayItemInvertDisplay = 4;
const int ModeMenuSettingsDisplayItemLanguage = 5;
const int ModeMenuSettingsDisplayItemInAnimation = 6;
const int ModeMenuSettingsDisplayItemOutAnimation = 7;
const int ModeMenuSettingsDisplayItemResetBatteryBars = 8;

int outAnimationOptionsCnt()
{
  return 6;
}
const char *outAnimationOptions(int index)
{
  if (index == 0)
    return L("Без анімації", "No animation");
  if (index == 1)
    return L("Заливка зліва направо", "Fill from left to right");
  if (index == 2)
    return L("Заливка справа наліво", "Fill from right to left");
  if (index == 3)
    return L("Коло справа наліво", "Circle from right to left");
  if (index == 4)
    return L("Глітч", "Glitch");
  if (index == 5)
    return L("Шум", "Noize");
  
  return "---";
}
void onOutAnimationSelected() // callback
{
  saveClearAnimationValue(ModeListSelection_Selected);
  setModeMenuSettingsDisplay();
}

void setModeMenuSettingsDisplay()
{
  if(modeExit != 0)
    modeExit();
  clearScreenAnimation();
  Serial.println(F("Set mode: ModeMenuSettingsDisplay"));
  modeSetup = setModeMenuSettingsDisplay;
  modeLoop = ModeMenuSettingsDisplayLoop;
  modeExit = ModeMenuSettingsDisplayExit;
  modeButtonUp = modeMainMenuButtonUp;
  modeButtonCenter = ModeMenuSettingsDisplayButtonCenter;
  modeButtonDown = modeMainMenuButtonDown;
  modeButtonUpLong = 0;
  modeButtonCenterLong = 0;
  modeButtonDownLong = 0;
  registerAction();
  enableAutoReturn = true;
  enableAutoSleep = false;
  autoReturnTime = autoReturnDefaultTime;
  autoSleepTime = autoSleepDefaultTime;
  selected = 0;
  items = 9;
}

void ModeMenuSettingsDisplayExit(){
  modeExit = 0;
}
void ModeMenuSettingsDisplayLoop()
{
  lcd()->setColorIndex(white);
  lcd()->drawBox(0, 0, 400, 240);

  lcd()->setFont(u8g2_font_10x20_t_cyrillic); // ok
  lcd()->setColorIndex(black);
  lcd()->setCursor(5, 18);
  lcd()->print(L("Параметри дисплея", "Display settigns"));

  drawMenuLegend();
  drawStatusbar(363, 1, true);

  drawListItem(ModeMenuSettingsDisplayItemBack,                     draw_ic24_back,       L("Повернутись", "Back"),                                      L("B меню налаштувань", "To settings menu"),                         firstDraw);
  drawListItem(ModeMenuSettingsDisplayItemSelectWatchface,          draw_ic24_watchface,  L("Обрати циферблат", "Select watchface"),                     L("Дизайн відображення часу", "Main screen design"),                 firstDraw);
  drawListItem(ModeMenuSettingsDisplayItemSelectWatchfaceContent,   draw_ic24_checklist,  L("Вміст циферблату", "Watchface content"),                    L("Обрати елементи циферблату", "Select elements of watchface"),     firstDraw);
  drawListItem(ModeMenuSettingsDisplayItemLcdFrequency,             draw_ic24_frequency,  L("Частота SPI", "SPI Frequency"),                             L("Частота комунікації з дисплеєм", "LCD communication frequency"),  firstDraw);
  drawListCheckbox(ModeMenuSettingsDisplayItemInvertDisplay,        draw_ic24_invert,     L("Інвертувати екран", "Invert colors"),                       L("Поміняти місцями чорне i біле", "Swap black and white colors"),   getInversionValue(), firstDraw);
  drawListItem(ModeMenuSettingsDisplayItemLanguage,                 draw_ic24_language,   L("Мова", "Language"),                                         langName(getLang()),                                                 firstDraw);
  drawListCheckbox(ModeMenuSettingsDisplayItemInAnimation,          draw_ic24_animation,  L("Анімація входу в меню", "Enter animation"),                 L("Поступова поява елементів меню", "Menu appearing gradually"),     getEnterAnimationValue(), firstDraw);
  drawListItem(ModeMenuSettingsDisplayItemOutAnimation,             draw_ic24_animation,  L("Анімація виходу з меню", "Exit animation"),                 outAnimationOptions(getClearAnimation()),                            firstDraw);
  drawListItem(ModeMenuSettingsDisplayItemResetBatteryBars,         draw_ic24_battery50,  L("Скинути калібровку батареї", "Reset battery calibration"),  L("Якщо \"Палички\" неправильні", "If shows wrong"),                 firstDraw);
  lcd()->sendBuffer();
}

void ModeMenuSettingsDisplayButtonCenter()
{
  if (selected == ModeMenuSettingsDisplayItemBack)
  {
    setModeSettingsMenu();
  }
  if (selected == ModeMenuSettingsDisplayItemSelectWatchface)
  {
    setModeSetWatchface();
    return;
  }
  if (selected == ModeMenuSettingsDisplayItemSelectWatchfaceContent)
  {
    setModeMenuSettingsWatchfaceContent();
    return;
  }
  if (selected == ModeMenuSettingsDisplayItemLcdFrequency)
  {
    setmodeSetLcdFrequencyMenu();
    return;
  }
  if (selected == ModeMenuSettingsDisplayItemInvertDisplay)
  {
    saveInversionValue(!getInversionValue());
    black = getBlackValue();
    white = getWhiteValue();
    return;
  }
  if (selected == ModeMenuSettingsDisplayItemLanguage)
  {
    int lang = getLang();
    lang ++;
    if(lang >= langCnt())
      lang = 0;
    saveLangValue(lang);
    return;
  }
  if (selected == ModeMenuSettingsDisplayItemInAnimation)
  {
    saveEnterAnimationValue(!getEnterAnimationValue());
    return;
  }
  if (selected == ModeMenuSettingsDisplayItemOutAnimation)
  {
    ModeListSelection_Items = outAnimationOptions;
    ModeListSelection_Name = L("Обрати анімацію", "Select animation");
    ModeListSelection_Cnt = outAnimationOptionsCnt();
    ModeListSelection_Selected = getClearAnimation();
    ModeListSelection_OnSelected = onOutAnimationSelected;
    setModeListSelection();
    return;
  }
  if (selected == ModeMenuSettingsDisplayItemResetBatteryBars)
  {
    resetBatteryCalibrationData();
    drawMessage(L("Калібровку батареї скинуто", "Battery calibration was reset"), L("Проведіть повний цикл для калібровки", "Run full cycle to calibrate again"), true);
    return;
  }
}

#endif