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
void playDwmMelody(const char* path);


#include "Global.h"
#include "../AutoSleep.h"
#include "../Button.h"
#include "../ModeSerialLog.h"
#include <Arduino.h>
#include "FFat.h"
#include "FS.h"
#include "ModeFileReaderText.h"
#include "ModeFileReaderBmp.h"

// https://github.com/espressif/esp-idf/blob/master/examples/storage/partition_api/partition_ops/main/main.c
//  Find the partition map in the partition table
RTC_DATA_ATTR const char *modeFileManagerDir = "/";
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

  modeFileManagerFatReady = FFat.begin();
  
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


  if (modeFileManagerFatReady)
  {
    fs::FS &fs = FFat;
    File dir = fs.open(F(modeFileManagerDir));
    // Serial.println(dir.name());
    if (!dir)
    {
      draw_ic24_bad_file(170, 90, black);
      drawCentered(L("Помилка відкриття папки", "Error opening folder"), 150);
      items = 1;
    }
    else if (!dir.isDirectory())
    {
      drawCentered(L("Папка не папка", "Folder is not folder"), 100);
      drawCentered(modeFileManagerDir, 150);
      items = 1;
    }
    else
    {
      drawListItem(itemBack, draw_ic24_back, L("Назад", "Back"), L("В попереднє меню", "To previous menu"), firstDraw);
      int cnt = 1;
      while (true)
      {
        File file = dir.openNextFile();
        if (!file) // no more files
          break;

        //Serial.println(file.name());
        if (file.isDirectory())
        {
          drawListItem(cnt, draw_ic24_folder, file.name(), L("Папка", "Folder"), false);
        }
        else
        {
          if (strendswith(file.name(), ".txt"))
          {
            sprintf(buffer, L("Текст, %d байт", "Text, %d bytes"), file.size());
            drawListItem(cnt, draw_ic24_file, file.name(), buffer, false);
          }
          else if (strendswith(file.name(), ".dwm"))
          {
            sprintf(buffer, L("Мелодія, %d байт", "Melody, %d bytes"), file.size());
            drawListItem(cnt, draw_ic24_music, file.name(), buffer, false);
          }
          else if (strendswith(file.name(), ".wav"))
          {
            sprintf(buffer, L("Аудіо, %d байт", "Audio, %d bytes"), file.size());
            drawListItem(cnt, draw_ic24_music, file.name(), buffer, false);
          }
          else if (strendswith(file.name(), ".bmp"))
          {
            sprintf(buffer, L("Картинка, %d байт", "Image, %d bytes"), file.size());
            drawListItem(cnt, draw_ic24_image, file.name(), buffer, false);
          }
          else
          {
            sprintf(buffer, L("Файл, %d байт", "File, %d bytes"), file.size());
            drawListItem(cnt, draw_ic24_question, file.name(), buffer, false);
          }
        }
        file.close();
        cnt++;
      }
      items = cnt;
    }
    //because often filenames is too long, have to clear space after filenames bar
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
  FFat.end();
  modeExit = 0;
}

void playWavMelody(const char* path){
  fs::FS &fs = FFat;
  File file = FFat.open(F(path), FILE_READ, false);
  if(file!=0){
    unsigned long lastTick = micros();
    ledStatusOn();
    draw_ic16_empty(lx(), ly1(), black);
    draw_ic16_empty(lx(), ly3(), black);
    drawDim();
    draw_ic16_back(lx(), ly2(), black);
    drawMessage(L("Відтворення...", "Playing..."), file.name(), true);
    ledcSetup( /*channel*/0, /*freq*/24000, /*PWM_Res*/8);
    while(file.available())
    {
      char c = (char)file.read();
      ledcWrite( /*channel*/0, /*value*/c);
      while(micros()-lastTick < 120);
      lastTick = micros();
      if(isButtonCenterPressed()){
        buttonBeep();
        break;
      }
    }
    ledStatusOff();
    buzNoTone();
    file.close(); 
    clearScreenAnimation();
    while(isButtonCenterPressed());
  }
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
      selected = 0;
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
            const char* melodyPath = strdup(file.path());
            file.close();
            playDwmMelody(melodyPath);
            break;
          }
          if (!file.isDirectory() && strendswith(file.name(), ".wav"))
          {
            const char* melodyPath = strdup(file.path());
            file.close();
            playWavMelody(melodyPath);
            break;
          }
          if (!file.isDirectory() && strendswith(file.name(), ".bmp"))
          {
            modeFileReaderBmpPath = strdup(file.path());
            file.close();
            setmodeFileReaderBmp();
            break;
          }
        }
        file.close();
      }
    }
  }
}


void playDwmMelody(const char* path){
  fs::FS &fs = FFat;
  File file = FFat.open(F(path), FILE_READ, false);
  if(file!=0){
    melodyPlayerSetMelodyName(String(strdup(file.name())));
    const int melody_max = 2048;
    int* tmp_melody = new int[melody_max];// dynamically allocate memory
    int buffer_index = 0;
    int melody_index = 0;
    while(file.available())
    {
      char c = (char)file.read();
      if(isdigit(c) || c=='-'){
        buffer[buffer_index] = c;
        if(buffer_index < BUFFER_SIZE)
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
      if(melody_index >= melody_max)
        break;
    }
    file.close(); 
    if(melody_index > 0){
      tmp_melody[melody_index] = 19;
      tmp_melody[melody_max-1] = 19;

      melodyPlayerPlayMelody(/*const int* melody*/tmp_melody, /*bool alarm*/false); 
    }
    else{
      drawDim();
      drawMessageAnimated(L("Битий файл", "Corrupted file"));
    }
    delete tmp_melody; // deallocate the memory
    tmp_melody = nullptr; // set pointer to nullptr
  }
}

#endif