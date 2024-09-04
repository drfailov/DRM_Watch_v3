#ifndef MODEMENUSETTINGWATCHFACECONTENT_H
#define MODEMENUSETTINGWATCHFACECONTENT_H

void setModeMenuSettingsWatchfaceContent();
void ModeMenuSettingsWatchfaceContentLoop();
void ModeMenuSettingsWatchfaceContentButtonCenter();

#include "../Global.h"
#include "../AutoSleep.h"
#include "../Button.h"
#include "../GlobalMenu.h"


const int ModeMenuSettingsWatchfaceContentItemBack = 0;
const int ModeMenuSettingsWatchfaceContentItemShowStatusbar = 1;
const int ModeMenuSettingsWatchfaceContentItemShowAnalog = 2;
const int ModeMenuSettingsWatchfaceContentItemShowDigital = 3;
const int ModeMenuSettingsWatchfaceContentItemShowDate = 4;
const int ModeMenuSettingsWatchfaceContentItemShowMonth = 5;
const int ModeMenuSettingsWatchfaceContentItemShowDayOfWeek = 6;
const int ModeMenuSettingsWatchfaceContentItemShowLifeBackgound = 7;
const int ModeMenuSettingsWatchfaceContentItemShowRandomBackgound = 8;
const int ModeMenuSettingsWatchfaceContentItemShowAntBackgound = 9;
const int ModeMenuSettingsWatchfaceContentItemShowDotsBackgound = 10;
const int ModeMenuSettingsWatchfaceContentItemShowFireBackgound = 11;
const int ModeMenuSettingsWatchfaceContentItemShowLavaBackgound = 12;
const int ModeMenuSettingsWatchfaceContentItemShowCalendar = 13;
const int ModeMenuSettingsWatchfaceContentItemShowTemperature = 14;
const int ModeMenuSettingsWatchfaceContentItemShowDjiLogo = 15;
const int ModeMenuSettingsWatchfaceContentItemShowTimeInStatusbar = 16;
const int ModeMenuSettingsWatchfaceContentItemShowSinceCharged = 17;

void setModeMenuSettingsWatchfaceContent(){
  clearScreenAnimation();
  Serial.println(F("Set mode: ModeMenuSettingsWatchfaceContent"));
  modeSetup = setModeMenuSettingsWatchfaceContent;
  modeLoop = ModeMenuSettingsWatchfaceContentLoop;
  modeButtonUp = globalMenuButtonUp;
  modeButtonCenter = ModeMenuSettingsWatchfaceContentButtonCenter;
  modeButtonDown = globalMenuButtonDown;
  modeButtonUpLong = 0;
  modeButtonCenterLong = 0;
  modeButtonDownLong = 0;
  registerAction();
  enableAutoReturn = true;
  enableAutoSleep = false; 
  autoReturnTime = autoReturnDefaultTime;
  autoSleepTime = autoSleepDefaultTime;
  selected = 0;
  items = 18;
}


