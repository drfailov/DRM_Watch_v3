#ifndef SHORTCUTMANAGER_H
#define SHORTCUTMANAGER_H

#define EVENT_WF_BUT_UP_PRESS 1
#define EVENT_WF_BUT_UP_LONG 2
#define EVENT_WF_BUT_CE_PRESS 3
#define EVENT_WF_BUT_CE_LONG 4
#define EVENT_WF_BUT_DN_PRESS 5
#define EVENT_WF_BUT_DN_LONG 6

#define ACTION_NOTHING 0
#define ACTION_OPEN_TIMER 1
#define ACTION_OPEN_STOPWATCH 2
#define ACTION_TOGGLE_FLASHLIGHT 3
#define ACTION_TURNOFF 4

#define ACTION_PLAY_MELODY 5
#define ACTION_OPEN_ALARMS 6
#define ACTION_OPEN_CALENDAR 7
#define ACTION_OPEN_SETTING 8
#define ACTION_TOGGLE_MUTE 9
#define ACTION_TOGGLE_INVERSE 10
#define ACTION_REBOOT 11
#define ACTION_OPEN_ABOUT 12
#define ACTION_SYNC_TIME 13
#define ACTION_OPEN_APPS 14
#define ACTION_SET_TIMER_TO 15


int defaultAction(int eventId);
void shortcutRun(int eventId);
void runAction(int actionId);
void setModeShortcutListEventsMenu();
void ModeShortcutListEventsMenuLoop();
void ModeShortcutListEventsMenuButtonCenter();



#include "DrmPreferences.h"
#include "ModeStopwatch.h"
#include "ModeTimer.h"


void runAction(int actionId)
{
    switch (actionId)
    {
    case ACTION_OPEN_STOPWATCH:
        setModeStopwatch();
        break;
    case ACTION_OPEN_TIMER:
        setModeTimer();
        break;
    case ACTION_TURNOFF:
        turnOff();
        break;
    case ACTION_TOGGLE_FLASHLIGHT:
        ledFlashlightToggle();
        break;
    case ACTION_OPEN_APPS:
        setModeAppsMenu();
        break;
    
    default:
        break;
    }
}

void shortcutRun(int eventId)
{
    int actionId = getActionId(eventId, defaultAction(eventId));
    runAction(actionId);
}

int defaultAction(int eventId)
{
    switch (eventId)
    {
    case EVENT_WF_BUT_UP_PRESS:
        return ACTION_OPEN_STOPWATCH;
    case EVENT_WF_BUT_UP_LONG:
        return ACTION_OPEN_TIMER;
    case EVENT_WF_BUT_CE_PRESS:
        return ACTION_TOGGLE_FLASHLIGHT;
    case EVENT_WF_BUT_CE_LONG:
        return ACTION_TURNOFF;
    case EVENT_WF_BUT_DN_PRESS:
        return ACTION_OPEN_APPS;
    default:
        return ACTION_NOTHING;
    }
}





// ================================== MODE LIST EVENTS    START
const int itemModeShortcutListEventsBack=0;

void setModeShortcutListEventsMenu(){
  clearScreenAnimation();
  Serial.println(F("Set mode: Shortcuts List Events"));
  modeSetup = setModeShortcutListEventsMenu;
  modeLoop = ModeShortcutListEventsMenuLoop;
  modeButtonUp = modeMainMenuButtonUp;
  modeButtonCenter = ModeShortcutListEventsMenuButtonCenter;
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
  items = 7;
}


void ModeShortcutListEventsMenuLoop(){
  lcd()->setColorIndex(white);
  lcd()->drawBox(0, 0, 400, 240);

  lcd()->setFont(u8g2_font_10x20_t_cyrillic);  //ok
  lcd()->setColorIndex(black);
  lcd()->setCursor(5, 18); 
  lcd()->print("Події швидкого доступу");

  drawStatusbar(363, 1, true);  
  drawMenuLegend();

  drawListItem(itemModeShortcutListEventsBack,    draw_ic24_back,       "Назад",                   "Повернутись до меню налаштувань",       firstDraw);
  drawListItem(EVENT_WF_BUT_UP_PRESS,             draw_ic24_shortcut,   "Кнопка вгору",            "Коротке натиснення на циферблаті",      firstDraw);
  drawListItem(EVENT_WF_BUT_UP_LONG,              draw_ic24_shortcut,   "Кнопка вгору довге",      "Довге натиснення на циферблаті",        firstDraw);
  drawListItem(EVENT_WF_BUT_CE_PRESS,             draw_ic24_shortcut,   "Центральна кнопка",       "Коротке натиснення на циферблаті",      firstDraw);
  drawListItem(EVENT_WF_BUT_CE_LONG,              draw_ic24_shortcut,   "Центральна кнопка довге", "Довге натиснення на циферблаті",        firstDraw);
  drawListItem(EVENT_WF_BUT_DN_PRESS,             draw_ic24_shortcut,   "Кнопка вниз",             "Коротке натиснення на циферблаті",      firstDraw);
  drawListItem(EVENT_WF_BUT_DN_LONG,              draw_ic24_shortcut,   "Кнопка вниз довге",       "Довге натиснення на циферблаті",        firstDraw);
  

  lcd()->sendBuffer();
}

void ModeShortcutListEventsMenuButtonCenter(){
  if(selected == itemBack){
    setModeSettingsMenu();
    return;
  }
}

// ================================== MODE LIST EVENTS    END

#endif