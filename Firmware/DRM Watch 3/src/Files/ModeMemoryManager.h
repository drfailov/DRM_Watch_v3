/*
Big thanks to this author:
https://github.com/Munsutari/esp32-s3-internal-flash-msc
*/
#ifndef modeMemoryManager_H
#define modeMemoryManager_H

const int modeMemoryManagerItemUpload = 1;
const int modeMemoryManagerItemShowPartitions = 2;
const int modeMemoryManagerItemFormatPartition = 3;


/*PROTOTYPES*/
void modeMemoryManagerLoop();
void modeMemoryManagerExit();
void setmodeMemoryManager();
void modeMemoryManagerButtonUp();
void modeMemoryManagerButtonCenter();
void modeMemoryManagerButtonDown();



#include "Global.h"
#include "AutoSleep.h"
#include "Button.h"
#include "ModeSerialLog.h"
#include "ModeUsbMSC.h"
#include "ModePartitionList.h"
#include <Arduino.h>



void setmodeMemoryManager()
{
  if(modeExit != 0)
    modeExit();
  clearScreenAnimation();
  Serial.println(F("Set mode: Memory Manager"));
  modeSetup = setmodeMemoryManager;
  modeLoop = modeMemoryManagerLoop;
  modeExit = modeMemoryManagerExit;
  modeButtonUp = modeMainMenuButtonUp;
  modeButtonCenter = modeMemoryManagerButtonCenter;
  modeButtonDown = modeMainMenuButtonDown;
  modeButtonUpLong = 0;
  modeButtonCenterLong = 0;
  modeButtonDownLong = 0;
  registerAction();
  enableAutoReturn = false;
  enableAutoSleep = false;
  autoReturnTime = autoReturnDefaultTime;
  autoSleepTime = autoSleepDefaultTime;
  selected = 0;
  items = 4;
}

void modeMemoryManagerLoop()
{
  lcd()->setColorIndex(white);
  lcd()->drawBox(0, 0, 400, 240);

  lcd()->setFont(u8g2_font_10x20_t_cyrillic); // ok
  lcd()->setColorIndex(black);
  lcd()->setCursor(5, 18);
  lcd()->print("Менеджер пам'яті");

  drawStatusbar(363, 1, true);

  drawListItem(itemBack,                              draw_ic24_back,      "Назад",                       "До головного меню",      false);
  drawListItem(modeMemoryManagerItemUpload,           draw_ic24_usb,       "Підключити до USB",           "Щоб закинути файли з компа",      false);
  drawListItem(modeMemoryManagerItemShowPartitions,   draw_ic24_partitions,"Показати карту розділів",     "Як поділена пам'ять контролера",      false);
  drawListItem(modeMemoryManagerItemFormatPartition,  draw_ic24_clean,     "Форматувати розділ",          "Це зітре всі дані на розділі",      false);

  drawMenuLegend();
  lcd()->sendBuffer();
}
void modeMemoryManagerExit()
{
  
}

void modeMemoryManagerButtonCenter()
{
  if(selected == itemBack){
    modeMemoryManagerExit();
    setModeAppsMenu();
    return;
  }
  if(selected == modeMemoryManagerItemUpload){
    setmodeUsbMsc();
    return;
  }
  if(selected == modeMemoryManagerItemShowPartitions){
    setmodePartitionList();
    return;
  }
  if(selected == modeMemoryManagerItemFormatPartition){
    drawMessage("Форматування...", "fatffs", true);
    if(FFat.format(FFAT_WIPE_FULL)){
      drawMessage("Успішно!", "відформатовано fatffs", true);
    } else {
      drawMessage("Помилка", "форматування fatffs", true);
    }
    return;
  }
}



#endif