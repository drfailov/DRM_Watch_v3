
#ifndef modePartitionList_H
#define modePartitionList_H

const int modePartitionListItemUpload = 1;
const int modePartitionListItemShowPartitions = 2;
const int modePartitionListItemFormatPartition = 3;


/*PROTOTYPES*/
void modePartitionListLoop();
void modePartitionListExit();
void setmodePartitionList();
void modePartitionListButtonUp();
void modePartitionListButtonCenter();
void modePartitionListButtonDown();
void showPartitions();



#include "Global.h"
#include "AutoSleep.h"
#include "Button.h"
#include "ModeSerialLog.h"
#include "ModeUsbMSC.h"
#include <Arduino.h>



void setmodePartitionList()
{
  if(modeExit != 0)
    modeExit();
  clearScreenAnimation();
  Serial.println(F("Set mode: Partition List"));
  modeSetup = setmodePartitionList;
  modeLoop = modePartitionListLoop;
  modeExit = modePartitionListExit;
  modeButtonUp = modePartitionListButtonUp;
  modeButtonCenter = modePartitionListButtonCenter;
  modeButtonDown = modePartitionListButtonDown;
  modeButtonUpLong = 0;
  modeButtonCenterLong = 0;
  modeButtonDownLong = 0;
  registerAction();
  enableAutoReturn = true;
  enableAutoSleep = false;
  autoReturnTime = autoReturnDefaultTime;
  autoSleepTime = autoSleepDefaultTime;
  
  u8g2log.begin(LOG_W, LOG_H, u8log_buffer);
  u8g2log.setLineHeightOffset(0);

  showPartitions();
}

void modePartitionListLoop()
{
  lcd()->setColorIndex(white);
  lcd()->drawBox(0, 0, 400, 240);

  lcd()->setFont(u8g2_font_10x20_t_cyrillic); // ok
  lcd()->setColorIndex(black);
  lcd()->setCursor(5, 18);
  lcd()->print(L("Карта розділів", "Partition map"));

  drawStatusbar(363, 1, true);


  lcd()->setColorIndex(black);
  lcd()->setFont(u8g2_font_unifont_t_cyrillic); // smalll
  lcd()->drawLog(2, 39, u8g2log);

  lcd()->setColorIndex(black);
  lcd()->drawBox(369, 0, 2, 260); // draw_ic16_repeat  draw_ic16_arrow_right  draw_ic16_back
  draw_ic16_back(lx(), ly2(), black);

  lcd()->sendBuffer();
}
void modePartitionListExit()
{

}

void showPartitions()
{
  //showHelpText = false;
  u8g2log.printf("\n======== ESP32 Partition table: =======\n\n");
  modePartitionListLoop();

  u8g2log.printf("|Type|Sub| Offset |  Size  |  Label   |\n");
  modePartitionListLoop();
  u8g2log.printf("|----|---|--------|--------|----------|\n");
  modePartitionListLoop();

  esp_partition_iterator_t pi = esp_partition_find(ESP_PARTITION_TYPE_ANY, ESP_PARTITION_SUBTYPE_ANY, NULL);
  if (pi != NULL)
  {
    do
    {
      const esp_partition_t *p = esp_partition_get(pi);
      u8g2log.printf("| %02x |%02x |0x%06X|0x%06X|%-10s|\r\n",
                     p->type, p->subtype, p->address, p->size, p->label);
      modePartitionListLoop();
    } while (pi = (esp_partition_next(pi)));
  }
  u8g2log.printf("|----|---|--------|--------|----------|\n");
  modePartitionListLoop();
}

void modePartitionListButtonUp()
{
  
}
void modePartitionListButtonCenter()
{
  setmodeMemoryManager();
}
void modePartitionListButtonDown()
{

}



#endif