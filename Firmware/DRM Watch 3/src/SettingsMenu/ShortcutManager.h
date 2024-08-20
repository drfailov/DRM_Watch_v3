#ifndef SHORTCUTMANAGER_H
#define SHORTCUTMANAGER_H

#define EVENT_NOTHING 0
#define EVENT_TIMER 1
#define EVENT_WF_BUT_UP_PRESS 2
#define EVENT_WF_BUT_UP_LONG 3
#define EVENT_WF_BUT_CE_PRESS 4
#define EVENT_WF_BUT_CE_LONG 5
const int eventCnt = 6;
const char* eventName[] = {
   /* 0 */ "---",
   /* 1 */ "Спрацювання таймера",
   /* 2 */ "Кнопка вгору", 
   /* 3 */ "Кнопка вгору довге",     
   /* 4 */ "Центральна кнопка",      
   /* 5 */ "Центральна кнопка довге"
};

#define ACTION_NOTHING 0
#define ACTION_OPEN_TIMER 1
#define ACTION_OPEN_STOPWATCH 2
#define ACTION_TOGGLE_FLASHLIGHT 3
#define ACTION_TURNOFF 4
#define ACTION_PLAY_MELODY 5
#define ACTION_PLAY_ALERT 6
#define ACTION_OPEN_ALARMS 7
#define ACTION_OPEN_CALENDAR 8
#define ACTION_OPEN_SETTING 9

#define ACTION_TOGGLE_MUTE 10
#define ACTION_TOGGLE_INVERSE 11
#define ACTION_REBOOT 12
#define ACTION_OPEN_ABOUT 13

#define ACTION_SYNC_TIME 14
#define ACTION_OPEN_APPS 15
#define ACTION_SET_TIMER_TO 16
const int actionCnt = 14;
const char *actionName[] = {
    /* 0 */ "Нічого не робити",       //+
    /* 1 */ "Відкрити таймер",        //+   
    /* 2 */ "Відкрити секундомір",    //+ 
    /* 3 */ "Перемкнути ліхтарик",    //+     
    /* 4 */ "Вимкнути годинник",      //+
    /* 5 */ "Зіграти мелодію",        //+
    /* 6 */ "Сповіщення мелодією",    //+
    /* 7 */ "Відкрити будильники",    //+        
    /* 8 */ "Відкрити календар",      //+
    /* 9 */ "Відкрити налаштування",  //+
    /*10 */ "Перемкнути тихий режим", //+
    /*11 */ "Перемкнути інверсію",    //+
    /*12 */ "Перезавантажити",        //+
    /*13 */ "Відкрити \"Про годинник\""//+
};

int defaultAction(int eventId);
bool shortcutRun(int eventId);
bool runAction(int actionId, int actionArgument);
void setModeShortcutListEventsMenu();
void ModeShortcutListEventsMenuLoop();
void ModeShortcutListEventsMenuButtonCenter();
int ModeShortcutEventSettings_EventId = 0;
void setModeShortcutEventSettingsMenu();
void ModeShortcutEventSettingsMenuLoop();
void ModeShortcutEventSettingsMenuButtonCenter();
void onActionSelected();
void onMelodySelected();
const char* getActioNameC(int index);



#include "../DrmPreferences.h"
#include "../ModeStopwatch.h"
#include "../ModeTimer.h"
#include "../ModeListSelection.h"
#include "../MelodyPlayer.h"
#include "../ModeAlertsList.h"
#include "../ModeCalendar.h"
#include "ModeMenuSettings.h"



const char* getActioNameC(int index)
{
  return actionName[index];
}
bool runAction(int actionId, int actionArgument)
{
    switch (actionId)
    {
    case ACTION_OPEN_STOPWATCH:
      setModeStopwatch();
      return false;
    case ACTION_OPEN_TIMER:
      setModeTimer();
      return false;
    case ACTION_TURNOFF:
      turnOff();
      return false;
    case ACTION_TOGGLE_FLASHLIGHT:
      ledFlashlightToggle();
      return false;
    case ACTION_OPEN_APPS:
      setModeAppsMenu();
      return false;
    case ACTION_PLAY_MELODY:
      melodyPlayerSetMelodyName(getMelodyName(actionArgument));  //to draw on screen
      return melodyPlayerPlayMelody(getMelodyData(actionArgument), false);
    case ACTION_PLAY_ALERT:
    {
      //return true if was played completely or false if interrupted
      unsigned long started = rtcGetEpoch();
      unsigned long endTime = started+60*2; //2min
      while(rtcGetEpoch() < endTime)
      {  
        melodyPlayerSetMelodyName("Сповіщення: " + getMelodyName(actionArgument));  //to draw on screen
        if(!melodyPlayerPlayMelody(getMelodyData(actionArgument), true)) //if interrupted
          return false;
      }
      return true;
    }
    case ACTION_OPEN_ALARMS:
      setModeAlertsList();
      return false;
    case ACTION_OPEN_CALENDAR:
      setModeCalendar();
      return false;
    case ACTION_OPEN_SETTING:
      setModeSettingsMenu();
      return false;
    case ACTION_TOGGLE_MUTE:
      saveMuteEnabled(!getMuteEnabled());
      return false;
    case ACTION_TOGGLE_INVERSE:
      saveInversionValue(!getInversionValue());
      black = getBlackValue();
      white = getWhiteValue();
      return false;
    case ACTION_REBOOT:
      reboot();
      return false;
    case ACTION_OPEN_ABOUT:
      modeAboutSetup();
      return false;
    default:
        return false;
    }
}

