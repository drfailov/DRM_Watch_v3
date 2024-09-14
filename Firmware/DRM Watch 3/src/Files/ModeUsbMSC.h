/*
Big thanks to this author:
https://github.com/Munsutari/esp32-s3-internal-flash-msc
*/
#ifndef modeUsbMsc_H
#define modeUsbMsc_H

const int modeUsbMscItemUpload = 1;
const int modeUsbMscItemShowPartitions = 2;
const int modeUsbMscItemFormatPartition = 3;


/*PROTOTYPES*/
void modeUsbMscLoop();
void modeUsbMscExit();
void setmodeUsbMsc();
void modeUsbMscButtonUp();
void modeUsbMscButtonCenter();
void modeUsbMscButtonDown();



#include "Global.h"
#include "AutoSleep.h"
#include "Button.h"
#include "ModeSerialLog.h"
#include "ModeMemoryManager.h"
#include <Arduino.h>
#include "USB.h"
#include "USBMSC.h"


USBMSC MSC;
uint32_t modeUsbMsc_bytesRead = 0;
uint32_t modeUsbMsc_bytesWrite = 0;
uint32_t modeUsbMsc_errorsWrite = 0;


static int32_t modeUsbMsc_onWrite(uint32_t lba, uint32_t offset, uint8_t* buffer, uint32_t bufsize){
  
  if(s_wl_handle == WL_INVALID_HANDLE)
    return 0;

  for(int retries = 0; retries < 5; retries++)
  {
    ledStatusOn();
    modeUsbMsc_bytesWrite += bufsize;
    //res = esp_partition_erase_range(modeUsbMsc_partition, offset + (lba * BLOCK_SIZE), bufsize);
    res = wl_erase_range(s_wl_handle, offset + (lba * BLOCK_SIZE), bufsize); //esp_err_t wl_erase_range(wl_handle_thandle, size_t start_addr, size_t size)
    if(res != ESP_OK) 
      modeUsbMsc_errorsWrite++;
    //res = esp_partition_write(modeUsbMsc_partition, offset + (lba * BLOCK_SIZE), buffer,      bufsize);
    res = wl_write(s_wl_handle, offset + (lba * BLOCK_SIZE), buffer, bufsize); //esp_err_t wl_write(wl_handle_thandle, size_t dest_addr, const void *src, size_t size)
    if(res != ESP_OK) 
      modeUsbMsc_errorsWrite++;
    //res = esp_partition_read (modeUsbMsc_partition, offset + (lba * BLOCK_SIZE), page_buffer, bufsize);
    res = wl_read (s_wl_handle, offset + (lba * BLOCK_SIZE), page_buffer, bufsize); //esp_err_twl_read(wl_handle_thandle, size_t src_addr, void *dest, size_t size)
    if(res != ESP_OK) 
      modeUsbMsc_errorsWrite++;
    if(memcmp (buffer, page_buffer, bufsize) == 0)
      return bufsize;
    modeUsbMsc_errorsWrite ++;
  }
  return 0;
}

static int32_t modeUsbMsc_onRead(uint32_t lba, uint32_t offset, void* buffer, uint32_t bufsize)
{
  if(s_wl_handle == WL_INVALID_HANDLE)
    return 0;
  ledStatusOn();
  modeUsbMsc_bytesRead += bufsize;
  //esp_partition_read(modeUsbMsc_partition, offset + (lba * BLOCK_SIZE), buffer, bufsize);       //(uint32_t*)
  //esp_err_twl_read(wl_handle_thandle, size_t src_addr, void *dest, size_t size)
  res = wl_read(s_wl_handle, offset + (lba * BLOCK_SIZE), buffer, bufsize);
  if(res != ESP_OK){
    ledFlashlightOnTop();
    Serial.print("Error reading: ");
    Serial.println( esp_err_to_name(res));
  }
  return bufsize;
}

static bool modeUsbMsc_onStartStop(uint8_t power_condition, bool start, bool load_eject){
  if(load_eject){
    setmodeMemoryManager();
    return true;
  }
  ledStatusOn();
  return true;
}

