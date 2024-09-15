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
void openFile(const char *path);
void playDwmMelody(const char *path);
void playWavMelody(const char *path);

// https://github.com/espressif/esp-idf/blob/master/examples/storage/partition_api/partition_ops/main/main.c
//https://github.com/pcbreflux/espressif/blob/master/esp32/app/ESP32_sd_card/main/sd_card_example_main.c  
//  Find the partition map in the partition table
RTC_DATA_ATTR const char *modeFileManagerDir = "/spi";
Runnable modeFileManagerOnFileSelected = 0;
Runnable modeFileManagerOnCancel = 0;
char modeFileManagerFileSelected[256];
struct dirent *pDirent;
struct stat _stat;

#include "Global.h"
#include "../AutoSleep.h"
#include "../Button.h"
#include "../ModeSerialLog.h"
#include <Arduino.h>
#include "ModeFileReaderText.h"
#include "ModeFileReaderBmp.h"
#include "ModeMemoryManager.h"
#include "MelodyPlayer.h"
#include "esp_vfs.h"
#include "esp_vfs_fat.h"
#include "esp_system.h"

void setmodeFileManager()
{
  if (modeExit != 0)
    modeExit();
  clearScreenAnimation();
  Serial.println(F("Set mode: File Manager"));
  modeSetup = setmodeFileManager;
  modeLoop = modeFileManagerLoop;
  modeExit = modeFileManagerExit;
  modeButtonUp = globalMenuButtonUp; // modeFileManagerButtonUp;
  modeButtonCenter = modeFileManagerButtonCenter;
  modeButtonDown = globalMenuButtonDown; // modeFileManagerButtonDown;
  modeButtonUpLong = 0;
  modeButtonCenterLong = 0;
  modeButtonDownLong = 0;
  registerAction();
  enableAutoReturn = true;
  enableAutoSleep = false;
  autoReturnTime = autoReturnDefaultTime;
  autoSleepTime = autoSleepDefaultTime;

  initFat();
}

void modeFileManagerLoop()
{
  ledStatusOff();
  lcd()->setColorIndex(white);
  lcd()->drawBox(0, 0, 400, 240);

  lcd()->setFont(u8g2_font_10x20_t_cyrillic); // ok
  lcd()->setColorIndex(black);
  lcd()->setCursor(5, 18);
  lcd()->print(L("Файли: ", "Files: "));
  lcd()->print(modeFileManagerDir);

  drawStatusbar(363, 1, true);

  if (fatReady())
  {
    DIR *dir = opendir(modeFileManagerDir);
    if (!dir)
    {
      draw_ic24_bad_file(170, 90, black);
      drawCentered(L("Помилка відкриття папки", "Error opening folder"), 150);
      drawCentered(modeFileManagerDir, 170);
      items = 1;
    }
    else
    {
      drawListItem(itemBack, draw_ic24_back, L("Назад", "Back"), L("В попереднє меню", "To previous menu"), firstDraw);
      int cnt=1;
      while((pDirent = readdir(dir)) != NULL)
      {
        sprintf(buffer, "%s/%s", modeFileManagerDir, pDirent->d_name);
        stat(buffer, &_stat);
        if (S_ISDIR(_stat.st_mode))
        {
          drawListItem(cnt, draw_ic24_folder, pDirent->d_name, L("Папка", "Folder"), false);
        }
        else
        {
          if (strendswith(pDirent->d_name, ".txt"))
          {
            sprintf(buffer, L("Текст, %d байт", "Text, %d bytes"), _stat.st_size);
            drawListItem(cnt, draw_ic24_file, pDirent->d_name, buffer, false);
          }
          else if (strendswith(pDirent->d_name, ".dwm"))
          {
            sprintf(buffer, L("Мелодія, %d байт", "Melody, %d bytes"), _stat.st_size);
            drawListItem(cnt, draw_ic24_music, pDirent->d_name, buffer, false);
          }
          else if (strendswith(pDirent->d_name, ".wav"))
          {
            sprintf(buffer, L("Аудіо, %d байт", "Audio, %d bytes"), _stat.st_size);
            drawListItem(cnt, draw_ic24_music, pDirent->d_name, buffer, false);
          }
          else if (strendswith(pDirent->d_name, ".bmp"))
          {
            sprintf(buffer, L("Картинка, %d байт", "Image, %d bytes"), _stat.st_size);
            drawListItem(cnt, draw_ic24_image, pDirent->d_name, buffer, false);
          }
          else
          {
            sprintf(buffer, L("Файл, %d байт", "File, %d bytes"), _stat.st_size);
            drawListItem(cnt, draw_ic24_question, pDirent->d_name, buffer, false);
          }
        }
        cnt++;
      }
      items = cnt;
      closedir(dir);
    }
    // because often filenames is too long, have to clear space after filenames bar
    lcd()->setColorIndex(white);
    lcd()->drawBox(355, 30, 45, 210);
  }
  else
  {
    draw_ic24_bad_file(170, 90, black);
    drawCentered(L("Файлова система пошкоджена", "File system damaged"), 150);
    drawCentered(modeFileManagerDir, 170);
    items = 1;
  }

  drawMenuLegend();

  lcd()->sendBuffer();
}

void modeFileManagerExit()
{
  modeExit = 0;
  exitFat();
}

