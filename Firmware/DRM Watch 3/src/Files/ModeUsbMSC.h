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
// https://github.com/espressif/esp-idf/blob/master/examples/storage/partition_api/partition_ops/main/main.c
//  Find the partition map in the partition table
const esp_partition_t *modeUsbMsc_partition = NULL;
uint32_t modeUsbMsc_bytesRead = 0;
uint32_t modeUsbMsc_bytesWrite = 0;

//bool showMenu = true;
//bool showFormatting = false;
//bool showLog = true;
//bool showMsc = false;

static int32_t modeUsbMsc_onWrite(uint32_t lba, uint32_t offset, uint8_t* buffer, uint32_t bufsize){
  ledStatusOn();
  modeUsbMsc_bytesWrite += bufsize;
  esp_partition_erase_range(modeUsbMsc_partition, offset + (lba * BLOCK_SIZE), bufsize);
  esp_partition_write(modeUsbMsc_partition, offset + (lba * BLOCK_SIZE), (uint32_t*)buffer, bufsize);
  return bufsize;
}

static int32_t modeUsbMsc_onRead(uint32_t lba, uint32_t offset, void* buffer, uint32_t bufsize){
  ledStatusOn();
  modeUsbMsc_bytesRead += bufsize;
  esp_partition_read(modeUsbMsc_partition, offset + (lba * BLOCK_SIZE), (uint32_t*)buffer, bufsize);
  return bufsize;
}

static bool modeUsbMsc_onStartStop(uint8_t power_condition, bool start, bool load_eject){
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
  //selected = 0;
  //items = 4;

  //showHelpText = true;

  //u8g2log.begin(LOG_W, LOG_H, u8log_buffer);
  //u8g2log.setLineHeightOffset(0);
  
  //partition = esp_partition_find_first(ESP_PARTITION_TYPE_DATA, ESP_PARTITION_SUBTYPE_ANY, "spiffs");
  //modeUsbMsc_partition = esp_partition_find_first(ESP_PARTITION_TYPE_DATA, ESP_PARTITION_SUBTYPE_ANY, "spiffs");
  //modeUsbMsc_partition = esp_partition_find_first(ESP_PARTITION_TYPE_DATA, ESP_PARTITION_SUBTYPE_DATA_FAT, NULL); //fatffs
  // partition = esp_partition_find_first(ESP_PARTITION_TYPE_APP, ESP_PARTITION_SUBTYPE_ANY, "app1");
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


  // if(showLog) // DRAW LOG
  // {
  //   lcd()->setColorIndex(black);
  //   lcd()->setFont(u8g2_font_unifont_t_cyrillic); // smalll
  //   lcd()->drawLog(2, 39, u8g2log);
  // }


  // if(showMenu)
  // {
  //   drawListItem(itemBack,                              draw_ic24_back,      "Назад",                 "До головного меню",      false);
  //   drawListItem(modeUsbMscItemUpload,           draw_ic24_usb,       "Підключити до USB",     "Щоб закинути файли з компа",      false);
  //   drawListItem(modeUsbMscItemShowPartitions,   draw_ic24_partitions,"Показати карту розділів",     "Щоб закинути файли з компа",      false);
  //   drawListItem(modeUsbMscItemFormatPartition,  draw_ic24_clean,     "Форматувати розділ",     "Щоб закинути файли з компа",      false);
  // }
  
  //if (showHelpText)
  // {
  if(modeUsbMsc_partition != NULL){
    draw_ic24_usb(180, 60, black);
    drawCentered("Підключіть до USB", 110);
    drawCentered("та закиньте файли з компа", 130);

    draw_ic24_arrow_up(80, 170, black);
    drawCentered("Прочитано", 90, 210);
    sprintf(buffer, "%dK байт", modeUsbMsc_bytesRead/1000);
    drawCentered(buffer, 90, 230);
    
    draw_ic24_arrow_down(280, 170, black);
    drawCentered("Записано", 290, 210);
    sprintf(buffer, "%dK байт", modeUsbMsc_bytesWrite/1000);
    drawCentered(buffer, 290, 230);
  }
  else{
    draw_ic24_bad_file(170, 90, black);
    drawCentered("Розділ недоступний", 150);
  }
  //   if (partition != NULL)
  //   {
  //     drawCentered(partition->label, 200);
  //     itoa(partition->size, buffer, 10);
  //     drawCentered(buffer, 220);
  //     itoa(SPI_FLASH_SEC_SIZE, buffer, 10);
  //     drawCentered(buffer, 240);
  //   }
  //   else
  //     drawCentered("Розділ не знайдено", 220);
  // }

  

  //drawMenuLegend();
  lcd()->setColorIndex(black);
  lcd()->drawBox(369, 0, 2, 260); // draw_ic16_repeat  draw_ic16_arrow_right  draw_ic16_back
  //draw_ic16_hashtag(lx(), ly1(), black);
  draw_ic16_back(lx(), ly2(), black);
  //draw_ic16_warning(lx(), ly3(), black);
  //draw_ic16_menu(lx(), ly3(), black);

  lcd()->sendBuffer();
}
void modeUsbMscExit()
{
  modeExit = 0;
  MSC.end();
}

