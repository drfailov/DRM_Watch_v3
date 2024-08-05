#ifndef ModeListSelection_H
#define ModeListSelection_H

void setModeListSelection();
void ModeListSelectionLoop();
void ModeListSelectionButtonCenter();

const char *ModeListSelection_Name;      //fill this before call
Textable ModeListSelection_Items;       //fill this before call
int ModeListSelection_Selected;         //fill this before call
int ModeListSelection_Cnt;              //fill this before call
Runnable ModeListSelection_OnSelected;  //fill this before call

#include "Lcd.h"
#include "Global.h"
#include "AutoSleep.h"
#include "Button.h"
#include "Icons.h"
#include "SettingsMenu/ModeMenuSettings.h"
#include "SettingsMenu/ModeMenuSettingsWatchfaceContent.h"
#include "SettingsMenu/ModeSetLcdFrequency.h"
#include "SettingsMenu/ModeSetWatchface.h"
#include "ModeMainMenu.h"
#include "DrmPreferences.h"




void drawListItem(byte index, Drawable drawIcon, const char* name, bool animate){
  int lines = 8;
  if(selected/(lines) != index/(lines)) return;
  const int xOffset = 10;
  const int yOffset = 26;
  const int width=345;
  const int height=26;
  const int margin = -2;
  int x = xOffset;
  int y = yOffset + (height+margin) * ((index%(lines)));
  
  if(selected == index){
    lcd()->setColorIndex(black);
    lcd()->drawRBox(/*x*/x, /*y*/y, /*w*/width, /*h*/height, roundness);
    //lcd()->drawBox(/*x*/x, /*y*/y, /*w*/width, /*h*/height);
  }
  else{
    lcd()->setColorIndex(black);
    lcd()->drawRBox(/*x*/x, /*y*/y, /*w*/width, /*h*/height, roundness);
    lcd()->setColorIndex(white);
    lcd()->drawRBox(/*x*/x+frame, /*y*/y+frame, /*w*/width-frame*2, /*h*/height-frame*2, roundness);
  }
  drawIcon(x + 7, y+5, selected == index?white:black);
  
  lcd()->setFont(u8g2_font_10x20_t_cyrillic);  //ok
  lcd()->setColorIndex(selected == index?white:black);
  lcd()->setCursor(x+30, y+18); lcd()->print(name);
  //lcd()->setFont(u8g2_font_unifont_t_cyrillic);  //ok
  if(selected == index){
    lcd()->setFont(u8g2_font_10x20_t_cyrillic);  //ok
    lcd()->setColorIndex(black);
    lcd()->setCursor(5, 235); 
    lcd()->print(selected+1);lcd()->print("/");lcd()->print(items); 
  }
  if(animate)
    lcd()->sendBuffer();
}





const int ModeListSelectionItemBack = 0;

void setModeListSelection(){
  clearScreenAnimation();
  Serial.println(F("Set mode: ModeListSelection"));
  modeSetup = setModeListSelection;
  modeLoop = ModeListSelectionLoop;
  modeButtonUp = modeMainMenuButtonUp;
  modeButtonCenter = ModeListSelectionButtonCenter;
  modeButtonDown = modeMainMenuButtonDown;
  modeButtonUpLong = 0;
  modeButtonCenterLong = 0;
  modeButtonDownLong = 0;
  registerAction();
  enableAutoReturn = true;
  enableAutoSleep = false; 
  autoReturnTime = autoReturnDefaultTime;
  autoSleepTime = autoSleepDefaultTime;
  selected = ModeListSelection_Selected;
  items = ModeListSelection_Cnt;
}


void ModeListSelectionLoop(){
  lcd()->setColorIndex(white);
  lcd()->drawBox(0, 0, 400, 240);

  lcd()->setFont(u8g2_font_10x20_t_cyrillic);  //ok
  lcd()->setColorIndex(black);
  lcd()->setCursor(5, 18); 
  lcd()->print(ModeListSelection_Name);

  drawMenuLegend();
  drawStatusbar(363, 1, true);

  for(int i=0; i<ModeListSelection_Cnt; i++)
    drawListItem(i, ModeListSelection_Selected==i?draw_ic16_check:draw_ic16_empty, ModeListSelection_Items(i), firstDraw&i%2);

  lcd()->sendBuffer();
}

void ModeListSelectionButtonCenter(){
  ModeListSelection_Selected = selected;
  if(ModeListSelection_OnSelected != 0){
      ModeListSelection_OnSelected();
  }
}





#endif