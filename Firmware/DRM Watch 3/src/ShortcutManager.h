#ifndef SHORTCUTMANAGER_H
#define SHORTCUTMANAGER_H

#define EVENT_NOTHING 0
#define EVENT_WF_BUT_UP_PRESS 1
#define EVENT_WF_BUT_UP_LONG 2
#define EVENT_WF_BUT_CE_PRESS 3
#define EVENT_WF_BUT_CE_LONG 4
const int eventCnt = 5;
const char* eventName[] = {
   /* 0 */ "---",
   /* 1 */ "Кнопка вгору",           
   /* 2 */ "Кнопка вгору довге",     
   /* 3 */ "Центральна кнопка",      
   /* 4 */ "Центральна кнопка довге"
};

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
const int actionCnt = 9;
const char* actionName[] = {
    /* 0 */ "Нічого не робити",
    /* 1 */ "Відкрити таймер",           
    /* 2 */ "Відкрити секундомір",     
    /* 3 */ "Перемкнути ліхтарик",      
    /* 4 */ "Вимкнути годинник",
    /* 5 */ "Зіграти мелодію",            
    /* 6 */ "Відкрити будильники",            
    /* 7 */ "Відкрити календар",
    /* 8 */ "Відкрити налаштування"
};


int defaultAction(int eventId);
void shortcutRun(int eventId);
void runAction(int actionId);
void setModeShortcutListEventsMenu();
void ModeShortcutListEventsMenuLoop();
void ModeShortcutListEventsMenuButtonCenter();
int ModeShortcutEventSettings_EventId = 0;
void setModeShortcutEventSettingsMenu();
void ModeShortcutEventSettingsMenuLoop();
void ModeShortcutEventSettingsMenuButtonCenter();




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
    // case EVENT_WF_BUT_DN_PRESS:
    //     return ACTION_OPEN_APPS;
    default:
        return ACTION_NOTHING;
    }
}





// ================================== 111111111111 MODE LIST EVENTS    START
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
  items = eventCnt+1;
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
  for(int i=1; i<eventCnt; i++)
    drawListItem(i,       draw_ic24_shortcut,   eventName[i],   actionName[getActionId(i)],      firstDraw);
  drawListItem(eventCnt,  draw_ic24_lock,       "Кнопка вниз",  "Відкрити меню програм",         firstDraw);
  
  lcd()->sendBuffer();
}

void ModeShortcutListEventsMenuButtonCenter(){
  if(selected == itemModeShortcutListEventsBack){
    setModeSettingsMenu();
    return;
  }
  if(selected > 0 && selected <= eventCnt){
    ModeShortcutEventSettings_EventId = selected;
    setModeShortcutEventSettingsMenu();
  }

}

// ================================== 111111111111 MODE LIST EVENTS    END



// ================================== 22222222222 MODE EVENT SETTINGS    START

const int itemModeShortcutEventSettingsBack=0;
const int itemModeShortcutEventSettingsAction=1;

void setModeShortcutEventSettingsMenu()
{
  clearScreenAnimation();
  Serial.println(F("Set mode: Shortcut Event Settings"));
  modeSetup = setModeShortcutEventSettingsMenu;
  modeLoop = ModeShortcutEventSettingsMenuLoop;
  modeButtonUp = modeMainMenuButtonUp;
  modeButtonCenter = ModeShortcutEventSettingsMenuButtonCenter;
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
  items = 2;
}


void ModeShortcutEventSettingsMenuLoop()
{
  lcd()->setColorIndex(white);
  lcd()->drawBox(0, 0, 400, 240);

  lcd()->setFont(u8g2_font_10x20_t_cyrillic);  //ok
  lcd()->setColorIndex(black);
  lcd()->setCursor(5, 18); 
  lcd()->print(eventName[ModeShortcutEventSettings_EventId]);

  drawStatusbar(363, 1, true);  
  drawMenuLegend();

  drawListItem(itemModeShortcutEventSettingsBack,    draw_ic24_back,       "Назад",   "Повернутись до списку подій",                   firstDraw);
  drawListItem(itemModeShortcutEventSettingsAction,  draw_ic24_shortcut,   "Дія",     actionName[getActionId(ModeShortcutEventSettings_EventId)],   firstDraw);

  lcd()->sendBuffer();
}

void ModeShortcutEventSettingsMenuButtonCenter()
{
  if(selected == itemModeShortcutEventSettingsBack){
    setModeShortcutListEventsMenu();
    return;
  }
  
  if(selected == itemModeShortcutEventSettingsAction){
    setModeShortcutListEventsMenu();
    return;
  }
}

// ================================== 222222222222 MODE EVENT SETTINGS    END

#endif