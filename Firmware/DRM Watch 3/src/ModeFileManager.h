/*
Big thanks to this author:
https://github.com/Munsutari/esp32-s3-internal-flash-msc
*/
#ifndef modeFileManager_H
#define modeFileManager_H

/*PROTOTYPES*/
void modeFileManagerLoop();
void modeFileManagerExit();
void setmodeFileManager();
void modeFileManagerButtonUp();
void modeFileManagerButtonCenter();
void modeFileManagerButtonDown();

#include "Global.h"
#include "AutoSleep.h"
#include "Button.h"
#include "ModeSerialLog.h"
#include <Arduino.h>
#include "FFat.h"
#include "FS.h"
#include "ModeFileReaderText.h"

// https://github.com/espressif/esp-idf/blob/master/examples/storage/partition_api/partition_ops/main/main.c
//  Find the partition map in the partition table
const char *modeFileManagerDir = "/";
bool modeFileManagerFatReady = false;

void setmodeFileManager()
{
  if(modeExit != 0)
    modeExit();
  clearScreenAnimation();
  Serial.println(F("Set mode: File Manager"));
  modeSetup = setmodeFileManager;
  modeLoop = modeFileManagerLoop;
  modeExit = modeFileManagerExit;
  modeButtonUp = modeMainMenuButtonUp; // modeFileManagerButtonUp;
  modeButtonCenter = modeFileManagerButtonCenter;
  modeButtonDown = modeMainMenuButtonDown; // modeFileManagerButtonDown;
  modeButtonUpLong = 0;
  modeButtonCenterLong = 0;
  modeButtonDownLong = 0;
  registerAction();
  enableAutoReturn = true;
  enableAutoSleep = false;
  autoReturnTime = autoReturnDefaultTime;
  autoSleepTime = autoSleepDefaultTime;
  

  modeFileManagerFatReady = FFat.begin();
  selected = 0;
}

void modeFileManagerLoop()
{
  ledStatusOff();
  lcd()->setColorIndex(white);
  lcd()->drawBox(0, 0, 400, 240);

  lcd()->setFont(u8g2_font_10x20_t_cyrillic); // ok
  lcd()->setColorIndex(black);
  lcd()->setCursor(5, 18);
  lcd()->print("Файли: ");
  lcd()->print(modeFileManagerDir);

  drawStatusbar(363, 1, true);

  if (modeFileManagerFatReady)
  {
    fs::FS &fs = FFat;
    File dir = fs.open(F(modeFileManagerDir));
    //Serial.println(dir.name());
    if (!dir)
    {
      drawCentered("Помилка відкриття папки", 100);
      drawCentered(modeFileManagerDir, 150);
    }
    else if (!dir.isDirectory())
    {
      drawCentered("Папка не папка", 100);
      drawCentered(modeFileManagerDir, 150);
    }
    else{
      int cnt = 0;
      while (true)
      {
        File file = dir.openNextFile();
        if (!file) // no more files
          break;
        Serial.println(file.name());
        if (file.isDirectory())
        {
          drawListItem(cnt,   draw_ic24_apps,   file.name(),    "Папка",      false);
        }
        else
        {
          if(strendswith(file.name(), ".txt"))
            sprintf(buffer, "Текст %d Байт", file.size());
          else
            sprintf(buffer, "%d Байт", file.size());
          drawListItem(cnt,   draw_ic24_life,   file.name(),    buffer,      false);
        }
        file.close();
        cnt++;
      }
      items = cnt;
    }
  }
  else
  {
    drawCentered("Файлова система пошкоджена", 100);
    drawCentered(modeFileManagerDir, 150);
  }

  // if (showHelpText)
  // {
  //   // drawCentered("Підключіть до USB", 100);
  //   // drawCentered("та закиньте текстові файли з компа", 140);
  //   // if (partition != NULL)
  //   // {
  //   //   drawCentered(partition->label, 200);
  //   //   itoa(partition->size, buffer, 10);
  //   //   drawCentered(buffer, 220);
  //   //   itoa(SPI_FLASH_SEC_SIZE, buffer, 10);
  //   //   drawCentered(buffer, 240);
  //   // }
  //   // else
  //   //   drawCentered("Розділ не знайдено", 220);
  // }

  // if(showLog) // DRAW LOG
  // {
  //   lcd()->setColorIndex(black);
  //   lcd()->setFont(u8g2_font_unifont_t_cyrillic); // smalll
  //   lcd()->drawLog(2, 39, u8g2log);
  // }

  // lcd()->setColorIndex(black);
  // lcd()->drawBox(369, 0, 2, 260); // draw_ic16_repeat  draw_ic16_arrow_right  draw_ic16_back
  // draw_ic16_hashtag(lx(), ly1(), black);
  // draw_ic16_back(lx(), ly2(), black);
  // draw_ic16_warning(lx(), ly3(), black);
  // draw_ic16_menu(lx(), ly3(), black);
  drawMenuLegend();

  lcd()->sendBuffer();
}
void modeFileManagerExit(){
  FFat.end();
  modeExit = 0;
}

