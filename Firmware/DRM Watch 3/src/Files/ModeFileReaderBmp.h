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
RTC_DATA_ATTR char *modeFileReaderBmpPath = (char *)"/";
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

  //lcd()->setFont(u8g2_font_10x20_t_cyrillic); // ok
  lcd()->setColorIndex(black);
  //lcd()->setCursor(5, 18);
  // lcd()->print(getFileNameFromPath(modeFileReaderBmpPath));
  // if (modeFileReaderBmpPath != 0)
  //  lcd()->print((modeFileReaderBmpPath));

  //drawStatusbar(363, 1, true);

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

      char id[2];
      uint32_t bmp_size = 0;
      uint32_t imgdata_offset = 0xFFFFFFFF;
      uint32_t header_size = 0;
      int32_t width = 0;
      int32_t height = 0;
      uint16_t bitsPerPixel = 0;
      uint32_t compressionMethod = 0;

      int32_t offset_x = 0;
      int32_t offset_y = 0;
      int32_t bmp_x = 0;
      int32_t bmp_y = 0;


      while (f.available())
      {
        /*
        We're reading file linearly and when we're on offset of needed data, read it then continue reading linearly
        */
        if (offset == 0x00)
        {
          f.read((uint8_t *)id, 2/*bytes*/);
          offset += 2/*bytes*/;
          if (id[0] != 'B' || id[1] != 'M')
          { // BM is valid
            draw_ic24_bad_file(170, 90, black);
            drawCentered("Невалідний формат файлу!", 150);
            drawCentered("Має бути BMP монохромний", 180);
            break;
          }
        }
        else if (offset == 0x02) //bmp_size offset
        {
          f.read((uint8_t *)&bmp_size, 4 /*bytes*/);
          offset += 4;
        }
        else if (offset == 0x0A) //imgdata_offset_OFFSET
        {
          f.read((uint8_t *)&imgdata_offset, 4 /*bytes*/);
          offset += 4;
        }
        else if (offset == 0x0E) //header_size_OFFSET
        {
          f.read((uint8_t *)&header_size, 4  /*bytes*/);
          offset += 4;
          if (header_size != 40)
          { // should be 40
            draw_ic24_bad_file(170, 90, black);
            drawCentered("Невалідний тип заголовку!", 150);
            drawCentered("Спробуй створити іншою програмою", 180);
            break;
          }
        }
        else if (offset == 0x12) //width
        {
          f.read((uint8_t *)&width,   4/*bytes*/);
          offset += 4;
          offset_x = (W-width)/2;
          if (width == 0)
          { // should be not 0
            draw_ic24_bad_file(170, 90, black);
            drawCentered("Некоректна ширина зображення!", 150);
            drawCentered("Чомусь прописано 0 пікселів", 180);
            break;
          }
        }
        else if (offset == 0x16) //height
        {
          f.read((uint8_t *)&height, 4/*bytes*/);
          offset += 4;
          bmp_y = height-1;
          offset_y = (H-height)/2 ;
          if (height == 0)
          { // should be not 0
            draw_ic24_bad_file(170, 90, black);
            drawCentered("Некоректна висота зображення!", 150);
            drawCentered("Чомусь прописано 0 пікселів", 180);
            break;
          }
        }
        else if (offset == 0x1C) //bitsPerPixel
        {
          f.read((uint8_t *)&bitsPerPixel, 2/*bytes*/);
          offset += 2;
          if (bitsPerPixel != 1)
          { // should be 1
            draw_ic24_bad_file(170, 90, black);
            drawCentered("Некоректний кольоровий формат!", 150);
            drawCentered("Має бути монохромне зображення", 180);
            break;
          }
        }
        else if (offset == 0x1E) //compression method
        {
          f.read((uint8_t *)&compressionMethod, 4/*bytes*/);
          offset += 4;
          if (compressionMethod != 0)
          { // should be 0
            draw_ic24_bad_file(170, 90, black);
            drawCentered("Файл стиснений!", 150);
            drawCentered("Має бути без компресії", 180);
            break;
          }
        }
        else if (offset >= imgdata_offset) //actual image
        {
          uint32_t img_buffer = 0;
          f.read((uint8_t *)&img_buffer, 4/*bytes*/);
          offset += 4;
          for (int j = 0;  j < 4;  j++)
          {
            for(int i=0; i<8; i++){
              bool pixel =  0 != (img_buffer & (1 << (8*j+(7-i))));
              if(!pixel){
                int scr_x = offset_x+bmp_x;
                int scr_y = offset_y+bmp_y;
                if(scr_x < W && scr_y < H && bmp_y > 0 && bmp_x < width)
                  lcd()->drawPixel(offset_x+bmp_x, offset_y+bmp_y);
              }
                
              bmp_x ++;
            }
          }
          if(bmp_x > width){
            bmp_x = 0;
            bmp_y --;
          }   
        }
        else
        {
          f.read();
          offset++;
        }
      }

      if(false){  //DEBUG
        lcd()->setFont(u8g2_font_unifont_t_cyrillic); // smalll
        lcd()->setColorIndex(black);
        
        lcd()->setCursor(5, 40);
        lcd()->print("id=");
        print(id, 2);

        lcd()->setCursor(5, 55);
        lcd()->print("size=");
        lcd()->print(bmp_size);

        lcd()->setCursor(5, 70);
        lcd()->print("imgdata_offset=");
        lcd()->print(imgdata_offset);

        lcd()->setCursor(5, 85);
        lcd()->print("header_size=");
        lcd()->print(header_size);

        lcd()->setCursor(5, 100);
        lcd()->print("width=");
        lcd()->print(width);

        lcd()->setCursor(5, 115);
        lcd()->print("height=");
        lcd()->print(height);

        lcd()->setCursor(5, 130);
        lcd()->print("bitsPerPixel=");
        lcd()->print(bitsPerPixel);

        lcd()->setCursor(5, 145);
        lcd()->print("compressionMethod=");
        lcd()->print(compressionMethod);
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
  //lcd()->setColorIndex(black);
  //lcd()->drawBox(369, 0, 2, 260); // draw_ic16_repeat  draw_ic16_arrow_right  draw_ic16_back
  //draw_ic16_back(lx(), ly2(), black);

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