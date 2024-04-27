const int ModeMenuSettingsWatchfaceContentItemBack = 0;
const int ModeMenuSettingsWatchfaceContentItemShowStatusbar = 1;
const int ModeMenuSettingsWatchfaceContentItemShowAnalog = 2;
const int ModeMenuSettingsWatchfaceContentItemShowDigital = 3;
const int ModeMenuSettingsWatchfaceContentItemShowDate = 4;
const int ModeMenuSettingsWatchfaceContentItemShowDayOfWeek = 5;
const int ModeMenuSettingsWatchfaceContentItemShowLifeBackgound = 6;
const int ModeMenuSettingsWatchfaceContentItemShowCalendar = 7;
const int ModeMenuSettingsWatchfaceContentItemShowTemperature = 8;
const int ModeMenuSettingsWatchfaceContentItemShowMonth = 9;
const int ModeMenuSettingsWatchfaceContentItemShowDjiLogo = 10;
const int ModeMenuSettingsWatchfaceContentItemShowTimeInStatusbar = 11;

void setModeMenuSettingsWatchfaceContent(){
  clearScreenAnimation();
  Serial.println(F("Set mode: ModeMenuSettingsWatchfaceContent"));
  modeSetup = setModeMenuSettingsWatchfaceContent;
  modeLoop = ModeMenuSettingsWatchfaceContentLoop;
  modeButtonUp = modeMainMenuButtonUp;
  modeButtonCenter = ModeMenuSettingsWatchfaceContentButtonCenter;
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
  items = 16;
}


void ModeMenuSettingsWatchfaceContentLoop(){
  lcd()->setColorIndex(white);
  lcd()->drawBox(0, 0, 400, 240);


  lcd()->setFont(u8g2_font_10x20_t_cyrillic);  //ok
  lcd()->setColorIndex(black);
  lcd()->setCursor(5, 18); 
  lcd()->print("Вміст циферблату");

  drawMenuLegend();
  drawStatusbar(363, 1, true);

  drawListItem(ModeMenuSettingsWatchfaceContentItemBack, draw_ic24_back, "Повернутись", "В налаштування екрана", firstDraw);

  drawListCheckbox(ModeMenuSettingsWatchfaceContentItemShowStatusbar, draw_ic24_battery50, "Статус-бар", "Значки статусу годинника", getWatchfaceStatusbarEnabled(), firstDraw);
  drawListCheckbox(ModeMenuSettingsWatchfaceContentItemShowAnalog, draw_ic24_clock, "Час стрілками", "Класичний годинник", getWatchfaceAnalogEnabled(), firstDraw);
  drawListCheckbox(ModeMenuSettingsWatchfaceContentItemShowDigital, draw_ic24_watchface, "Час цифрами", "Цифровий годинник", getWatchfaceDigitalEnabled(), firstDraw);
  drawListCheckbox(ModeMenuSettingsWatchfaceContentItemShowTemperature, draw_ic24_clock, "Температура", "Температура мікросхеми RTC", getWatchfaceTemperatureEnabled(), firstDraw);
  drawListCheckbox(ModeMenuSettingsWatchfaceContentItemShowDayOfWeek, draw_ic24_clock, "День тижня", "Який зараз день тижня", getWatchfaceDayOfWeekEnabled(), firstDraw);
  drawListCheckbox(ModeMenuSettingsWatchfaceContentItemShowDate, draw_ic24_calendar, "Поточна дата", "Яка зараз дата", getWatchfaceDateEnabled(), firstDraw);
  drawListCheckbox(ModeMenuSettingsWatchfaceContentItemShowMonth, draw_ic24_calendar, "Назва місяця", "Назва місяця під датою", getWatchfaceMonthEnabled(), firstDraw);
  drawListCheckbox(ModeMenuSettingsWatchfaceContentItemShowDjiLogo, draw_ic24_about, "Лого DJI", "Назва місяця під датою", getWatchfaceDjiLogoEnabled(), firstDraw);
  drawListCheckbox(ModeMenuSettingsWatchfaceContentItemShowLifeBackgound, draw_ic24_life, "Фон \"Життя\"", "Назва місяця під датою", getWatchfaceLifeBackgroundEnabled(), firstDraw);
  drawListCheckbox(ModeMenuSettingsWatchfaceContentItemShowCalendar, draw_ic24_calendar, "Календар", "Назва місяця під датою", getWatchfaceCalendarEnabled(), firstDraw);
  drawListCheckbox(ModeMenuSettingsWatchfaceContentItemShowTimeInStatusbar, draw_ic24_clock, "Час в статус-барі", "Назва місяця під датою", getWatchfaceStatusbarDigitalEnabled(), firstDraw);

  drawListCheckbox(ModeMenuSettingsWatchfaceContentItemShowTimeInStatusbar, draw_ic24_about, "Легента кнопок", "Легенда кнопок", getWatchfaceStatusbarDigitalEnabled(), firstDraw);
  drawListCheckbox(ModeMenuSettingsWatchfaceContentItemShowTimeInStatusbar, draw_ic24_random, "Фон \"Шум\"", "Легенда кнопок", getWatchfaceStatusbarDigitalEnabled(), firstDraw);
  drawListCheckbox(ModeMenuSettingsWatchfaceContentItemShowTimeInStatusbar, draw_ic24_clock, "Фон \"Мураха\"", "Легенда кнопок", getWatchfaceStatusbarDigitalEnabled(), firstDraw);
  drawListCheckbox(ModeMenuSettingsWatchfaceContentItemShowTimeInStatusbar, draw_ic24_clock, "Фон \"Точки\"", "Легенда кнопок", getWatchfaceStatusbarDigitalEnabled(), firstDraw);


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
}





