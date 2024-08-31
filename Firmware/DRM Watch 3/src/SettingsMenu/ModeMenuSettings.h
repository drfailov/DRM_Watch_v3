#ifndef MODEMENUSETTINGS_H
#define MODEMENUSETTINGS_H

/*PROTOTYPES*/
void setModeSettingsMenu();
void modeSettingsMenuLoop();
void globalMenuButtonUp();
void modeSettingsMenuButtonCenter();
void globalMenuButtonDown();


#include "../Global.h"
#include "../AutoSleep.h"
#include "../Button.h"
#include "../ModeMenuApps.h"
#include "ModeMenuSettingsDisplay.h"
#include "ModeMenuSettingsTime.h"
#include "ModeMenuSettingsSound.h"
#include "ModeSavedWiFiList.h"
#include "ShortcutManager.h"

const int itemModeSettingsBack=0;
const int itemModeSettingsCategoryTime=1;
const int itemModeSettingsCategoryDisplay=2;
const int itemModeSettingsCategoryWiFi=3;
const int itemModeSettingsCategorySound=4;
const int itemModeSettingsCategoryShortcuts=5;
const int itemModeSettingsReboot=6;
const int itemModeSettingsShutdown=7;
const int itemModeSettingsAbout=8;

void setModeSettingsMenu(){
  clearScreenAnimation();
  Serial.println(F("Set mode: Settings Menu"));
  modeSetup = setModeSettingsMenu;
  modeLoop = modeSettingsMenuLoop;
  modeButtonUp = globalMenuButtonUp;
  modeButtonCenter = modeSettingsMenuButtonCenter;
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
  items = 9;
}


void modeSettingsMenuLoop(){
  lcd()->setColorIndex(white);
  lcd()->drawBox(0, 0, 400, 240);

  lcd()->setFont(u8g2_font_10x20_t_cyrillic);  //ok
  lcd()->setColorIndex(black);
  lcd()->setCursor(5, 18); 
  lcd()->print(L("Налаштування", "Settings"));

  drawStatusbar(363, 1, true);  
  drawMenuLegend();

  drawListItem(itemModeSettingsBack,                 draw_ic24_back,       L("Назад", "Back"),                  L("До меню програм", "To apps menu"),                              firstDraw);
  drawListItem(itemModeSettingsCategoryTime,         draw_ic24_clock,      L("Дата та час", "Date and time"),   L("Налаштування що стосуються часу", "Set, sync, timezone"),       firstDraw);
  drawListItem(itemModeSettingsCategoryDisplay,      draw_ic24_display,    L("Дисплей", "Display"),             L("Параметри відображення", "Screen style"),                       firstDraw);
  drawListItem(itemModeSettingsCategoryWiFi,         draw_ic24_wifi_3,     "Wi-Fi",                             L("Мережі для доступу до Інтернет", "Networks to access Internet"),firstDraw);
  drawListItem(itemModeSettingsCategorySound,        draw_ic24_sound_on,   L("Звук", "Sound"),                  L("Налаштування що стосуються звуку", "Beeps, tones, silent"),     firstDraw);
  drawListItem(itemModeSettingsCategoryShortcuts,    draw_ic24_shortcut,   L("Швидкий доступ", "Quick access"), L("Встановити дії на кнопки", "Set actions to buttons"),           firstDraw);
  drawListItem(itemModeSettingsReboot,               draw_ic24_reboot,     L("Перезавантажити", "Reboot"),      L("Перезавантажити годинник", "Reboot watch"),                     firstDraw);
  drawListItem(itemModeSettingsShutdown,             draw_ic24_shutdown,   L("Вимкнути", "Turn off"),           L("Вимкнути годинник", "Turn off watch"),                          firstDraw);
  drawListItem(itemModeSettingsAbout   ,             draw_ic24_about,      L("Про годинник", "About watch"),    L("Інформація про розробника", "Information about developer"),     firstDraw);
  

  lcd()->sendBuffer();
}

void modeSettingsMenuButtonCenter(){
  if(selected == itemBack){
    setModeAppsMenu();
    return;
  }
  if(selected == itemModeSettingsCategoryTime){
    setModeMenuSettingsTime();
    return;
  }
  if(selected == itemModeSettingsCategoryDisplay){
    setModeMenuSettingsDisplay();
    return;
  }
  if(selected==itemModeSettingsCategoryWiFi){
    setModeSavedWiFiList();
    return;
  }
  if(selected==itemModeSettingsCategorySound){
    setModeMenuSettingsSound();
    return;
  }
  if(selected==itemModeSettingsCategoryShortcuts){
    setModeShortcutListEventsMenu();
    return;
  }
  if(selected==itemModeSettingsReboot){
    reboot();
    return;
  }
  if(selected == itemModeSettingsShutdown){
    turnOff();
    return;
  }
  if(selected == itemModeSettingsAbout){
    modeAboutSetup();
    return;
  }

}


#endif