// void showPartitions()
// {
//   //showHelpText = false;
//   u8g2log.printf("\n======== ESP32 Partition table: =======\n\n");
//   modeUsbMscLoop();

//   u8g2log.printf("|Type|Sub| Offset |  Size  |  Label   |\n");
//   modeUsbMscLoop();
//   u8g2log.printf("|----|---|--------|--------|----------|\n");
//   modeUsbMscLoop();

//   esp_partition_iterator_t pi = esp_partition_find(ESP_PARTITION_TYPE_ANY, ESP_PARTITION_SUBTYPE_ANY, NULL);
//   if (pi != NULL)
//   {
//     do
//     {
//       const esp_partition_t *p = esp_partition_get(pi);
//       u8g2log.printf("| %02x |%02x |0x%06X|0x%06X|%-10s|\r\n",
//                      p->type, p->subtype, p->address, p->size, p->label);
//       modeUsbMscLoop();
//     } while (pi = (esp_partition_next(pi)));
//   }
//   u8g2log.printf("|----|---|--------|--------|----------|\n");
//   modeUsbMscLoop();
// }
// const char* modeUsbMscMenuItems(int index){
//   if(index == 0) return "Завантажити файли по USB";
//   if(index == 1) return "Форматувати пам'ять";
//   if(index == 2) return "Показати таблицю розділів";
//   return "---";
// }
// void modeUsbMscMenuSelected(){
//   int index = ModeListSelection_Selected;
//   setmodeUsbMsc();
//   if(index == 0)  //return "Завантажити файли по USB";
//   {

//   }
//   if(index == 1) //return "Форматувати пам'ять";
//   { 

//   } 
//   if(index == 2) //return "Показати таблицю розділів";
//   {

//   }
// }
void modeUsbMscButtonUp()
{
  //if(showMenu)
    //modeMainMenuButtonUp();
  //checkPartitionRead();
  //showPartitions();
}
void modeUsbMscButtonCenter()
{
  //if(showMenu){
    //if(selected == itemBack){
      setmodeMemoryManager();
      return;
    //}
    // if(selected == modeUsbMscItemFormatPartition){
    //   drawMessage("Форматування...", "fatffs", true);
    //   if(FFat.format(FFAT_WIPE_FULL)){
    //     drawMessage("Успішно!", "відформатовано fatffs", true);
    //   } else {
    //     drawMessage("Помилка", "форматування fatffs", true);
    //   }
    //   return;
    // }
  //}
  //partition = NULL;
  //setModeAppsMenu(); // exit
}
void modeUsbMscButtonDown()
{
  // if(showMenu)
  //   modeMainMenuButtonDown();
  // ModeListSelection_Items = modeUsbMscMenuItems;
  // ModeListSelection_Name = "Меню нотаток";
  // ModeListSelection_Cnt = 5;
  // ModeListSelection_Selected = 0;//getActionArgument(ModeShortcutEventSettings_EventId);
  // ModeListSelection_OnSelected = modeUsbMscMenuSelected;
  // setModeListSelection();
  //checkPartitionWrite();
  //initPartition();
}



#endif