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
#include "../AutoSleep.h"
#include "../Button.h"
#include "../ModeSerialLog.h"
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
  if (modeExit != 0)
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
  // modeFileManagerFatReady = FFat.begin(false, "/ffat", 10U, "app1");
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
  // lcd()->print("Файли: ");
  lcd()->print(modeFileManagerDir);

  drawStatusbar(363, 1, true);

  if (modeFileManagerFatReady)
  {
    fs::FS &fs = FFat;
    File dir = fs.open(F(modeFileManagerDir));
    // Serial.println(dir.name());
    if (!dir)
    {
      draw_ic24_bad_file(170, 90, black);
      drawCentered("Помилка відкриття папки", 150);
      // drawCentered("Помилка відкриття папки", 100);
      // drawCentered(modeFileManagerDir, 150);
      items = 1;
    }
    else if (!dir.isDirectory())
    {
      drawCentered("Папка не папка", 100);
      drawCentered(modeFileManagerDir, 150);
      items = 1;
    }
    else
    {
      drawListItem(itemBack, draw_ic24_back, "Назад", "В попереднє меню", firstDraw);
      int cnt = 1;
      while (true)
      {
        File file = dir.openNextFile();
        if (!file) // no more files
          break;

        Serial.println(file.name());
        if (file.isDirectory())
        {
          drawListItem(cnt, draw_ic24_folder, file.name(), "Папка", false);
        }
        else
        {
          if (strendswith(file.name(), ".txt"))
          {
            sprintf(buffer, "Текст, %d Байт", file.size());
            drawListItem(cnt, draw_ic24_file, file.name(), buffer, false);
          }
          else if (strendswith(file.name(), ".dwm"))
          {
            sprintf(buffer, "Мелодія, %d Байт", file.size());
            drawListItem(cnt, draw_ic24_music, file.name(), buffer, false);
          }
          else
          {
            sprintf(buffer, "%d Байт", file.size());
            drawListItem(cnt, draw_ic24_question, file.name(), buffer, false);
          }
        }
        file.close();
        cnt++;
      }
      items = cnt;
    }
  }
  else
  {
    draw_ic24_bad_file(170, 90, black);
    drawCentered("Файлова система пошкоджена", 150);
    // drawCentered("Файлова система пошкоджена", 100);
    drawCentered(modeFileManagerDir, 170);
    items = 1;
  }

  drawMenuLegend();

  lcd()->sendBuffer();
}
void modeFileManagerExit()
{
  FFat.end();
  modeExit = 0;
}

void modeFileManagerButtonCenter()
{
  if (selected == itemBack)
  {
    char *lastSlash = strrchr(modeFileManagerDir, '/');
    if (lastSlash == NULL || strlen(modeFileManagerDir) == 1)
    {
      setModeAppsMenu(); // exit
    }
    else
    {
      if (lastSlash != modeFileManagerDir) // not root
        *(lastSlash) = '\0';
      else // is root
        *(lastSlash + 1) = '\0';
      clearScreenAnimation();
    }
    return;
  }
  if (modeFileManagerFatReady)
  {
    fs::FS &fs = FFat;
    File dir = fs.open(F(modeFileManagerDir));
    if (dir != 0 && dir.isDirectory())
    {
      for (int index = 1; index < 1000; index++)
      {
        File file = dir.openNextFile();
        if (!file) // no more files
          break;
        if (index == selected)
        {
          if (file.isDirectory())
          {
            modeFileManagerDir = strdup(file.path());
            selected = 0;
            clearScreenAnimation();
          }
          if (!file.isDirectory() && strendswith(file.name(), ".txt"))
          {
            modeFileReaderTextPath = strdup(file.path());
            file.close();
            setmodeFileReaderText();
            break;
          }
          if (!file.isDirectory() && strendswith(file.name(), ".dwm"))
          {
            melodyPlayerSetMelodyName(String(strdup(file.name())));
            int* tmp_melody = new int[2048];// dynamically allocate memory
            tmp_melody[2047] = 19;
            int buffer_index = 0;
            int melody_index = 0;
            //fs::FS &fs = FFat;
            //File f = FFat.open(F(modeFileReaderTextPath), FILE_READ, false);
            //Serial.println(f.name());
            while(file.available())
            {
              char c = (char)file.read();
              if(isdigit(c) || c=='-'){
                buffer[buffer_index] = c;
                buffer_index ++;
              }
              else{
                if(buffer_index != 0){
                  buffer[buffer_index]='\0';
                  buffer_index = 0;
                  tmp_melody[melody_index] = atoi(buffer);
                  melody_index ++;
                }
              }
            }
            file.close(); 
            tmp_melody[melody_index] = 19;

            
            melodyPlayerPlayMelody(/*const int* melody*/tmp_melody, /*bool alarm*/false); 
            delete tmp_melody; // deallocate the memory
            tmp_melody = nullptr; // set pointer to nullptr
            break;
          }
        }
        file.close();
      }
    }
  }
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