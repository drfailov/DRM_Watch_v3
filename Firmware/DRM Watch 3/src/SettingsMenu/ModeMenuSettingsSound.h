#ifndef MODEMENUSETTINGSSOUND_H
#define MODEMENUSETTINGSSOUND_H

void setModeMenuSettingsSound();
void ModeMenuSettingsSoundLoop();
void ModeMenuSettingsSoundButtonCenter();

#include "../Global.h"
#include "../Lcd.h"
#include "../Button.h"
#include "../Buzzer.h"
#include "../AutoSleep.h"
#include "../GlobalMenu.h"
#include "ModeSetSoundTone.h"
#include "../Icons.h"

const int ModeMenuSettingsSoundItemBack = 0;
const int itemModeSettingsButtonSound=1;
const int itemModeSettingsMute=2;
const int itemModeSettingsSoundTone=3;

void setModeMenuSettingsSound(){
  clearScreenAnimation();
  Serial.println(F("Set mode: ModeMenuSettingsSound"));
  modeSetup = setModeMenuSettingsSound;
  modeLoop = ModeMenuSettingsSoundLoop;
  modeButtonUp = globalMenuButtonUp;
  modeButtonCenter = ModeMenuSettingsSoundButtonCenter;
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
  items = 4;
}

void ModeMenuSettingsSoundLoop()
{
  lcd()->setColorIndex(white);
  lcd()->drawBox(0, 0, 400, 240);


  lcd()->setFont(u8g2_font_10x20_t_cyrillic);  //ok
  lcd()->setColorIndex(black);
  lcd()->setCursor(5, 18); 
  lcd()->print(L("Параметри звуку", "Sound settings"));

  drawMenuLegend();
  drawStatusbar(363, 1, true);

  drawListItem(ModeMenuSettingsSoundItemBack, draw_ic24_back,         L("Повернутись","Back"),          L("B меню налаштувань","To settings menu"),                   firstDraw); 
  drawListValue(itemModeSettingsButtonSound,  draw_ic24_buttonsound,  L("Звук кнопок","Button sound"),  L("Обрати звук кнопок","Select button sound"),                getButtonSound(), firstDraw);
  drawListCheckbox(itemModeSettingsMute,      draw_ic24_sound_mute,   L("Тихий режим","Silent mode"),   L("Вимкнути все окрім мелодій","Disable all sound"),          getMuteEnabled(), firstDraw);
  drawListItem(itemModeSettingsSoundTone,     draw_ic24_sound_on,     L("Тон звуку","Sound tone"),      L("Змінити тон всього звуку","Change pitch of ALL sounds"),   firstDraw);
  lcd()->sendBuffer();
}

void ModeMenuSettingsSoundButtonCenter(){
  if(selected == ModeMenuSettingsSoundItemBack){
      setModeSettingsMenu();
  }
  if(selected==itemModeSettingsMute){
    saveMuteEnabled(!getMuteEnabled());
    return;
  }
  if(selected==itemModeSettingsSoundTone){
    setmodeSetSoundToneMenu();
    return;
  }
  if(selected == itemModeSettingsButtonSound){
    int sound = getButtonSound();
    saveButtonSound(sound+1);
    delay(500);
    buttonBeep();
    return;
  }
}





#endif