void ModeMenuSettingsWatchfaceContentLoop(){
  lcd()->setColorIndex(white);
  lcd()->drawBox(0, 0, 400, 240);


  lcd()->setFont(u8g2_font_10x20_t_cyrillic);  //ok
  lcd()->setColorIndex(black);
  lcd()->setCursor(5, 18); 
  lcd()->print(L("Вміст циферблату", "Watchface content"));

  drawMenuLegend();
  drawStatusbar(363, 1, true);

  drawListItem(ModeMenuSettingsWatchfaceContentItemBack, draw_ic24_back, L("Повернутись", "Back"), L("В налаштування екрана", "To display settings"), firstDraw);

  drawListCheckbox(ModeMenuSettingsWatchfaceContentItemShowStatusbar,         draw_ic24_battery50,       L("Статус-бар", "Status bar"),             "Значки статусу годинника", getWatchfaceStatusbarEnabled(), firstDraw);
  drawListCheckbox(ModeMenuSettingsWatchfaceContentItemShowAnalog,            draw_ic24_clock,           L("Час стрілками", "Analog clock"),        "Класичний годинник", getWatchfaceAnalogEnabled(), firstDraw);
  drawListCheckbox(ModeMenuSettingsWatchfaceContentItemShowDigital,           draw_ic24_digitalclock,    L("Час цифрами", "Digital clock"),         "Цифровий годинник", getWatchfaceDigitalEnabled(), firstDraw);
  drawListCheckbox(ModeMenuSettingsWatchfaceContentItemShowTemperature,       draw_ic24_temperature,     L("Температура", "Temperature"),           "Температура мікросхеми RTC", getWatchfaceTemperatureEnabled(), firstDraw);
  drawListCheckbox(ModeMenuSettingsWatchfaceContentItemShowDayOfWeek,         draw_ic24_weekday,         L("День тижня", "Day of week"),            "Який зараз день тижня", getWatchfaceDayOfWeekEnabled(), firstDraw);
  drawListCheckbox(ModeMenuSettingsWatchfaceContentItemShowDate,              draw_ic24_date,            L("Поточна дата", "Date"),                 "Яка зараз дата", getWatchfaceDateEnabled(), firstDraw);
  drawListCheckbox(ModeMenuSettingsWatchfaceContentItemShowMonth,             draw_ic24_date,            L("Назва місяця", "Month name"),           "Назва місяця під датою", getWatchfaceMonthEnabled(), firstDraw);
  drawListCheckbox(ModeMenuSettingsWatchfaceContentItemShowDjiLogo,           draw_ic24_dji,             L("Лого DJI", "DJI Logo"),                 "У мене всюди їх лого, навіть тут", getWatchfaceDjiLogoEnabled(), firstDraw);
  drawListCheckbox(ModeMenuSettingsWatchfaceContentItemShowLifeBackgound,     draw_ic24_life,            L("Фон \"Життя\"", "Background \"Life\""), "Convey's Game Of Life", getWatchfaceLifeBackgroundEnabled(), firstDraw);
  drawListCheckbox(ModeMenuSettingsWatchfaceContentItemShowRandomBackgound,   draw_ic24_random,          L("Фон \"Шум\"", "Background \"Noize\""),  "Випадковий шум на фоні", getWatchfaceRandomBackgroundEnabled(), firstDraw);
  drawListCheckbox(ModeMenuSettingsWatchfaceContentItemShowAntBackgound,      draw_ic24_ant2,            "Фон \"Mypaxa\"", "Мураха Ленгтона на фоні", getWatchfaceAntBackgroundEnabled(), firstDraw);
  drawListCheckbox(ModeMenuSettingsWatchfaceContentItemShowDotsBackgound,     draw_ic24_pixels,          "Фон \"Точки\"", "Щокадру додається 1 точка на фоні", getWatchfaceDotsBackgroundEnabled(), firstDraw);
  drawListCheckbox(ModeMenuSettingsWatchfaceContentItemShowFireBackgound,     draw_ic24_fire,            "Фон \"Вогонь\"", "Красива симуляція вогню", getWatchfaceFireBackgroundEnabled(), firstDraw);
  drawListCheckbox(ModeMenuSettingsWatchfaceContentItemShowLavaBackgound,     draw_ic24_bubbles,         "Фон \"Лава\"", "Симуляція лава-лампи", getWatchfaceLavaBackgroundEnabled(), firstDraw);
  drawListCheckbox(ModeMenuSettingsWatchfaceContentItemShowCalendar,          draw_ic24_calendar,        "Календар", "Ha поточний місяць", getWatchfaceCalendarEnabled(), firstDraw);
  drawListCheckbox(ModeMenuSettingsWatchfaceContentItemShowTimeInStatusbar,   draw_ic24_digitalclock2,   "Час в статус-бapi", "Серед інконок статусу ще й час", getWatchfaceStatusbarDigitalEnabled(), firstDraw);
  drawListCheckbox(ModeMenuSettingsWatchfaceContentItemShowSinceCharged,      draw_ic24_battery100,      "Днів від зарядки", "Скільки днів тримає заряд", getWatchfaceSinceChargedEnabled(), firstDraw);


  lcd()->sendBuffer();
}

