#ifndef MODEALERTLIST_H
#define MODEALERTLIST_H

void setModeAlertsList();
void modeAlertsListLoop();
void modeAlertsListButtonCenter();

#include "Lcd.h"
#include "Global.h"
#include "AutoSleep.h"
#include "Button.h"
#include "ModeMenuApps.h"
#include "ModeAlertSettings.h"


void setModeAlertsList(){
  clearScreenAnimation();
  Serial.println(F("Set mode: AlertsList"));
  modeSetup = setModeAlertsList;
  modeLoop = modeAlertsListLoop;
  modeButtonUp = modeMainMenuButtonUp;
  modeButtonCenter = modeAlertsListButtonCenter;
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
  items = getAlertsNumber()+1;
}


void modeAlertsListLoop(){
  lcd()->setColorIndex(white);
  lcd()->drawBox(0, 0, 400, 240);


  lcd()->setFont(u8g2_font_10x20_t_cyrillic);  //ok
  lcd()->setColorIndex(black);
  lcd()->setCursor(5, 18); 
  lcd()->print(L("Список будильників", "Alerts list"));

  drawMenuLegend();
  drawStatusbar(363, 1, true);

  drawListItem(0, draw_ic24_back, L("Повернутись", "Back"), L("В меню програм", "To apps menu"), firstDraw); //
  for(int i=1; i<items; i++){
    int h = getAlertHour(i-1);
    int m = getAlertMinute(i-1);
    sprintf(buffer, "%02d:%02d", h,m);
    drawListCheckbox(
      i, 
      draw_ic24_alarm, 
      ((String("") + (i-1) + ": " + getAlertName(i-1) ).c_str()), 
      buffer, 
      getAlertEnabled(i-1), 
      firstDraw); 
  }

  lcd()->sendBuffer();
}

void modeAlertsListButtonCenter(){
  if(selected == 0){
      setModeAppsMenu(); //setModeSettingsMenu();
  }
  else{
    setModeAlertSettingsMenu(selected-1);
  }
}


#endif