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
void formatSelected();



#include "Global.h"
#include "AutoSleep.h"
#include "Button.h"
#include "ModeSerialLog.h"
#include "ModeUsbMSC.h"
#include "ModePartitionList.h"
#include <Arduino.h>
#include "esp_vfs.h"
#include "esp_vfs_fat.h"
#include "esp_system.h"

// Handle of the wear levelling library instance
static wl_handle_t s_wl_handle = WL_INVALID_HANDLE;
const char *base_path = "/spiflash";

void setmodeMemoryManager()
{
  if(modeExit != 0)
    modeExit();
  clearScreenAnimation();
  Serial.println(F("Set mode: Memory Manager"));
  modeSetup = setmodeMemoryManager;
  modeLoop = modeMemoryManagerLoop;
  modeExit = modeMemoryManagerExit;
  modeButtonUp = globalMenuButtonUp;
  modeButtonCenter = modeMemoryManagerButtonCenter;
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


  //https://github.com/espressif/esp-idf/blob/release/v4.4/examples/storage/wear_levelling/main/wear_levelling_example_main.c
    Serial.println("Mounting FAT filesystem");
    // To mount device we need name of device partition, define base_path
    // and allow format partition in case if it is new one and was not formated before
    const esp_vfs_fat_mount_config_t mount_config={true, 4, CONFIG_WL_SECTOR_SIZE};
    // mount_config.max_files = 4;
    //         format_if_mount_failed = true,
    //         allocation_unit_size = CONFIG_WL_SECTOR_SIZE
    
    esp_err_t err = esp_vfs_fat_spiflash_mount(base_path, "ffat", &mount_config, &s_wl_handle);
    if (err != ESP_OK) {
        Serial.print( "Failed to mount FATFS ");
        Serial.println( esp_err_to_name(err));
        return;
    }
}

void modeMemoryManagerLoop()
{
  lcd()->setColorIndex(white);
  lcd()->drawBox(0, 0, 400, 240);

  lcd()->setFont(u8g2_font_10x20_t_cyrillic); // ok
  lcd()->setColorIndex(black);
  lcd()->setCursor(5, 18);
  lcd()->print(L("Менеджер пам'яті", "Memory manager"));

  drawStatusbar(363, 1, true);

  drawListItem(itemBack,                              draw_ic24_back,      L("Назад", "Back"),                              L("До головного меню", "To apps menu"),                        false);
  drawListItem(modeMemoryManagerItemUpload,           draw_ic24_usb,       L("Підключити до USB", "Connect to USB"),        L("Щоб закинути файли з компа", "To upload files from PC"),    false);
  drawListItem(modeMemoryManagerItemShowPartitions,   draw_ic24_partitions,L("Показати карту розділів", "Partitions list"), L("Як поділена пам'ять контролера", "List and sizes"),         false);
  drawListItem(modeMemoryManagerItemFormatPartition,  draw_ic24_clean,     L("Форматувати розділ", "Format partition"),     L("Це зітре всі файли на розділі", "It will erase all files"), false);

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
    questionModeSet(L("Форматувати розділ?", "Format partition?"), L("Всі файли будуть видалені!","All files will be erased!"), formatSelected, setmodeMemoryManager);
    return;
  }
}

void formatSelected()
{
  drawDim();
  drawMessage(L("Форматування...", "Formatting..."), "fatffs", true);
  if(FFat.format(false)){
    drawMessage(L("Успішно!", "Success!"), L("відформатовано fatffs", "fatffs formatted"), true);
  } else {
    drawMessage(L("Помилка", "Failed"), L("форматування fatffs", "formatting fatffs"), true);
  }
  setmodeMemoryManager();
}



#endif