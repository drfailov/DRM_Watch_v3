int modeAlertSettingsIndex = 0;
int modeAlertSettingsHourValue = 0;
int modeAlertSettingsMinuteValue = 0;
int modeAlertSettingsMelodyValue = 0;
bool modeAlertSettingsEditMode = false;

const int itemModeAlertSettingsBack=0;
const int itemModeAlertSettingsEnabled=1;
const int itemModeAlertSettingsHour=2;
const int itemModeAlertSettingsMinute=3;
const int itemModeAlertSettingsMelody=4;

void setModeAlertSettingsMenu(){
  Serial.println(F("Set mode: Alert settings Menu"));
  modeSetup = setModeAlertSettingsMenu;
  modeLoop = ModeAlertSettingsMenuLoop;
  modeButtonUp = ModeAlertSettingsMenuButtonUp;
  modeButtonCenter = ModeAlertSettingsMenuButtonCenter;
  modeButtonDown = ModeAlertSettingsMenuButtonDown;
  modeButtonUpLong = 0;
  modeButtonCenterLong = 0;
  modeButtonDownLong = 0;
  registerAction();
  enableAutoReturn = true;
  enableAutoSleep = false; 
  autoReturnTime = autoReturnDefaultTime;
  autoSleepTime = autoSleepDefaultTime;
  selected = 0;
  items = 5;
  modeAlertSettingsHourValue = getAlertHour(modeAlertSettingsIndex);
  modeAlertSettingsMinuteValue = getAlertMinute(modeAlertSettingsIndex);
  modeAlertSettingsMelodyValue = getAlertMelody(modeAlertSettingsIndex);
}


void ModeAlertSettingsMenuLoop(){
  if(modeAlertSettingsHourValue < 0) modeAlertSettingsHourValue = 23;
  if(modeAlertSettingsHourValue > 23) modeAlertSettingsHourValue = 0;
  if(modeAlertSettingsMinuteValue < 0) modeAlertSettingsMinuteValue = 59;
  if(modeAlertSettingsMinuteValue > 59) modeAlertSettingsMinuteValue = 0;
  if(modeAlertSettingsMelodyValue < 0) modeAlertSettingsMelodyValue = getMelodyCount()-1;
  if(modeAlertSettingsMelodyValue > getMelodyCount()-1) modeAlertSettingsMelodyValue = 0;

  lcd()->setColorIndex(white);
  lcd()->drawBox(0, 0, 400, 240);

  lcd()->setFont(u8g2_font_10x20_t_cyrillic);  //ok
  lcd()->setColorIndex(black);
  lcd()->setCursor(5, 18); 
  lcd()->print("Будильник ");
  lcd()->print(modeAlertSettingsIndex);
  
  drawMenuItem(itemModeAlertSettingsBack, draw_ic24_back, "Назад", false, 30, 32);
  lcd()->setFont(u8g2_font_10x20_t_cyrillic);  //ok
  lcd()->setColorIndex(black);
  lcd()->setCursor(189, 57); 
  lcd()->print("Активний");
  if(getAlertEnabled(modeAlertSettingsIndex))
    drawMenuItem(itemModeAlertSettingsEnabled, draw_ic24_check, "Вимкнути", false, 277, 32);
  else
    drawMenuItem(itemModeAlertSettingsEnabled, draw_ic24_empty, "Увімкнути", false, 277, 32);

  lcd()->setFont(u8g2_font_10x20_t_cyrillic);  //ok
  lcd()->setColorIndex(black);
  lcd()->setCursor(30, 113); 
  lcd()->print("Час");
  drawNumberFrame(/*index*/itemModeAlertSettingsHour, /*number*/modeAlertSettingsHourValue, /*name*/"Обрати годину", /*editMode*/modeAlertSettingsEditMode, /*animate*/false, /*x*/68, /*y*/87, /*Width*/82);
  lcd()->setFont(u8g2_font_inr24_t_cyrillic);  //ok
  lcd()->setColorIndex(black);
  lcd()->setCursor(152, 116); 
  lcd()->print(":");
  drawNumberFrame(/*index*/itemModeAlertSettingsMinute, /*number*/modeAlertSettingsMinuteValue, /*name*/"Обрати хвилину", /*editMode*/modeAlertSettingsEditMode, /*animate*/false, /*x*/173, /*y*/87, /*Width*/82);

  lcd()->setFont(u8g2_font_10x20_t_cyrillic);  //ok
  lcd()->setColorIndex(black);
  lcd()->setCursor(30, 151); 
  lcd()->print("Мелодія");
  drawTextFrame(/*index*/itemModeAlertSettingsMelody, /*text*/getMelodyName(modeAlertSettingsMelodyValue).c_str(), /*name*/"Обрати мелодію", /*editMode*/modeAlertSettingsEditMode, /*animate*/false, /*x*/30, /*y*/157, /*width*/308);

  drawStatusbar(363, 1, true);
  drawMenuLegend();
  if(modeAlertSettingsEditMode){
    draw_ic16_plus(lx(), ly1(), black);
    draw_ic16_minus(lx(), ly3(), black);
  }
  lcd()->sendBuffer();
}

