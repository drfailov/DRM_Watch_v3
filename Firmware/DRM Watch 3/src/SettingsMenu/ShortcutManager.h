#ifndef SHORTCUTMANAGER_H
#define SHORTCUTMANAGER_H

#define EVENT_NOTHING 0
#define EVENT_TIMER 1
#define EVENT_WF_BUT_UP_PRESS 2
#define EVENT_WF_BUT_UP_LONG 3
#define EVENT_WF_BUT_CE_PRESS 4
#define EVENT_WF_BUT_CE_LONG 5
const int eventCnt = 6;
const char* eventName(int i){
   if(i==1) return L("Спрацювання таймера","Timer event");
   if(i==2) return L("Кнопка вгору", "Button UP");
   if(i==3) return L("Кнопка вгору довге", "Button UP Long");  
   if(i==4) return L("Центральна кнопка", "Button CENTER");   
   if(i==5) return L("Центральна кнопка довге", "Button CENTER Long");
   return "---";
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
#define ACTION_OPEN_APPS 14
#define ACTION_OPEN_FILE 15
const int actionCnt = 16;
const char *actionName(int i) {
    if(i== ACTION_NOTHING)           return L("Нічого не робити", "Do nothing");                 //+
    if(i== ACTION_OPEN_TIMER)        return L("Відкрити таймер", "Open timer");                  //+
    if(i== ACTION_OPEN_STOPWATCH)    return L("Відкрити секундомір", "Open stopwatch");          //+
    if(i== ACTION_TOGGLE_FLASHLIGHT) return L("Перемкнути ліхтарик", "Switch flashlight");       //+
    if(i== ACTION_TURNOFF)           return L("Вимкнути годинник", "Turn off");                  //+
    if(i== ACTION_PLAY_MELODY)       return L("Зіграти мелодію", "Play melody");                 //+
    if(i== ACTION_PLAY_ALERT)        return L("Сповіщення мелодією", "Alert melody");            //+
    if(i== ACTION_OPEN_ALARMS)       return L("Відкрити будильники", "Open alerts");             //+
    if(i== ACTION_OPEN_CALENDAR)     return L("Відкрити календар", "Open calendar");             //+
    if(i== ACTION_OPEN_SETTING)      return L("Відкрити налаштування", "Open settings");         //+
    if(i==ACTION_TOGGLE_MUTE)        return L("Перемкнути тихий режим", "Switch silent mode");   //+
    if(i==ACTION_TOGGLE_INVERSE)     return L("Перемкнути інверсію", "Switch screen inversion"); //+
    if(i==ACTION_REBOOT)             return L("Перезавантажити", "Reboot");                      //+
    if(i==ACTION_OPEN_ABOUT)         return L("Відкрити \"Про годинник\"", "Open \"About\"");    //+
    if(i==ACTION_OPEN_APPS)          return L("Відкрити список програм", "Open apps");    
    if(i==ACTION_OPEN_FILE)          return L("Відкрити файл", "Open file");    
    return "---";
};

int defaultAction(int eventId);
bool shortcutRun(int eventId);  //returns value based on application scenario. No strict definition.
bool runAction(int actionId, int actionArgument, String actionArgumentString); //returns value based on application scenario. No strict definition.
void setModeShortcutListEventsMenu();
void ModeShortcutListEventsMenuLoop();
void ModeShortcutListEventsMenuButtonCenter();
int ModeShortcutEventSettings_EventId = 0;
void setModeShortcutEventSettingsMenu();
void ModeShortcutEventSettingsMenuLoop();
void ModeShortcutEventSettingsMenuButtonCenter();
void onActionSelected();
void onMelodySelected();
void onFileSelected();
const char* getActioNameC(int index);



#include "../DrmPreferences.h"
#include "../ModeStopwatch.h"
#include "../ModeTimer.h"
#include "../ModeListSelection.h"
#include "../MelodyPlayer.h"
#include "../ModeAlertsList.h"
#include "../ModeCalendar.h"
#include "../Files/ModeFileManager.h"
#include "ModeMenuSettings.h"



const char* getActioNameC(int index)
{
  return actionName(index);
}
bool runAction(int actionId, int actionArgument, String actionArgumentString) //returns value based on application scenario. No strict definition.
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
        melodyPlayerSetMelodyName(L("Сповіщення: ", "Alert: ") + getMelodyName(actionArgument));  //to draw on screen
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
    case ACTION_OPEN_FILE:
      openFile(actionArgumentString.c_str());
      return false;
    default:
        return false;
    }
}

bool shortcutRun(int eventId) //returns value based on application scenario. No strict definition.
{
    int actionId = getActionId(eventId, defaultAction(eventId));
    int actionArgument = getActionArgument(eventId);
    String actionArgumentString = getActionArgumentString(eventId);
    return runAction(actionId, actionArgument, actionArgumentString);
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
  modeButtonUp = globalMenuButtonUp;
  modeButtonCenter = ModeShortcutListEventsMenuButtonCenter;
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
  items = eventCnt+1;
}