void setmodeUsbMsc()
{
  if(modeExit != 0)
    modeExit();
  clearScreenAnimation();
  Serial.println(F("Set mode: USB MSC"));
  modeSetup = setmodeUsbMsc;
  modeLoop = modeUsbMscLoop;
  modeExit = modeUsbMscExit;
  modeButtonUp = modeUsbMscButtonUp;
  modeButtonCenter = modeUsbMscButtonCenter;
  modeButtonDown = modeUsbMscButtonDown;
  modeButtonUpLong = 0;
  modeButtonCenterLong = 0;
  modeButtonDownLong = 0;
  registerAction();
  enableAutoReturn = false;
  enableAutoSleep = false;
  autoReturnTime = autoReturnDefaultTime;
  autoSleepTime = autoSleepDefaultTime;
  modeUsbMsc_bytesRead = 0;
  modeUsbMsc_bytesWrite = 0;
  
  //https://github.com/espressif/esp-idf/blob/release/v4.4/examples/storage/wear_levelling/main/wear_levelling_example_main.c
  
  // To mount device we need name of device partition, define base_path
  // and allow format partition in case if it is new one and was not formated before
  
  Serial.println("Finding FFAT partition...");
  if(ffat_partition != NULL)
  {
    Serial.println("FFAT partition already found.");
  }
  else
  {
    //modeUsbMsc_partition = esp_partition_find_first(ESP_PARTITION_TYPE_DATA, ESP_PARTITION_SUBTYPE_ANY, "spiffs");
    //modeUsbMsc_partition = esp_partition_find_first(ESP_PARTITION_TYPE_APP, ESP_PARTITION_SUBTYPE_ANY, "app1");
    ffat_partition = esp_partition_find_first(ESP_PARTITION_TYPE_DATA, ESP_PARTITION_SUBTYPE_DATA_FAT, NULL); //fatffs
    if(ffat_partition == NULL)
    {
      Serial.println("Partition not found=(");
      return;
    }
    Serial.println("Partition found.");
  }

  
  Serial.println("Mounting WearLeveling...");
  if(s_wl_handle != WL_INVALID_HANDLE)
  {
    Serial.println("WearLeveling already mounted.");
  }
  else
  {
    //const esp_vfs_fat_mount_config_t mount_config={true, 4, CONFIG_WL_SECTOR_SIZE}; // mount_config.max_files = 4; format_if_mount_failed = true, allocation_unit_size = CONFIG_WL_SECTOR_SIZE
    //esp_err_t err = esp_vfs_fat_spiflash_mount(base_path, "ffat", &mount_config, &s_wl_handle);
    res = wl_mount(ffat_partition, &s_wl_handle);  //esp_err_twl_mount(const esp_partition_t *partition, wl_handle_t *out_handle) 
    if (res != ESP_OK) {
        Serial.print("Failed to mount FATFFS partition WL: ");
        Serial.println( esp_err_to_name(res));
        return;
    }
    Serial.println("WL Mounted.");
  }
  Serial.print("WL Sector size: ");
  Serial.println(wl_sector_size(s_wl_handle));  //WL Sector size: 4096
  

  if(s_wl_handle != WL_INVALID_HANDLE){
    MSC.vendorID("DRM");     // max 8 chars
    MSC.productID("Watch 3"); // max 16 chars
    MSC.productRevision("3.0");   // max 4 chars
    MSC.onStartStop(modeUsbMsc_onStartStop);
    MSC.onRead(modeUsbMsc_onRead);
    MSC.onWrite(modeUsbMsc_onWrite);
    MSC.mediaPresent(true);
    MSC.begin(wl_size(s_wl_handle)/BLOCK_SIZE, BLOCK_SIZE);
  }
}

void modeUsbMscLoop()
{
  ledStatusOff();
  ledFlashlightOffTop();
  lcd()->setColorIndex(white);
  lcd()->drawBox(0, 0, 400, 240);

  lcd()->setFont(u8g2_font_10x20_t_cyrillic); // ok
  lcd()->setColorIndex(black);
  lcd()->setCursor(5, 18);
  lcd()->print(L("Доступ по USB", "USB Access"));

  drawStatusbar(363, 1, true);

  if(s_wl_handle != WL_INVALID_HANDLE)
  {

    draw_ic24_usb(180, 20, black);
    drawCentered(L("Підключіть до USB", "Connect to USB"), 60);
    drawCentered(L("та закиньте файли з компа", "and copy files from PC"), 75);

    draw_ic24_arrow_up(80, 100, black);
    drawCentered(L("Прочитано", "Read"), 90, 135);
    sprintf(buffer, L("%dK байт", "%dK bytes"), modeUsbMsc_bytesRead/1000);
    drawCentered(buffer, 90, 150);
    
    draw_ic24_arrow_down(280, 100, black);
    drawCentered(L("Записано", "Write"), 290, 135);
    sprintf(buffer, L("%dK байт", "%dK bytes"), modeUsbMsc_bytesWrite/1000);
    drawCentered(buffer, 290, 150);

    if(modeUsbMsc_errorsWrite > 0)
    {
      sprintf(buffer, L("Помилок запису: %d!", "Write errors: %d!"), modeUsbMsc_errorsWrite);
      drawCentered(buffer, 200, 170);
    }

    draw_ic16_warning(25, 206, black);
    lcd()->drawUTF8(55, 202, L("Увага! Ніколи не зберігайте", "Warning! Never store"));
    lcd()->drawUTF8(55, 217, L("на годиннику цінні файли!", "impotrant files in watch!"));
    lcd()->drawUTF8(55, 232, L("Буває, що файли втрачаються!", "Files can be lost!"));
  }
  else{
    draw_ic24_bad_file(170, 90, black);
    drawCentered(L("Розділ недоступний", "Partition error"), 150);
  }

  lcd()->setColorIndex(black);
  lcd()->drawBox(369, 0, 2, 260); // draw_ic16_repeat  draw_ic16_arrow_right  draw_ic16_back
  draw_ic16_back(lx(), ly2(), black);

  lcd()->sendBuffer();
}

void modeUsbMscExit()
{
  modeExit = 0;
  Serial.println("UNmounting WearLeveling...");
  if(s_wl_handle != WL_INVALID_HANDLE)
  {
    res = wl_unmount(s_wl_handle);  //esp_err_twl_mount(const esp_partition_t *partition, wl_handle_t *out_handle) 
    if (res != ESP_OK) {
      Serial.print("Failed to unmount partition WL: ");
      Serial.println( esp_err_to_name(res));
    }
    else
    {
      s_wl_handle = WL_INVALID_HANDLE;
      Serial.println("WL Unmounted.");
    }
    
  }
  MSC.end();
}

void modeUsbMscButtonUp()
{

}

void modeUsbMscButtonCenter()
{
  setmodeMemoryManager();
  return;
}

void modeUsbMscButtonDown()
{

}

#endif