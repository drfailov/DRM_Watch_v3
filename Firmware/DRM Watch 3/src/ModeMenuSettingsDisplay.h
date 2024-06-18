#ifndef MODEMENUSETTINGSDISPLAY_H
#define MODEMENUSETTINGSDISPLAY_H

void setModeMenuSettingsDisplay();
void ModeMenuSettingsDisplayLoop();
void ModeMenuSettingsDisplayButtonCenter();

#include "Lcd.h"
#include "Global.h"
#include "AutoSleep.h"
#include "Button.h"
#include "Icons.h"
#include "ModeMenuSettings.h"
#include "ModeMenuSettingsWatchfaceContent.h"
#include "ModeSetLcdFrequency.h"
#include "ModeSetWatchface.h"
#include "ModeMainMenu.h"
#include "DrmPreferences.h"


const int ModeMenuSettingsDisplayItemBack = 0;
const int ModeMenuSettingsDisplayItemSelectWatchface = 1;
const int ModeMenuSettingsDisplayItemSelectWatchfaceContent = 2;
const int ModeMenuSettingsDisplayItemLcdFrequency = 3;
const int ModeMenuSettingsDisplayItemInvertDisplay = 4;
const int ModeMenuSettingsDisplayItemInAnimation = 5;

void setModeMenuSettingsDisplay(){
  clearScreenAnimation();
  Serial.println(F("Set mode: ModeMenuSettingsDisplay"));
  modeSetup = setModeMenuSettingsDisplay;
  modeLoop = ModeMenuSettingsDisplayLoop;
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
  items = 6;
}


void ModeMenuSettingsDisplayLoop(){
  lcd()->setColorIndex(white);
  lcd()->drawBox(0, 0, 400, 240);

  lcd()->setFont(u8g2_font_10x20_t_cyrillic);  //ok
  lcd()->setColorIndex(black);
  lcd()->setCursor(5, 18); 
  lcd()->print("Параметри дисплея");

  drawMenuLegend();
  drawStatusbar(363, 1, true);

  drawListItem(ModeMenuSettingsDisplayItemBack, draw_ic24_back, "Повернутись", "B меню налаштувань", firstDraw); //
  drawListItem(ModeMenuSettingsDisplayItemSelectWatchface, draw_ic24_watchface, "Обрати циферблат", "Дизайн відображення часу", firstDraw); //
  drawListItem(ModeMenuSettingsDisplayItemSelectWatchfaceContent, draw_ic24_checklist, "Вміст циферблату", "Чи показувати елементи циферблатів", firstDraw); //
  drawListItem(ModeMenuSettingsDisplayItemLcdFrequency, draw_ic24_frequency, "Частота SPI", "Частота комунікації з дисплеєм", firstDraw); //
  drawListCheckbox(ModeMenuSettingsDisplayItemInvertDisplay, draw_ic24_invert, "Інвертувати екран", "Поміняти місцями чорне i біле", getInversionValue(), firstDraw); //
  drawListCheckbox(ModeMenuSettingsDisplayItemInAnimation, draw_ic24_animation, "Анімація входу в меню", "Поступова поява елементів меню", getEnterAnimationValue(), firstDraw); //

  lcd()->sendBuffer();
}

void ModeMenuSettingsDisplayButtonCenter(){
  if(selected == ModeMenuSettingsDisplayItemBack){
      setModeSettingsMenu();
  }
  if(selected == ModeMenuSettingsDisplayItemSelectWatchface){
    setModeSetWatchface();
    return;
  }
  if(selected == ModeMenuSettingsDisplayItemSelectWatchfaceContent){
    setModeMenuSettingsWatchfaceContent();
    return;
  }
  if(selected == ModeMenuSettingsDisplayItemLcdFrequency){
    setmodeSetLcdFrequencyMenu();
    return;
  }
  if(selected == ModeMenuSettingsDisplayItemInvertDisplay){
    saveInversionValue(!getInversionValue());
    black = getBlackValue();
    white = getWhiteValue();
    return;
  }  
  if(selected == ModeMenuSettingsDisplayItemInAnimation){
    saveEnterAnimationValue(!getEnterAnimationValue());
    return;
  }  
}





#endif