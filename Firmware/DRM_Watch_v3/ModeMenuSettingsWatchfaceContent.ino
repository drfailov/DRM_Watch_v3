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
  items = 11;
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

  if(getWatchfaceStatusbarEnabled()) drawListItem(ModeMenuSettingsWatchfaceContentItemShowStatusbar, draw_ic24_check2, "Статус-бар", "Зараз відображатиметься", firstDraw);
  else drawListItem(ModeMenuSettingsWatchfaceContentItemShowStatusbar, draw_ic24_cancel, "Статус-бар", "Зараз вимкнено", firstDraw);

  if(getWatchfaceAnalogEnabled()) drawListItem(ModeMenuSettingsWatchfaceContentItemShowAnalog, draw_ic24_check2, "Час стрілками", "Зараз відображатиметься", firstDraw);
  else drawListItem(ModeMenuSettingsWatchfaceContentItemShowAnalog, draw_ic24_cancel, "Час стрілками", "Зараз вимкнено", firstDraw);

  if(getWatchfaceDigitalEnabled()) drawListItem(ModeMenuSettingsWatchfaceContentItemShowDigital, draw_ic24_check2, "Час цифрами", "Зараз відображатиметься", firstDraw);
  else drawListItem(ModeMenuSettingsWatchfaceContentItemShowDigital, draw_ic24_cancel, "Час цифрами", "Зараз вимкнено", firstDraw);

  if(getWatchfaceTemperatureEnabled()) drawListItem(ModeMenuSettingsWatchfaceContentItemShowTemperature, draw_ic24_check2, "Температура годинника", "Зараз відображатиметься", firstDraw);
  else drawListItem(ModeMenuSettingsWatchfaceContentItemShowTemperature, draw_ic24_cancel, "Температура годинника", "Зараз вимкнено", firstDraw);

  if(getWatchfaceDayOfWeekEnabled()) drawListItem(ModeMenuSettingsWatchfaceContentItemShowDayOfWeek, draw_ic24_check2, "День тижня", "Зараз відображатиметься", firstDraw);
  else drawListItem(ModeMenuSettingsWatchfaceContentItemShowDayOfWeek, draw_ic24_cancel, "День тижня", "Зараз вимкнено", firstDraw);

  if(getWatchfaceDateEnabled()) drawListItem(ModeMenuSettingsWatchfaceContentItemShowDate, draw_ic24_check2, "Поточна дата", "Зараз відображатиметься", firstDraw);
  else drawListItem(ModeMenuSettingsWatchfaceContentItemShowDate, draw_ic24_cancel, "Поточна дата", "Зараз вимкнено", firstDraw);

  if(getWatchfaceMonthEnabled()) drawListItem(ModeMenuSettingsWatchfaceContentItemShowMonth, draw_ic24_check2, "Назва місяця", "Відображатиметься з датою", firstDraw);
  else drawListItem(ModeMenuSettingsWatchfaceContentItemShowMonth, draw_ic24_cancel, "Назва місяця", "Зараз вимкнено", firstDraw);

  if(getWatchfaceDjiLogoEnabled()) drawListItem(ModeMenuSettingsWatchfaceContentItemShowDjiLogo, draw_ic24_check2, "Лого DJI", "Зараз відображатиметься", firstDraw);
  else drawListItem(ModeMenuSettingsWatchfaceContentItemShowDjiLogo, draw_ic24_cancel, "Лого DJI", "Зараз вимкнено", firstDraw);

  if(getWatchfaceLifeBackgroundEnabled()) drawListItem(ModeMenuSettingsWatchfaceContentItemShowLifeBackgound, draw_ic24_check2, "Фон \"Життя\"", "Зараз відображатиметься", firstDraw);
  else drawListItem(ModeMenuSettingsWatchfaceContentItemShowLifeBackgound, draw_ic24_cancel, "Фон \"Життя\"", "Зараз вимкнено", firstDraw);

  if(getWatchfaceCalendarEnabled()) drawListItem(ModeMenuSettingsWatchfaceContentItemShowCalendar, draw_ic24_check2, "Календар", "Зараз відображатиметься", firstDraw);
  else drawListItem(ModeMenuSettingsWatchfaceContentItemShowCalendar, draw_ic24_cancel, "Календар", "Зараз вимкнено", firstDraw);


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
}