bool shortcutRun(int eventId)
{
    int actionId = getActionId(eventId, defaultAction(eventId));
    int actionArgument = getActionArgument(eventId);
    return runAction(actionId, actionArgument);
}

int defaultAction(int eventId)
{
    switch (eventId)
    {
    case EVENT_TIMER:
        return ACTION_PLAY_ALERT;
    case EVENT_WF_BUT_UP_PRESS:
        return ACTION_OPEN_STOPWATCH;
    case EVENT_WF_BUT_UP_LONG:
        return ACTION_OPEN_TIMER;
    case EVENT_WF_BUT_CE_PRESS:
        return ACTION_TOGGLE_FLASHLIGHT;
    case EVENT_WF_BUT_CE_LONG:
        return ACTION_TURNOFF;
    default:
        return ACTION_NOTHING;
    }
}





// ================================== 111111111111 MODE LIST EVENTS    START
const int itemModeShortcutListEventsBack=0;

void setModeShortcutListEventsMenu(){
  if(modeExit != 0)
    modeExit();
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
  if(selected > 0 && selected < eventCnt){
    ModeShortcutEventSettings_EventId = selected;
    setModeShortcutEventSettingsMenu();
  }
  if(selected == eventCnt){
    drawMessage("Цей пункт не змінити", "Зарезервовано на відкриття меню", true);
  }

}

// ================================== 111111111111 MODE LIST EVENTS    END



// ================================== 22222222222 MODE EVENT SETTINGS    START

const int itemModeShortcutEventSettingsBack=0;
const int itemModeShortcutEventSettingsAction=1;
const int itemModeShortcutEventSettingsArgument=2;

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
  items = 3;
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

  drawListItem(itemModeShortcutEventSettingsBack,    draw_ic24_back,       "Назад",    "Повернутись до списку подій",                                        firstDraw);
  drawListItem(itemModeShortcutEventSettingsAction,  draw_ic24_shortcut,   "Дія",      actionName[getActionId(ModeShortcutEventSettings_EventId)],           firstDraw);
  if(getActionId(ModeShortcutEventSettings_EventId) == ACTION_PLAY_MELODY  || getActionId(ModeShortcutEventSettings_EventId) == ACTION_PLAY_ALERT)
    drawListItem(itemModeShortcutEventSettingsArgument,draw_ic24_music,   "Мелодія",   getMelodyName(getActionArgument(ModeShortcutEventSettings_EventId)).c_str(), firstDraw);
  else
    drawListItem(itemModeShortcutEventSettingsArgument,draw_ic24_settings,   "Аргумент", String(getActionArgument(ModeShortcutEventSettings_EventId)).c_str(), firstDraw);

  lcd()->sendBuffer();
}

void ModeShortcutEventSettingsMenuButtonCenter()
{
  if(selected == itemModeShortcutEventSettingsBack){
    setModeShortcutListEventsMenu();
    return;
  }
  
  if(selected == itemModeShortcutEventSettingsAction)
  {  //обрати дію
    ModeListSelection_Items = getActioNameC;
    ModeListSelection_Name = eventName[ModeShortcutEventSettings_EventId];
    ModeListSelection_Cnt = actionCnt;
    ModeListSelection_Selected = getActionId(ModeShortcutEventSettings_EventId);
    ModeListSelection_OnSelected = onActionSelected;
    setModeListSelection();
    return;
  }
  if(selected == itemModeShortcutEventSettingsArgument && (getActionId(ModeShortcutEventSettings_EventId) == ACTION_PLAY_MELODY || getActionId(ModeShortcutEventSettings_EventId) == ACTION_PLAY_ALERT))
  {//обрати мелодію
    ModeListSelection_Items = getMelodyNameC;
    ModeListSelection_Name = "Обрати мелодію";
    ModeListSelection_Cnt = getMelodyCount();
    ModeListSelection_Selected = getActionArgument(ModeShortcutEventSettings_EventId);
    ModeListSelection_OnSelected = onMelodySelected;
    setModeListSelection();
    return;
  }
}

void onMelodySelected()//callback
{
  saveActionArgument(ModeShortcutEventSettings_EventId, ModeListSelection_Selected);
  setModeShortcutEventSettingsMenu();
}
void onActionSelected()//callback
{ 
  saveActionId(ModeShortcutEventSettings_EventId, ModeListSelection_Selected);
  setModeShortcutEventSettingsMenu();
}

// ================================== 222222222222 MODE EVENT SETTINGS    END

#endif