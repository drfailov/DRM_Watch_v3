/*
Big thanks to this author:
https://github.com/Munsutari/esp32-s3-internal-flash-msc
*/
#ifndef modeFileReaderBmp_H
#define modeFileReaderBmp_H

/*PROTOTYPES*/
void modeFileReaderBmpLoop();
void modeFileReaderBmpExit();
void setmodeFileReaderBmp();
void modeFileReaderBmpButtonUp();
void modeFileReaderBmpButtonCenter();
void modeFileReaderBmpButtonDown();

#include "Global.h"
#include "AutoSleep.h"
#include "Button.h"
#include "ModeSerialLog.h"
#include <Arduino.h>
#include "FFat.h"
#include "FS.h"

// https://github.com/espressif/esp-idf/blob/master/examples/storage/partition_api/partition_ops/main/main.c
//  Find the partition map in the partition table
char *modeFileReaderBmpPath = (char *)"/";
bool modeFileReaderBmp_fatReady = false;

void setmodeFileReaderBmp()
{
  if (modeExit != 0)
    modeExit();
  clearScreenAnimation();
  Serial.println(F("Set mode: BMP Reader"));
  modeSetup = setmodeFileReaderBmp;
  modeLoop = modeFileReaderBmpLoop;
  modeExit = modeFileReaderBmpExit;
  modeButtonUp = modeFileReaderBmpButtonUp; // modeFileReaderBmpButtonUp;
  modeButtonCenter = modeFileReaderBmpButtonCenter;
  modeButtonDown = modeFileReaderBmpButtonDown; // modeFileReaderBmpButtonDown;
  modeButtonUpLong = 0;
  modeButtonCenterLong = 0;
  modeButtonDownLong = 0;
  registerAction();
  enableAutoReturn = true;
  enableAutoSleep = false;
  autoReturnTime = autoReturnDefaultTime;
  autoSleepTime = autoSleepDefaultTime;

  modeFileReaderBmp_fatReady = FFat.begin();
}

void modeFileReaderBmpLoop()
{
  ledStatusOff();
  lcd()->setColorIndex(white);
  lcd()->drawBox(0, 0, 400, 240);

  lcd()->setFont(u8g2_font_10x20_t_cyrillic); // ok
  lcd()->setColorIndex(black);
  lcd()->setCursor(5, 18);
  // lcd()->print(getFileNameFromPath(modeFileReaderBmpPath));
  if (modeFileReaderBmpPath != 0)
    lcd()->print((modeFileReaderBmpPath));

  drawStatusbar(363, 1, true);

  if (modeFileReaderBmp_fatReady)
  {
    fs::FS &fs = FFat;
    File f = FFat.open(F(modeFileReaderBmpPath), FILE_READ, false);
    Serial.println(f.name());
    if (!f)
    {
      draw_ic24_bad_file(170, 90, black);
      drawCentered("Помилка відкриття файлу", 150);
    }
    else
    {

      uint32_t offset = 0;
      
      //https://en.wikipedia.org/wiki/BMP_file_format#Bitmap_file_header

      const int ID_OFFSET = 0x00;
      const int ID_SIZE = 2; // bytes
      char id[ID_SIZE];

      const int BMP_SIZE_OFFSET = 0x02;
      const int BMP_SIZE_SIZE = 4; // bytes
      uint32_t bmp_size = 0;

      const int IMGDATA_OFFSET_OFFSET = 0x0A;
      const int IMGDATA_OFFSET_SIZE = 4; // bytes
      uint32_t imgdata_offset = 0xFFFFFFFF;

      while (f.available())
      {
        /*
        We're reading file linearly and when we're on offset of needed data, read it then continue reading linearly
        */
        if (offset == ID_OFFSET)
        {
          f.read((uint8_t *)id, ID_SIZE);
          offset += ID_SIZE;

          if (id[0] != 'B' || id[1] != 'M')
          { // BM is valid
            draw_ic24_bad_file(170, 90, black);
            drawCentered("Невалідний формат!", 150);
            break;
          }
        }
        else if (offset == BMP_SIZE_OFFSET)
        {
          f.read((uint8_t *)&bmp_size, BMP_SIZE_SIZE);
          offset += BMP_SIZE_SIZE;
        }
        else if (offset == IMGDATA_OFFSET_OFFSET)
        {
          f.read((uint8_t *)&imgdata_offset, IMGDATA_OFFSET_SIZE);
          offset += IMGDATA_OFFSET_SIZE;
        }
        else
        {
          f.read();
          offset++;
        }
      }

      {  //DEBUG
        lcd()->setFont(u8g2_font_unifont_t_cyrillic); // smalll
        lcd()->setColorIndex(black);
        
        lcd()->setCursor(5, 40);
        lcd()->print("id=");
        print(id, ID_SIZE);

        lcd()->setCursor(5, 55);
        lcd()->print("size=");
        lcd()->print(bmp_size);

        lcd()->setCursor(5, 70);
        lcd()->print("imgdata_offset=");
        lcd()->print(imgdata_offset);
      }

      f.close();
    }
  }
  else
  {
    draw_ic24_bad_file(170, 90, black);
    drawCentered("Файлова система пошкоджена", 150);
    if (modeFileReaderBmpPath != 0)
      drawCentered(modeFileReaderBmpPath, 170);
  }
  drawMenuLegend();

  lcd()->sendBuffer();
}

void modeFileReaderBmpExit()
{
  free(modeFileReaderBmpPath);
  FFat.end();
  modeExit = 0;
}

void modeFileReaderBmpButtonUp()
{
}
void modeFileReaderBmpButtonCenter()
{
  FFat.end();
  setmodeFileManager(); // exit
}
void modeFileReaderBmpButtonDown()
{
}

#endif