void ModeAlertSettingsMenuButtonUp(){
  if(!modeAlertSettingsEditMode){
    modeMainMenuButtonUp();
    return;
  }
  if(selected == itemModeAlertSettingsHour)  modeAlertSettingsHourValue ++;
  if(selected == itemModeAlertSettingsMinute)  modeAlertSettingsMinuteValue ++;
  if(selected == itemModeAlertSettingsMelody)  modeAlertSettingsMelodyValue ++;
}
void ModeAlertSettingsMenuButtonDown(){
  if(!modeAlertSettingsEditMode){
    modeMainMenuButtonDown();
    return;
  }
  if(selected == itemModeAlertSettingsHour)  modeAlertSettingsHourValue --;
  if(selected == itemModeAlertSettingsMinute)  modeAlertSettingsMinuteValue --;
  if(selected == itemModeAlertSettingsMelody)  modeAlertSettingsMelodyValue --;
}
void ModeAlertSettingsMenuButtonCenter(){
  if(selected == itemModeAlertSettingsBack){
    setModeAppsMenu();
    return;
  }
  if(selected == itemModeAlertSettingsEnabled){
    saveAlertEnabled(modeAlertSettingsIndex, !getAlertEnabled(modeAlertSettingsIndex));
    resetAlertMetadata(modeAlertSettingsIndex);
    return;
  }
  if(selected==itemModeAlertSettingsHour){
    modeAlertSettingsEditMode = !modeAlertSettingsEditMode;
    if(!modeAlertSettingsEditMode){
      saveAlertHour(modeAlertSettingsIndex, modeAlertSettingsHourValue);
      resetAlertMetadata(modeAlertSettingsIndex);
    }
    return;
  }
  if(selected==itemModeAlertSettingsMinute){
    modeAlertSettingsEditMode = !modeAlertSettingsEditMode;
    if(!modeAlertSettingsEditMode){
      saveAlertMinute(modeAlertSettingsIndex, modeAlertSettingsMinuteValue);
      resetAlertMetadata(modeAlertSettingsIndex);
    }
    return;
  }
  if(selected==itemModeAlertSettingsMelody){
    modeAlertSettingsEditMode = !modeAlertSettingsEditMode;
    if(!modeAlertSettingsEditMode)
      saveAlertMelody(modeAlertSettingsIndex, modeAlertSettingsMelodyValue);
    return;
  }
}
//fix alarm, предотвращение включения будильника сразу в момент установки
void resetAlertMetadata(int index){
  int hour = rtcGetHour();
  int minute = rtcGetMinute();
  int day = rtcGetDay();
  int HourValue = getAlertHour(index);
  int MinuteValue = getAlertMinute(index);
  if((hour == HourValue && minute >= MinuteValue) || (hour > HourValue))
    saveAlertLastRunDay(index, day);
  else
    saveAlertLastRunDay(index, 0);
}
