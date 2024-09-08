#ifndef modeSetLcdFrequency_H
#define modeSetLcdFrequency_H

/*PROTOTYPES*/
void setmodeSetLcdFrequencyMenu();
void modeSetLcdFrequencyMenuLoop();
void modeSetLcdFrequencyMenuButtonUp();
void modeSetLcdFrequencyMenuButtonCenter();
void modeSetLcdFrequencyMenuButtonDown();

#include "../Global.h"
#include "../AutoSleep.h"
#include "../Button.h"
#include "../GlobalMenu.h"
#include "../DrmPreferences.h"

void setmodeSetLcdFrequencyMenu(){
  clearScreenAnimation();
  Serial.println(F("Set mode: set LCD Frequency"));
  modeSetup = setmodeSetLcdFrequencyMenu;
  modeLoop = modeSetLcdFrequencyMenuLoop;
  modeButtonUp = modeSetLcdFrequencyMenuButtonUp;
  modeButtonCenter = modeSetLcdFrequencyMenuButtonCenter;
  modeButtonDown = modeSetLcdFrequencyMenuButtonDown;
  modeButtonUpLong = 0;
  modeButtonCenterLong = 0;
  modeButtonDownLong = 0;
  registerAction();
  enableAutoReturn = true;
  enableAutoSleep = false; 
  autoReturnTime = autoReturnDefaultTime;
  autoSleepTime = autoSleepDefaultTime;
}

void modeSetLcdFrequencyMenuLoop(){
  lcd()->setColorIndex(white);
  lcd()->drawBox(0, 0, 400, 240);

  lcd()->setFont(u8g2_font_10x20_t_cyrillic);  //ok
  lcd()->setColorIndex(black);
  lcd()->setCursor(5, 18); 
  lcd()->print(L("Частота SPI дисплея", "LCD SPI Frequency"));

  drawStatusbar(363, 1, true);
  drawMenuLegend();
  draw_ic16_back(lx(), ly2(), black);
  draw_ic16_plus(lx(), ly1(), black);
  draw_ic16_minus(lx(), ly3(), black);
  
  unsigned long frequency = getLcdSpiSpeed();
  String text = "";
  text += frequency;
  text += " Hz";
  drawTextFrame(/*index*/0, /*text*/text.c_str(), /*name*/L("Обрати частоту SPI дисплея", "Select SPI frequency"), /*editMode*/false, /*animate*/firstDraw, /*x*/30, /*y*/35, /*width*/308);
  lcd()->setFont(u8g2_font_10x20_t_cyrillic);
  int y=90; int h = 17;
  lcd()->setCursor(10, y+=h); lcd()->print(L("За замовчуванням: 2 000 000 Hz.", "Default:  2 000 000 Hz."));
  lcd()->setCursor(10, y+=h); lcd()->print(L("Чим більша частота SPI, тим швидше", "Higher frequency - Faster watch"));
  lcd()->setCursor(10, y+=h); lcd()->print(L("працюватиме годинник.", "will work."));
  lcd()->setCursor(10, y+=h); lcd()->print(L("Оптимальне значення індивідуальне.", "Optimal value is individual."));
  lcd()->setCursor(10, y+=h); lcd()->print(L("Забагато=пошкодження зображення.", "Too high=damaged image."));
  lcd()->setCursor(10, y+=h); lcd()->print(L("Замало-повільна робота годинника.", "Too low=slow work."));
  lcd()->sendBuffer();
}

void modeSetLcdFrequencyMenuButtonUp(){
  if(getLcdSpiSpeed() > 9000000)
    return;
  saveLcdSpiSpeed(getLcdSpiSpeed()+10000);
  lcd()-> setBusClock(getLcdSpiSpeed());
}

void modeSetLcdFrequencyMenuButtonDown(){
  if(getLcdSpiSpeed() < 30000)
    return;
  saveLcdSpiSpeed(getLcdSpiSpeed()-10000);
  lcd()-> setBusClock(getLcdSpiSpeed());
}

void modeSetLcdFrequencyMenuButtonCenter(){
    setModeMenuSettingsDisplay();
}

#endif