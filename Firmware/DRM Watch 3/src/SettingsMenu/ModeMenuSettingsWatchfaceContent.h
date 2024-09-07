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

  drawListCheckbox(ModeMenuSettingsWatchfaceContentItemShowStatusbar,         draw_ic24_battery50,       L("Статус-бар", "Status bar"),                L("Значки статусу годинника", "Watch status icons"),            getWatchfaceStatusbarEnabled(), firstDraw);
  drawListCheckbox(ModeMenuSettingsWatchfaceContentItemShowAnalog,            draw_ic24_clock,           L("Час стрілками", "Analog clock"),           L("Класичний годинник", "Classic round clock"),                 getWatchfaceAnalogEnabled(), firstDraw);
  drawListCheckbox(ModeMenuSettingsWatchfaceContentItemShowDigital,           draw_ic24_digitalclock,    L("Час цифрами", "Digital clock"),            L("Цифровий годинник", "Digital clock"),                        getWatchfaceDigitalEnabled(), firstDraw);
  drawListCheckbox(ModeMenuSettingsWatchfaceContentItemShowTemperature,       draw_ic24_temperature,     L("Температура", "Temperature"),              L("Температура мікросхеми RTC", "RTC Chip temperature"),        getWatchfaceTemperatureEnabled(), firstDraw);
  drawListCheckbox(ModeMenuSettingsWatchfaceContentItemShowDayOfWeek,         draw_ic24_weekday,         L("День тижня", "Day of week"),               L("Який зараз день тижня", "Current day of week"),              getWatchfaceDayOfWeekEnabled(), firstDraw);
  drawListCheckbox(ModeMenuSettingsWatchfaceContentItemShowDate,              draw_ic24_date,            L("Поточна дата", "Date"),                    L("Яка зараз дата", "Current date"),                            getWatchfaceDateEnabled(), firstDraw);
  drawListCheckbox(ModeMenuSettingsWatchfaceContentItemShowMonth,             draw_ic24_date,            L("Назва місяця", "Month name"),              L("Назва місяця під датою", "Name of month"),                   getWatchfaceMonthEnabled(), firstDraw);
  drawListCheckbox(ModeMenuSettingsWatchfaceContentItemShowDjiLogo,           draw_ic24_dji,             L("Лого DJI", "DJI Logo"),                    L("У мене всюди їх лого, навіть тут", "I have it everywhere"),  getWatchfaceDjiLogoEnabled(), firstDraw);
  drawListCheckbox(ModeMenuSettingsWatchfaceContentItemShowLifeBackgound,     draw_ic24_life,            L("Фон \"Життя\"", "Background \"Life\""),    L("Convey's Game Of Life", "Convey's Game Of Life"),            getWatchfaceLifeBackgroundEnabled(), firstDraw);
  drawListCheckbox(ModeMenuSettingsWatchfaceContentItemShowRandomBackgound,   draw_ic24_random,          L("Фон \"Шум\"", "Background \"Noize\""),     L("Випадковий шум на фоні", "Random noize on background"),      getWatchfaceRandomBackgroundEnabled(), firstDraw);
  drawListCheckbox(ModeMenuSettingsWatchfaceContentItemShowAntBackgound,      draw_ic24_ant2,            L("Фон \"Mypaxa\"", "Background \"Ant\""),    L("Мураха Ленгтона на фоні", "Lengton's ant on background"),    getWatchfaceAntBackgroundEnabled(), firstDraw);
  drawListCheckbox(ModeMenuSettingsWatchfaceContentItemShowDotsBackgound,     draw_ic24_pixels,          L("Фон \"Точки\"", "Background \"Dots\""),    L("Щокадру додаються токи", "Adds some dots every frame"),      getWatchfaceDotsBackgroundEnabled(), firstDraw);
  drawListCheckbox(ModeMenuSettingsWatchfaceContentItemShowFireBackgound,     draw_ic24_fire,            L("Фон \"Вогонь\"", "Background \"Fire\""),   L("Красива симуляція вогню", "File simulation"),                getWatchfaceFireBackgroundEnabled(), firstDraw);
  drawListCheckbox(ModeMenuSettingsWatchfaceContentItemShowLavaBackgound,     draw_ic24_bubbles,         L("Фон \"Лава\"", "Background \"Lava\""),     L("Симуляція лава-лампи", "Lava lamp simulation"),              getWatchfaceLavaBackgroundEnabled(), firstDraw);
  drawListCheckbox(ModeMenuSettingsWatchfaceContentItemShowCalendar,          draw_ic24_calendar,        L("Календар", "Calendar"),                    L("Ha поточний місяць", "For current month"),                   getWatchfaceCalendarEnabled(), firstDraw);
  drawListCheckbox(ModeMenuSettingsWatchfaceContentItemShowTimeInStatusbar,   draw_ic24_digitalclock2,   L("Час в статус-бapi", "Clock in statusbar"), L("Серед інконок статусу ще й час", "Add clock to statusbar"),  getWatchfaceStatusbarDigitalEnabled(), firstDraw);
  drawListCheckbox(ModeMenuSettingsWatchfaceContentItemShowSinceCharged,      draw_ic24_battery100,      L("Днів від зарядки", "Days from charge"),    L("Скільки днів тримає заряд", "How many days charge lasts"),   getWatchfaceSinceChargedEnabled(), firstDraw);


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