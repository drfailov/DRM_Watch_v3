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
const int itemModeAlertSettingsName=5;

void setModeAlertSettingsMenu(){
  setModeAlertSettingsMenu(modeAlertSettingsIndex);
}
void setModeAlertSettingsMenu(int alertIndex){
  clearScreenAnimation();
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
  items = 6;
  modeAlertSettingsIndex = alertIndex;
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

  drawStatusbar(363, 1, true);
  drawMenuLegend();
  if(modeAlertSettingsEditMode){
    draw_ic16_plus(lx(), ly1(), black);
    draw_ic16_minus(lx(), ly3(), black);
  }

  
  drawMenuItem(itemModeAlertSettingsBack, draw_ic24_back, "Назад", firstDraw, 10, 26);
  lcd()->setFont(u8g2_font_10x20_t_cyrillic);  //ok
  lcd()->setColorIndex(black);
  lcd()->setCursor(189, 57); 
  lcd()->print("Активний");
  if(getAlertEnabled(modeAlertSettingsIndex))
    drawMenuItem(itemModeAlertSettingsEnabled, draw_ic24_check2, "Вимкнути", firstDraw, 277, 26);  //draw_ic24_cancel   draw_ic24_check2
  else
    drawMenuItem(itemModeAlertSettingsEnabled, draw_ic24_cancel, "Увімкнути", firstDraw, 277, 26);  //draw_ic24_cancel   draw_ic24_check2

  lcd()->setFont(u8g2_font_10x20_t_cyrillic);  //ok
  lcd()->setColorIndex(black);
  lcd()->setCursor(50, 108); 
  lcd()->print("Час");
  drawNumberFrame(/*index*/itemModeAlertSettingsHour, /*number*/modeAlertSettingsHourValue, /*name*/"Обрати годину", /*editMode*/modeAlertSettingsEditMode, /*animate*/firstDraw, /*x*/88, /*y*/81, /*Width*/82);
  lcd()->setFont(u8g2_font_inr24_t_cyrillic);  //ok
  lcd()->setColorIndex(black);
  lcd()->setCursor(172, 111); 
  lcd()->print(":");
  drawNumberFrame(/*index*/itemModeAlertSettingsMinute, /*number*/modeAlertSettingsMinuteValue, /*name*/"Обрати хвилину", /*editMode*/modeAlertSettingsEditMode, /*animate*/firstDraw, /*x*/193, /*y*/81, /*Width*/82);

  lcd()->setFont(u8g2_font_10x20_t_cyrillic);  //ok
  lcd()->setColorIndex(black);
  lcd()->setCursor(8, 154); 
  lcd()->print("Мелодія");
  drawTextFrame(/*index*/itemModeAlertSettingsMelody, /*text*/getMelodyName(modeAlertSettingsMelodyValue).c_str(), /*name*/"Обрати мелодію", /*editMode*/modeAlertSettingsEditMode, /*animate*/firstDraw, /*x*/88, /*y*/127, /*width*/265);
  

  lcd()->setFont(u8g2_font_10x20_t_cyrillic);  //ok
  lcd()->setColorIndex(black);
  lcd()->setCursor(28, 199); 
  lcd()->print("Назва");
  drawTextFrame(/*index*/itemModeAlertSettingsName, /*text*/getAlertName(modeAlertSettingsIndex).c_str(), /*name*/"Назва будильника", /*editMode*/modeAlertSettingsEditMode, /*animate*/firstDraw, /*x*/88, /*y*/173, /*width*/265);

  
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
    setModeAlertsList();//setModeAppsMenu();
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
  if(selected==itemModeAlertSettingsName){
    //todo open keyboard
    setModeKeyboard("Ім'я будильника", ModeAlertSettingsMenuNameSelected, setModeAlertSettingsMenu); //then String password = getKeybordResult();
    setKeybordText(getAlertName(modeAlertSettingsIndex));
    return;
  }
}
void ModeAlertSettingsMenuNameSelected(){
  String newName = getKeybordResult();
  saveAlertName(modeAlertSettingsIndex, newName);
  setModeAlertSettingsMenu();
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


void alertLoop(){
  for(int alertIndex=0; alertIndex<getAlertsNumber(); alertIndex++){ //Обработка будильника 
    //play melody and mark this day as playen if:  alert enabled, in this day was not playen, this is right time to play
    bool alertIsEnabled = getAlertEnabled(alertIndex);
    int alertLastRunDay = getAlertLastRunDay(alertIndex);
    int alertTimeHour = getAlertHour(alertIndex);
    int alertTimeMinute = getAlertMinute(alertIndex);
    int alertMelodyIndex = getAlertMelody(alertIndex);
    int hour = rtcGetHour();
    int minute = rtcGetMinute();
    int day = rtcGetDay();
    if (alertIsEnabled) {
      if (alertLastRunDay != day) {
        if ((hour == alertTimeHour && minute >= alertTimeMinute) || (hour > alertTimeHour)) {
          saveAlertLastRunDay(alertIndex, day);
          long timeStarted = millis();
          long playTime = 180000;
          sprintf(buffer, (getAlertName(alertIndex)+" (%02d:%02d)").c_str(), alertTimeHour, alertTimeMinute);
          melodyPlayerSetMelodyName(String(buffer));  //to draw on screen
          while (melodyPlayerPlayMelody(getMelodyData(alertMelodyIndex), true) && millis() - timeStarted < playTime);
        }
      }
    }
  }
}