void ModeShortcutListEventsMenuLoop(){
  lcd()->setColorIndex(white);
  lcd()->drawBox(0, 0, 400, 240);

  lcd()->setFont(u8g2_font_10x20_t_cyrillic);  //ok
  lcd()->setColorIndex(black);
  lcd()->setCursor(5, 18); 
  lcd()->print(L("Події швидкого доступу", "Quick access events"));

  drawStatusbar(363, 1, true);  
  drawMenuLegend();

  drawListItem(itemModeShortcutListEventsBack,    draw_ic24_back,       L("Назад", "Back"),     L("Повернутись до меню налаштувань", "Back to settings"),       firstDraw);
  for(int i=1; i<eventCnt; i++)
    drawListItem(i,       draw_ic24_shortcut,   eventName(i),                     actionName(getActionId(i)),                      firstDraw);
  drawListItem(eventCnt,  draw_ic24_lock,       L("Кнопка вниз", "Button DOWN"),  L("Відкрити меню програм", "Open apps menu"),    firstDraw);
  
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
    drawMessage(L("Цей пункт не змінити", "Can't be changed"), L("Зарезервовано на відкриття меню", "Reserved to open menu"), true);
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
  modeButtonUp = globalMenuButtonUp;
  modeButtonCenter = ModeShortcutEventSettingsMenuButtonCenter;
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
  items = 3;
}


void ModeShortcutEventSettingsMenuLoop()
{
  lcd()->setColorIndex(white);
  lcd()->drawBox(0, 0, 400, 240);

  lcd()->setFont(u8g2_font_10x20_t_cyrillic);  //ok
  lcd()->setColorIndex(black);
  lcd()->setCursor(5, 18); 
  lcd()->print(eventName(ModeShortcutEventSettings_EventId));

  drawStatusbar(363, 1, true);  
  drawMenuLegend();

  drawListItem(itemModeShortcutEventSettingsBack,    draw_ic24_back,       L("Назад", "Back"),    L("До списку подій", "To events list"),     firstDraw);
  drawListItem(itemModeShortcutEventSettingsAction,  draw_ic24_shortcut,   L("Дія", "Action"),      actionName(getActionId(ModeShortcutEventSettings_EventId)),   firstDraw);
  
  if(getActionId(ModeShortcutEventSettings_EventId) == ACTION_PLAY_MELODY)
    drawListItem(itemModeShortcutEventSettingsArgument,  draw_ic24_music,   L("Мелодія", "Melody"),   getMelodyName(getActionArgument(ModeShortcutEventSettings_EventId)).c_str(), firstDraw);
  
  if(getActionId(ModeShortcutEventSettings_EventId) == ACTION_PLAY_ALERT)
    drawListItem(itemModeShortcutEventSettingsArgument,  draw_ic24_music,   L("Мелодія", "Melody"),   getMelodyName(getActionArgument(ModeShortcutEventSettings_EventId)).c_str(), firstDraw);
  
  if(getActionId(ModeShortcutEventSettings_EventId) == ACTION_OPEN_FILE)
    drawListItem(itemModeShortcutEventSettingsArgument,  draw_ic24_file,   L("Файл", "File"),   getActionArgumentString(ModeShortcutEventSettings_EventId).c_str(), firstDraw);
  

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
    ModeListSelection_Name = eventName(ModeShortcutEventSettings_EventId);
    ModeListSelection_Cnt = actionCnt;
    ModeListSelection_Selected = getActionId(ModeShortcutEventSettings_EventId);
    ModeListSelection_OnSelected = onActionSelected;
    setModeListSelection();
    return;
  }
  if(selected == itemModeShortcutEventSettingsArgument && (getActionId(ModeShortcutEventSettings_EventId) == ACTION_PLAY_MELODY || getActionId(ModeShortcutEventSettings_EventId) == ACTION_PLAY_ALERT))
  {//обрати мелодію
    ModeListSelection_Items = getMelodyNameC;
    ModeListSelection_Name = L("Обрати мелодію", "Select melody");
    ModeListSelection_Cnt = getMelodyCount();
    ModeListSelection_Selected = getActionArgument(ModeShortcutEventSettings_EventId);
    ModeListSelection_OnSelected = onMelodySelected;
    setModeListSelection();
    return;
  }
  if(selected == itemModeShortcutEventSettingsArgument && getActionId(ModeShortcutEventSettings_EventId) == ACTION_OPEN_FILE)
  { //обрати файл
    modeFileManagerOnFileSelected = onFileSelected;
    modeFileManagerOnCancel = setModeShortcutEventSettingsMenu;
    selected = 0;
    setmodeFileManager();
  }
}


void onFileSelected() //callback
{
  saveActionArgumentString(ModeShortcutEventSettings_EventId, modeFileManagerFileSelected);
  setModeShortcutEventSettingsMenu();
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