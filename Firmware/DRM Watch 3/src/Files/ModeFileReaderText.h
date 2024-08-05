/*
Big thanks to this author:
https://github.com/Munsutari/esp32-s3-internal-flash-msc
*/
#ifndef modeFileReaderText_H
#define modeFileReaderText_H

/*PROTOTYPES*/
void modeFileReaderTextLoop();
void modeFileReaderTextExit();
void setmodeFileReaderText();
void modeFileReaderTextButtonUp();
void modeFileReaderTextButtonCenter();
void modeFileReaderTextButtonDown();

#include "Global.h"
#include "AutoSleep.h"
#include "Button.h"
#include "ModeSerialLog.h"
#include <Arduino.h>
#include "FFat.h"
#include "FS.h"

// https://github.com/espressif/esp-idf/blob/master/examples/storage/partition_api/partition_ops/main/main.c
//  Find the partition map in the partition table
char *modeFileReaderTextPath = (char*)"/";
bool modeFileReaderText_fatReady = false;
uint8_t modeFileReaderText_page=0;
const uint8_t modeFileReaderText_pageMax = 100;
size_t modeFileReaderText_pages[modeFileReaderText_pageMax];

void setmodeFileReaderText()
{
  if(modeExit != 0)
    modeExit();
  clearScreenAnimation();
  Serial.println(F("Set mode: File Manager"));
  modeSetup = setmodeFileReaderText;
  modeLoop = modeFileReaderTextLoop;
  modeExit = modeFileReaderTextExit;
  modeButtonUp = modeFileReaderTextButtonUp; // modeFileReaderTextButtonUp;
  modeButtonCenter = modeFileReaderTextButtonCenter;
  modeButtonDown = modeFileReaderTextButtonDown; // modeFileReaderTextButtonDown;
  modeButtonUpLong = 0;
  modeButtonCenterLong = 0;
  modeButtonDownLong = 0;
  registerAction();
  enableAutoReturn = true;
  enableAutoSleep = false;
  autoReturnTime = autoReturnDefaultTime;
  autoSleepTime = autoSleepDefaultTime;
  

  modeFileReaderText_fatReady = FFat.begin();
  selected = 0;
  modeFileReaderText_page = 0;
}

void modeFileReaderTextLoop()
{
  ledStatusOff();
  lcd()->setColorIndex(white);
  lcd()->drawBox(0, 0, 400, 240);

  lcd()->setFont(u8g2_font_10x20_t_cyrillic); // ok
  lcd()->setColorIndex(black);
  lcd()->setCursor(5, 18);
  //lcd()->print(getFileNameFromPath(modeFileReaderTextPath));
  if(modeFileReaderTextPath != 0)
    lcd()->print((modeFileReaderTextPath));
  

  drawStatusbar(363, 1, true);

  if (modeFileReaderText_fatReady)
  {
    fs::FS &fs = FFat;
    File f = FFat.open(F(modeFileReaderTextPath), FILE_READ, false);
    //File f = fs.open(F(modeFileReaderTextPath));
    Serial.println(f.name());
    if (!f)
    {
      drawCentered("Помилка відкриття файлу", 100);
      //drawCentered(modeFileReaderTextDir, 150);
    }
    else
    {
      lcd()->setFont(u8g2_font_10x20_t_cyrillic); // ok
      lcd()->setColorIndex(black);
      lcd()->setCursor(380, 230);
      lcd()->print(modeFileReaderText_page);

      //lcd()->setFont(u8g2_font_10x20_t_cyrillic); // ok
      lcd()->setFont(u8g2_font_unifont_t_cyrillic); //smalll
      lcd()->setColorIndex(black);
      int y=40;
      lcd()->setCursor(5, y);
      
      for(int i=0; i<modeFileReaderText_pages[modeFileReaderText_page]; i++)
      {
        if(f.available())
          f.read();
      }

      while(f.available())
      {
        char c = (char)f.read();
        if(c == '\n' || lcd()->getCursorX() > 350)
        {
          y += 13;
          lcd()->setCursor(5, y);
        }
        if(y > 238){
          break;
          
        }
        lcd()->print(c);
      }
      modeFileReaderText_pages[modeFileReaderText_page+1] = f.position();
      f.close(); 
    }
  }
  else{
    drawCentered("Файлова система пошкоджена", 100);
    if(modeFileReaderTextPath != 0)
      drawCentered(modeFileReaderTextPath, 150);
  }
  //     int cnt = 0;
  //     while (true)
  //     {
  //       File file = dir.openNextFile();
  //       if (!file) // no more files
  //         break;
  //       Serial.println(file.name());
  //       if (file.isDirectory())
  //       {
  //         drawListItem(cnt,   draw_ic24_apps,   file.name(),    "Папка",      false);
  //       }
  //       else
  //       {
  //         sprintf(buffer, "%d Байт", file.size());
  //         drawListItem(cnt,   draw_ic24_life,   file.name(),    buffer,      false);
  //       }
  //       file.close();
  //       cnt++;
  //     }
  //     items = cnt;
  //   }
  // }
  // else
  // {
  //   drawCentered("Текстовий файл", 100);
  //   drawCentered(modeFileReaderTextPath, 150);
  // }

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

void modeFileReaderTextExit(){
  free(modeFileReaderTextPath);
  FFat.end();
  modeExit = 0;
}

// const char* modeFileReaderTextMenuItems(int index){
//   if(index == 0) return "Завантажити файли по USB";
//   if(index == 1) return "Форматувати пам'ять";
//   if(index == 2) return "Показати таблицю розділів";
//   return "---";
// }
// void modeFileReaderTextMenuSelected(){
//   int index = ModeListSelection_Selected;
//   setmodeFileReaderText();
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
void modeFileReaderTextButtonUp()
{
  if(modeFileReaderText_page > 0)
    modeFileReaderText_page --;
  //checkPartitionRead();
  //showPartitions();
  //selected ++;
}
void modeFileReaderTextButtonCenter()
{
  // MSC.end();
  // partition = NULL;
  FFat.end();
  //setModeAppsMenu(); 
  setmodeFileManager(); // exit
}
void modeFileReaderTextButtonDown()
{
  if(modeFileReaderText_pages[modeFileReaderText_page+1] != modeFileReaderText_pages[modeFileReaderText_page] && modeFileReaderText_page < modeFileReaderText_pageMax-1)
    modeFileReaderText_page ++;
  //selected --;
  // ModeListSelection_Items = modeFileReaderTextMenuItems;
  // ModeListSelection_Name = "Меню нотаток";
  // ModeListSelection_Cnt = 5;
  // ModeListSelection_Selected = 0;//getActionArgument(ModeShortcutEventSettings_EventId);
  // ModeListSelection_OnSelected = modeFileReaderTextMenuSelected;
  // setModeListSelection();
  //checkPartitionWrite();
  //initPartition();
}

#endif