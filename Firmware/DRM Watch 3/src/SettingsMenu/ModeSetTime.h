#ifndef MODESETTIME_H
#define MODESETTIME_H

void setModeSetTimeMenu();
void exitModeSetTimeMenu();
void ModeSetTimeMenuLoop();
void ModeSetTimeMenuButtonUp();
void ModeSetTimeMenuButtonCenter();
void ModeSetTimeMenuButtonDown();

#include "../Global.h"
#include "../AutoSleep.h"
#include "../Button.h"
#include "../GlobalMenu.h"
#include "../ModeWatchface.h"
#include "../RTC.h"

int modeSetTimeMenuIndex = 0;
bool modeSetTimeMenuEditMode = false;

const int itemmodeSetTimeMenuBack = 0;
const int itemmodeSetTimeMenuHour = 1;
const int itemmodeSetTimeMenuMinute = 2;
const int itemmodeSetTimeMenuSecond = 3;
const int itemmodeSetTimeMenuDay = 4;
const int itemmodeSetTimeMenuMonth = 5;
const int itemmodeSetTimeMenuYear = 6;

void setModeSetTimeMenu()
{
  if(modeExit != 0)
    modeExit();
  clearScreenAnimation();
  Serial.println(F("Set mode: set time"));
  modeSetup = setModeSetTimeMenu;
  modeExit = exitModeSetTimeMenu;
  modeLoop = ModeSetTimeMenuLoop;
  modeButtonUp = ModeSetTimeMenuButtonUp;
  modeButtonCenter = ModeSetTimeMenuButtonCenter;
  modeButtonDown = ModeSetTimeMenuButtonDown;
  modeButtonUpLong = 0;
  modeButtonCenterLong = 0;
  modeButtonDownLong = 0;
  registerAction();
  enableAutoReturn = true;
  enableAutoSleep = false;
  autoReturnTime = autoReturnDefaultTime;
  autoSleepTime = autoSleepDefaultTime;
  selected = 0;
  items = 7;
  disableAlertsTemporarily = true;
}
void exitModeSetTimeMenu()
{
  disableAlertsTemporarily = false;
}

void ModeSetTimeMenuLoop()
{
  lcd()->setColorIndex(white);
  lcd()->drawBox(0, 0, 400, 240);

  drawStatusbar(363, 1, true);

  lcd()->setFont(u8g2_font_10x20_t_cyrillic); // ok
  lcd()->setColorIndex(black);
  lcd()->setCursor(5, 18);
  lcd()->print(L("Налаштування часу", "Set time"));

  drawMenuLegend();
  if (modeSetTimeMenuEditMode)
  {
    draw_ic16_plus(lx(), ly1(), black);
    draw_ic16_minus(lx(), ly3(), black);
  }

  drawMenuItem(itemmodeSetTimeMenuBack, draw_ic24_back, L("Назад", "Back"), firstDraw, 30, 32);

  lcd()->setFont(u8g2_font_10x20_t_cyrillic); // ok
  lcd()->setColorIndex(black);
  lcd()->setCursor(5, 100);
  lcd()->print(L("Введіть поточний місцевий час", "Enter current local time"));

  lcd()->setFont(u8g2_font_10x20_t_cyrillic); // ok
  lcd()->setColorIndex(black);
  lcd()->setCursor(22, 136);
  lcd()->print(L("Час", "Time"));
  drawNumberFrame(/*index*/ itemmodeSetTimeMenuHour, /*number*/ rtcGetHour(), /*name*/ L("Обрати годину", "Select hour"), /*editMode*/ modeSetTimeMenuEditMode, /*animate*/ firstDraw, /*x*/ 68, /*y*/ 110, /*Width*/ 82);
  drawNumberFrame(/*index*/ itemmodeSetTimeMenuMinute, /*number*/ rtcGetMinute(), /*name*/ L("Обрати хвилину", "Select minute"), /*editMode*/ modeSetTimeMenuEditMode, /*animate*/ firstDraw, /*x*/ 168, /*y*/ 110, /*Width*/ 82);
  drawNumberFrame(/*index*/ itemmodeSetTimeMenuSecond, /*number*/ rtcGetSecond(), /*name*/ L("Обрати секунду", "Select second"), /*editMode*/ modeSetTimeMenuEditMode, /*animate*/ firstDraw, /*x*/ 268, /*y*/ 110, /*Width*/ 82);

  lcd()->setFont(u8g2_font_10x20_t_cyrillic); // ok
  lcd()->setColorIndex(black);
  lcd()->setCursor(22, 186);
  lcd()->print(L("Дата", "Date"));
  drawNumberFrame(/*index*/ itemmodeSetTimeMenuDay, /*number*/ rtcGetDay(), /*name*/ L("Обрати день", "Select day"), /*editMode*/ modeSetTimeMenuEditMode, /*animate*/ firstDraw, /*x*/ 68, /*y*/ 160, /*Width*/ 82);
  drawNumberFrame(/*index*/ itemmodeSetTimeMenuMonth, /*number*/ rtcGetMonth()+1, /*name*/ L("Обрати місяць", "Select month"), /*editMode*/ modeSetTimeMenuEditMode, /*animate*/ firstDraw, /*x*/ 168, /*y*/ 160, /*Width*/ 82);
  drawNumberFrame(/*index*/ itemmodeSetTimeMenuYear, /*number*/ rtcGetYear(), /*name*/ L("Обрати рік", "Select year"), /*editMode*/ modeSetTimeMenuEditMode, /*animate*/ firstDraw, /*x*/ 268, /*y*/ 160, /*Width*/ 82);

  lcd()->sendBuffer();
}

void ModeSetTimeMenuButtonCenter()
{
  if (selected == itemmodeSetTimeMenuBack)
  {
    setModeMenuSettingsTime();
    return;
  }
  else
  {
    modeSetTimeMenuEditMode = !modeSetTimeMenuEditMode;
    return;
  }
}
void ModeSetTimeMenuButtonUp()
{
  if (!modeSetTimeMenuEditMode)
  {
    globalMenuButtonUp();
    return;
  }
  unsigned long epoch = rtcGetUtcEpoch();
  if (selected == itemmodeSetTimeMenuYear)
    epoch += 60 * 60 * 24 * 365;
  if (selected == itemmodeSetTimeMenuMonth)
    epoch += 60 * 60 * 24 * 30;
  if (selected == itemmodeSetTimeMenuDay)
    epoch += 60 * 60 * 24;
  if (selected == itemmodeSetTimeMenuHour)
    epoch += 60 * 60;
  if (selected == itemmodeSetTimeMenuMinute)
    epoch += 60;
  if (selected == itemmodeSetTimeMenuSecond)
    epoch += 1;

  rtcSetUtcEpoch(epoch);
}
void ModeSetTimeMenuButtonDown()
{
  if (!modeSetTimeMenuEditMode)
  {
    globalMenuButtonDown();
    return;
  }
  unsigned long epoch = rtcGetUtcEpoch();
  if (selected == itemmodeSetTimeMenuYear)
    epoch -= 60 * 60 * 24 * 365;
  if (selected == itemmodeSetTimeMenuMonth)
    epoch -= 60 * 60 * 24 * 30;
  if (selected == itemmodeSetTimeMenuDay)
    epoch -= 60 * 60 * 24;
  if (selected == itemmodeSetTimeMenuHour)
    epoch -= 60 * 60;
  if (selected == itemmodeSetTimeMenuMinute)
    epoch -= 60;
  if (selected == itemmodeSetTimeMenuSecond)
    epoch -= 1;
  rtcSetUtcEpoch(epoch);
}

#endif