void modeFileManagerButtonCenter()
{
  if (selected == itemBack)
  {
    char *lastSlash = strrchr(modeFileManagerDir, '/');
    if (lastSlash == NULL || strlen(modeFileManagerDir) <= 4)
    {
      if(modeFileManagerOnCancel != 0){
        modeFileManagerOnCancel();
        modeFileManagerOnCancel = 0;
        modeFileManagerOnFileSelected = 0;
      }
      else{
        setModeAppsMenu(); // exit
      }
    }
    else
    {
      if (lastSlash != modeFileManagerDir) // not root
        *(lastSlash) = '\0';
      else // is root
        *(lastSlash + 1) = '\0';
      clearScreenAnimation();
      selected = 0;
    }
    return;
  }
  if (!modeFileManagerFatReady)
    return;
  
  DIR *dir = opendir(modeFileManagerDir);
  if(dir)
  {
    for (int cnt=1; (pDirent = readdir(dir)) != NULL; cnt++)
    {
      if(cnt == selected)
      {
        sprintf(modeFileManagerFileSelected, "%s/%s", modeFileManagerDir, pDirent->d_name);
        stat(modeFileManagerFileSelected, &_stat);
        if (S_ISDIR(_stat.st_mode))  //is folder
        {
          modeFileManagerDir = strdup(modeFileManagerFileSelected);
          selected = 0;
          clearScreenAnimation();
        }
        else    //is file
        {
          if(modeFileManagerOnFileSelected != 0){
            modeFileManagerOnFileSelected();
            modeFileManagerOnFileSelected = 0;
            modeFileManagerOnCancel = 0;
          }
          else{
            openFile(modeFileManagerFileSelected);
          }
          break;
        }
      }
    }
    closedir(dir);
  }
}

void openFile(const char *path)
{
  if(strlen(path) == 0)
    return;
  if (strendswith(path, ".txt"))
  {
    modeFileReaderTextPath = strdup(path);
    setmodeFileReaderText();
  }
  else if (strendswith(path, ".dwm"))
  {
    const char *melodyPath = strdup(path);
    playDwmMelody(melodyPath);
  }
  else if (strendswith(path, ".wav"))
  {
    const char *melodyPath = strdup(path);
    playWavMelody(melodyPath);
  }
  else if (strendswith(path, ".bmp"))
  {
    strcpy(modeFileReaderBmpPath, path);
    setmodeFileReaderBmp();
  }
  else
  {
    drawDim();
    drawMessageAnimated(L("Формат не підтримується", "Unsupported format"));
  }
}

void playDwmMelody(const char *path)
{
  initFat();
  FILE* f = fopen(path, "r");
  if(f == NULL){
    drawMessage(L("Помилка відкриття файлу", "Error opening file"), path, true);
    return;
  }
  melodyPlayerSetMelodyName(String(strdup(strrchr(path, '/'))));
  const int melody_max = 2048;
  int *tmp_melody = new int[melody_max]; // dynamically allocate memory
  int buffer_index = 0;
  int melody_index = 0;
  while (feof(f) == 0)
  {
    char c = getc(f);
    if (isdigit(c) || c == '-')
    {
      buffer[buffer_index] = c;
      if (buffer_index < BUFFER_SIZE)
        buffer_index++;
    }
    else
    {
      if (buffer_index != 0)
      {
        buffer[buffer_index] = '\0';
        buffer_index = 0;
        tmp_melody[melody_index] = atoi(buffer);
        melody_index++;
      }
    }
    if (melody_index >= melody_max)
      break;
  }
  fclose(f);
  if (melody_index > 0)
  {
    tmp_melody[melody_index] = 19;
    tmp_melody[melody_max - 1] = 19;
    melodyPlayerPlayMelody(/*const int* melody*/ tmp_melody, /*bool alarm*/ false);
  }
  else
  {
    drawDim();
    drawMessageAnimated(L("Битий файл", "Corrupted file"));
  }
  delete tmp_melody;    // deallocate the memory
  tmp_melody = nullptr; // set pointer to nullptr
}

void playWavMelody(const char *path)
{ 
  while (isButtonCenterPressed());
  initFat();
  bool repeat = false;
  unsigned long lastTick = micros();
  draw_ic16_empty(lx(), ly3(), black);
  drawDim();
  draw_ic16_repeat(lx(), ly1(), black);
  draw_ic16_back(lx(), ly2(), black);
  char *lastSlash = strrchr(path, '/');
  drawMessage(L("Відтворення...", "Playing..."), lastSlash, false);
  lcd()->sendBuffer();
  ledStatusOn();
  backlightOff();
  do
  {
    FILE* f = fopen(path, "r");
    if(f == NULL)
    {
      drawMessage(L("Помилка відкриття файлу", "Error opening file"), path, true);
      return;
    }
    ledcSetup(/*channel*/ 0, /*freq*/ 24000, /*PWM_Res*/ 8);

    while(feof(f) == 0)
    {
      char c = getc(f);
      ledcWrite(/*channel*/ 0, /*value*/ c);
      while (micros() - lastTick < 120)
        ;
      lastTick = micros();
      if (isButtonUpPressed())
      {
        repeat = !repeat;
        if (repeat)
          draw_ic16_check(lx(), ly1(), black);
        else
          draw_ic16_repeat(lx(), ly1(), black);
        lcd()->sendBuffer();
        while (isButtonUpPressed());
      }
      if (isButtonCenterPressed())
      {
        buttonBeep();
        repeat = false;
        registerAction(); //to brevent auto exit after melody played
        break;
      }
    }
    fclose(f);
  } while (repeat);

  ledStatusOff();
  buzNoTone();
  clearScreenAnimation();
  registerAction();
  while (isButtonCenterPressed());
}
#endif