// const char* modeFileManagerMenuItems(int index){
//   if(index == 0) return "Завантажити файли по USB";
//   if(index == 1) return "Форматувати пам'ять";
//   if(index == 2) return "Показати таблицю розділів";
//   return "---";
// }
// void modeFileManagerMenuSelected(){
//   int index = ModeListSelection_Selected;
//   setmodeFileManager();
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
// void modeFileManagerButtonUp()
// {
//   //checkPartitionRead();
//   //showPartitions();
//   selected ++;
// }
void modeFileManagerButtonCenter()
{
  if(selected == 0){
    FFat.end();
    setModeAppsMenu(); // exit
    return;
  }
  if (modeFileManagerFatReady)
  {
    fs::FS &fs = FFat;
    File dir = fs.open(F(modeFileManagerDir));
    if(dir != 0 && dir.isDirectory()){
      for(int index=0; index<1000; index++){
        File file = dir.openNextFile();
        //dir.getNextFileName
        if (!file) // no more files
          break;
        if(index == selected){
          if(strendswith(file.name(), ".txt"))
          {
            //char *b = strdup(file.path());
            // size_t len = strlen(file.path()); // Doesn't include null-terminator character
            // char *b = new char[len+1];
            // memcpy(b, file.path(), len); // Assumes plain ASCII string
            // b[len] = '\0';
            modeFileReaderTextPath = strdup(file.path());
            file.close();
            
            setmodeFileReaderText();
            break;
          }
        }
        file.close();
      }
    }
  }
      // int cnt = 0;
      // while (true)
      // {
        
      //   Serial.println(file.name());
      //   if (file.isDirectory())
      //   {
      //     drawListItem(cnt,   draw_ic24_apps,   file.name(),    "Папка",      false);
      //   }
      //   else
      //   {
      //     sprintf(buffer, "%d Байт", file.size());
      //     drawListItem(cnt,   draw_ic24_life,   file.name(),    buffer,      false);
      //   }
        
      //   cnt++;
      // }
      // items = cnt;
    //}
  // }
  // else
  // {
  //   drawCentered("Файлова система пошкоджена", 100);
  //   drawCentered(modeFileManagerDir, 150);
  // }
  // MSC.end();
  // partition = NULL;
}
// void modeFileManagerButtonDown()
// {
//   selected --;
//   // ModeListSelection_Items = modeFileManagerMenuItems;
//   // ModeListSelection_Name = "Меню нотаток";
//   // ModeListSelection_Cnt = 5;
//   // ModeListSelection_Selected = 0;//getActionArgument(ModeShortcutEventSettings_EventId);
//   // ModeListSelection_OnSelected = modeFileManagerMenuSelected;
//   // setModeListSelection();
//   //checkPartitionWrite();
//   //initPartition();
// }

#endif