void ModeMenuSettingsWatchfaceContentButtonCenter(){
  if(selected == ModeMenuSettingsWatchfaceContentItemBack){
      setModeMenuSettingsDisplay();
  }
  if(selected==ModeMenuSettingsWatchfaceContentItemShowStatusbar){
    saveWatchfaceStatusbarEnabled(!getWatchfaceStatusbarEnabled());
    return;
  }
  if(selected==ModeMenuSettingsWatchfaceContentItemShowAnalog){
    saveWatchfaceAnalogEnabled(!getWatchfaceAnalogEnabled());
    return;
  }
  if(selected==ModeMenuSettingsWatchfaceContentItemShowDigital){
    saveWatchfaceDigitalEnabled(!getWatchfaceDigitalEnabled());
    return;
  }
  if(selected==ModeMenuSettingsWatchfaceContentItemShowTemperature){
    saveWatchfaceTemperatureEnabled(!getWatchfaceTemperatureEnabled());
    return;
  }
  if(selected==ModeMenuSettingsWatchfaceContentItemShowDayOfWeek){
    saveWatchfaceDayOfWeekEnabled(!getWatchfaceDayOfWeekEnabled());
    return;
  }
  if(selected==ModeMenuSettingsWatchfaceContentItemShowDate){
    saveWatchfaceDateEnabled(!getWatchfaceDateEnabled());
    return;
  }
  if(selected==ModeMenuSettingsWatchfaceContentItemShowMonth){
    saveWatchfaceMonthEnabled(!getWatchfaceMonthEnabled());
    return;
  }
  if(selected==ModeMenuSettingsWatchfaceContentItemShowDjiLogo){
    saveWatchfaceDjiLogoEnabled(!getWatchfaceDjiLogoEnabled());
    return;
  }
  if(selected==ModeMenuSettingsWatchfaceContentItemShowLifeBackgound){
    saveWatchfaceLifeBackgroundEnabled(!getWatchfaceLifeBackgroundEnabled());
    return;
  }
  if(selected==ModeMenuSettingsWatchfaceContentItemShowCalendar){
    saveWatchfaceCalendarEnabled(!getWatchfaceCalendarEnabled());
    return;
  }
  if(selected==ModeMenuSettingsWatchfaceContentItemShowTimeInStatusbar){
    saveWatchfaceStatusbarDigitalEnabled(!getWatchfaceStatusbarDigitalEnabled());
    return;
  }
  if(selected==ModeMenuSettingsWatchfaceContentItemShowAntBackgound){
    saveWatchfaceAntBackgroundEnabled(!getWatchfaceAntBackgroundEnabled());
    return;
  }
  if(selected==ModeMenuSettingsWatchfaceContentItemShowRandomBackgound){
    saveWatchfaceRandomBackgroundEnabled(!getWatchfaceRandomBackgroundEnabled());
    return;
  }
  if(selected==ModeMenuSettingsWatchfaceContentItemShowDotsBackgound){
    saveWatchfaceDotsBackgroundEnabled(!getWatchfaceDotsBackgroundEnabled());
    return;
  }
  if(selected==ModeMenuSettingsWatchfaceContentItemShowFireBackgound){
    saveWatchfaceFireBackgroundEnabled(!getWatchfaceFireBackgroundEnabled());
    return;
  }
  if(selected==ModeMenuSettingsWatchfaceContentItemShowLavaBackgound){
    saveWatchfaceLavaBackgroundEnabled(!getWatchfaceLavaBackgroundEnabled());
    return;
  }
  if(selected==ModeMenuSettingsWatchfaceContentItemShowSinceCharged){
    saveWatchfaceSinceChargedEnabled(!getWatchfaceSinceChargedEnabled());
    return;
  }
}





#endif