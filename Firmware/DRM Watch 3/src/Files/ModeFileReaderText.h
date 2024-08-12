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
    Serial.println(f.name());
    if (!f)
    {
      draw_ic24_bad_file(170, 90, black);
      drawCentered("Помилка відкриття файлу", 150);
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
    draw_ic24_bad_file(170, 90, black);
    drawCentered("Файлова система пошкоджена", 150);
    if(modeFileReaderTextPath != 0)
      drawCentered(modeFileReaderTextPath, 170);
  }
  drawMenuLegend();

  lcd()->sendBuffer();
}

void modeFileReaderTextExit(){
  free(modeFileReaderTextPath);
  FFat.end();
  modeExit = 0;
}

void modeFileReaderTextButtonUp()
{
  if(modeFileReaderText_page > 0)
    modeFileReaderText_page --;
}
void modeFileReaderTextButtonCenter()
{
  FFat.end();
  setmodeFileManager(); // exit
}
void modeFileReaderTextButtonDown()
{
  if(modeFileReaderText_pages[modeFileReaderText_page+1] != modeFileReaderText_pages[modeFileReaderText_page] && modeFileReaderText_page < modeFileReaderText_pageMax-1)
    modeFileReaderText_page ++;
}

#endif