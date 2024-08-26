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
#include <Arduino.h>
#include "USB.h"
#include "USBMSC.h"

USBMSC MSC;
// Block size of flash memory (in bytes) (4KB)
#define BLOCK_SIZE 4096  
uint8_t page_buffer[BLOCK_SIZE];
// https://github.com/espressif/esp-idf/blob/master/examples/storage/partition_api/partition_ops/main/main.c
const esp_partition_t *modeUsbMsc_partition = NULL;
uint32_t modeUsbMsc_bytesRead = 0;
uint32_t modeUsbMsc_bytesWrite = 0;
uint32_t modeUsbMsc_errorsWrite = 0;


static int32_t modeUsbMsc_onWrite(uint32_t lba, uint32_t offset, uint8_t* buffer, uint32_t bufsize){
  for(int retries = 0; retries < 5; retries++){
    ledStatusOn();
    modeUsbMsc_bytesWrite += bufsize;
    esp_partition_erase_range(modeUsbMsc_partition, offset + (lba * BLOCK_SIZE), bufsize);
    esp_partition_write(modeUsbMsc_partition, offset + (lba * BLOCK_SIZE), buffer,      bufsize);
    esp_partition_read (modeUsbMsc_partition, offset + (lba * BLOCK_SIZE), page_buffer, bufsize);
    if(memcmp (buffer, page_buffer, bufsize) == 0)
      return bufsize;
    modeUsbMsc_errorsWrite ++;
  }
  return 0;
}

static int32_t modeUsbMsc_onRead(uint32_t lba, uint32_t offset, void* buffer, uint32_t bufsize){
  ledStatusOn();
  modeUsbMsc_bytesRead += bufsize;
  esp_partition_read(modeUsbMsc_partition, offset + (lba * BLOCK_SIZE), buffer, bufsize);       //(uint32_t*)
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
  
  //modeUsbMsc_partition = esp_partition_find_first(ESP_PARTITION_TYPE_DATA, ESP_PARTITION_SUBTYPE_ANY, "spiffs");
  //modeUsbMsc_partition = esp_partition_find_first(ESP_PARTITION_TYPE_APP, ESP_PARTITION_SUBTYPE_ANY, "app1");
  modeUsbMsc_partition = esp_partition_find_first(ESP_PARTITION_TYPE_DATA, ESP_PARTITION_SUBTYPE_DATA_FAT, NULL); //fatffs

  if(modeUsbMsc_partition != NULL){
    MSC.vendorID("DRM");     // max 8 chars
    MSC.productID("Watch 3"); // max 16 chars
    MSC.productRevision("3.0");   // max 4 chars
    MSC.onStartStop(modeUsbMsc_onStartStop);
    MSC.onRead(modeUsbMsc_onRead);
    MSC.onWrite(modeUsbMsc_onWrite);
    MSC.mediaPresent(true);
    MSC.begin(modeUsbMsc_partition->size/BLOCK_SIZE, BLOCK_SIZE);
  }
}

void modeUsbMscLoop()
{
  ledStatusOff();
  lcd()->setColorIndex(white);
  lcd()->drawBox(0, 0, 400, 240);

  lcd()->setFont(u8g2_font_10x20_t_cyrillic); // ok
  lcd()->setColorIndex(black);
  lcd()->setCursor(5, 18);
  lcd()->print("Доступ по USB");

  drawStatusbar(363, 1, true);

  if(modeUsbMsc_partition != NULL){

    draw_ic24_usb(180, 20, black);
    drawCentered("Підключіть до USB", 60);
    drawCentered("та закиньте файли з компа", 75);

    draw_ic24_arrow_up(80, 100, black);
    drawCentered("Прочитано", 90, 135);
    sprintf(buffer, "%dK байт", modeUsbMsc_bytesRead/1000);
    drawCentered(buffer, 90, 150);
    
    draw_ic24_arrow_down(280, 100, black);
    drawCentered("Записано", 290, 135);
    sprintf(buffer, "%dK байт", modeUsbMsc_bytesWrite/1000);
    drawCentered(buffer, 290, 150);

    if(modeUsbMsc_errorsWrite > 0)
    {
      sprintf(buffer, "Помилок запису: %d!", modeUsbMsc_errorsWrite);
      drawCentered(buffer, 200, 170);
    }

    draw_ic16_warning(25, 206, black);
    lcd()->drawUTF8(55, 202, "Увага! Ніколи не зберігайте");
    lcd()->drawUTF8(55, 217, "на годиннику цінні файли!");
    lcd()->drawUTF8(55, 232, "Буває, що файли втрачаються!");
  }
  else{
    draw_ic24_bad_file(170, 90, black);
    drawCentered("Розділ недоступний", 150);
  }

  lcd()->setColorIndex(black);
  lcd()->drawBox(369, 0, 2, 260); // draw_ic16_repeat  draw_ic16_arrow_right  draw_ic16_back
  draw_ic16_back(lx(), ly2(), black);

  lcd()->sendBuffer();
}
void modeUsbMscExit()
{
  modeExit = 0;
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