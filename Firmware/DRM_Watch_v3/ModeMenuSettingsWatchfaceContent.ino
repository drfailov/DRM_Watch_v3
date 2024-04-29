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
const int ModeMenuSettingsWatchfaceContentItemShowCalendar = 11;
const int ModeMenuSettingsWatchfaceContentItemShowTemperature = 12;
const int ModeMenuSettingsWatchfaceContentItemShowDjiLogo = 13;
const int ModeMenuSettingsWatchfaceContentItemShowTimeInStatusbar = 14;
const int ModeMenuSettingsWatchfaceContentItemShowLegend = 15;

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
  drawListCheckbox(ModeMenuSettingsWatchfaceContentItemShowDigital, draw_ic24_digitalclock, "Час цифрами", "Цифровий годинник", getWatchfaceDigitalEnabled(), firstDraw);
  drawListCheckbox(ModeMenuSettingsWatchfaceContentItemShowTemperature, draw_ic24_temperature, "Температура", "Температура мікросхеми RTC", getWatchfaceTemperatureEnabled(), firstDraw);
  drawListCheckbox(ModeMenuSettingsWatchfaceContentItemShowDayOfWeek, draw_ic24_weekday, "День тижня", "Який зараз день тижня", getWatchfaceDayOfWeekEnabled(), firstDraw);
  drawListCheckbox(ModeMenuSettingsWatchfaceContentItemShowDate, draw_ic24_date, "Поточна дата", "Яка зараз дата", getWatchfaceDateEnabled(), firstDraw);
  drawListCheckbox(ModeMenuSettingsWatchfaceContentItemShowMonth, draw_ic24_date, "Назва місяця", "Назва місяця під датою", getWatchfaceMonthEnabled(), firstDraw);
  drawListCheckbox(ModeMenuSettingsWatchfaceContentItemShowDjiLogo, draw_ic24_dji, "Лого DJI", "У мене всюди їх лого, навіть тут", getWatchfaceDjiLogoEnabled(), firstDraw);
  drawListCheckbox(ModeMenuSettingsWatchfaceContentItemShowLifeBackgound, draw_ic24_life, "Фон \"Життя\"", "Convey's Game Of Life", getWatchfaceLifeBackgroundEnabled(), firstDraw);
  drawListCheckbox(ModeMenuSettingsWatchfaceContentItemShowRandomBackgound, draw_ic24_random, "Фон \"Шум\"", "Випадковий шум на фоні", getWatchfaceRandomBackgroundEnabled(), firstDraw);
  drawListCheckbox(ModeMenuSettingsWatchfaceContentItemShowAntBackgound, draw_ic24_ant, "Фон \"Мураха\"", "Мураха Ленгтона на фоні", getWatchfaceAntBackgroundEnabled(), firstDraw);
  drawListCheckbox(ModeMenuSettingsWatchfaceContentItemShowDotsBackgound, draw_ic24_random, "Фон \"Точки\"", "Щокадру додається 1 точка на фоні", getWatchfaceDotsBackgroundEnabled(), firstDraw);
  drawListCheckbox(ModeMenuSettingsWatchfaceContentItemShowCalendar, draw_ic24_calendar, "Календар", "На поточний місяць", getWatchfaceCalendarEnabled(), firstDraw);
  drawListCheckbox(ModeMenuSettingsWatchfaceContentItemShowTimeInStatusbar, draw_ic24_digitalclock2, "Час в статус-барі", "Серед інконок статусу ще й час", getWatchfaceStatusbarDigitalEnabled(), firstDraw);
  drawListCheckbox(ModeMenuSettingsWatchfaceContentItemShowLegend, draw_ic24_about, "Легента кнопок", "Значки що роблять кнопки", getWatchfaceLegendEnabled(), firstDraw);


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
  if(selected==ModeMenuSettingsWatchfaceContentItemShowLegend){
    saveWatchfaceLegendEnabled(!getWatchfaceLegendEnabled());
    return;
  }
}





