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
#include "ModeMemoryManager.h"
#include <Arduino.h>

Runnable modeFileReaderBmpBack = 0;
char* modeFileReaderBmpPath = (char *)"/";

void setmodeFileReaderBmp()
{
  if (modeExit != 0)
    modeExit();
  clearScreenAnimation();
  Serial.println(F("Set mode: BMP Reader"));
  modeFileReaderBmpBack = modeSetup;
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
  initFat();
}

void modeFileReaderBmpLoop()
{
  ledStatusOff();
  lcd()->setColorIndex(white);
  lcd()->drawBox(0, 0, 400, 240);
  lcd()->setColorIndex(black);

  FILE* f = fopen(modeFileReaderBmpPath, "r");   //https://cplusplus.com/reference/cstdio/
  if(f == NULL)
  {
    draw_ic24_bad_file(170, 90, black);
    drawCentered(L("Помилка відкриття файлу", "Error opening file"), 150);
    if (modeFileReaderBmpPath != 0)
      drawCentered(modeFileReaderBmpPath, 170);
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

    while(feof(f) == 0)
    {
      /*
      We're reading file linearly and when we're on offset of needed data, read it then continue reading linearly
      */
      if (offset == 0x00)
      {
        fread (id, 2/*size*/, 1/*cnt*/, /*FILE*/f );   //https://cplusplus.com/reference/cstdio/
        offset += 2/*bytes*/;
        if (id[0] != 'B' || id[1] != 'M')
        { // BM is valid
          draw_ic24_bad_file(170, 90, black);
          drawCentered(L("Невалідний формат файлу!", "Invalid file header!"), 150);
          drawCentered(L("Має бути BMP монохромний", "Only monochrome BMP supported"), 180);
          break;
        }
      }
      else if (offset == 0x02) //bmp_size offset
      {
        fread (&bmp_size, 4/*size*/, 1/*cnt*/, /*FILE*/f );  //https://cplusplus.com/reference/cstdio/
        offset += 4;
      }
      else if (offset == 0x0A) //imgdata_offset_OFFSET
      {
        fread (&imgdata_offset, 4/*size*/, 1/*cnt*/, /*FILE*/f );  //https://cplusplus.com/reference/cstdio/
        offset += 4;
      }
      else if (offset == 0x0E) //header_size_OFFSET
      {
        fread ((uint8_t *)&header_size, 1/*char=1byte*/, 4/*read 4 bytes*/, /*FILE*/f );  //https://cplusplus.com/reference/cstdio/
        offset += 4;
        if (header_size != 40)
        { // should be 40
          draw_ic24_bad_file(170, 90, black);
          drawCentered(L("Невалідний тип BMP!", "Wrong BMP type!"), 150);
          drawCentered(L("Спробуй створити іншою програмою", "Try save with other software"), 180);
          break;
        }
      }
      else if (offset == 0x12) //width
      {
        fread ((uint8_t *)&width, 1/*char=1byte*/, 4/*read 4 bytes*/, /*FILE*/f );  //https://cplusplus.com/reference/cstdio/
        offset += 4;
        offset_x = (W-width)/2;
        if (width == 0)
        { // should be not 0
          draw_ic24_bad_file(170, 90, black);
          drawCentered(L("Некоректна ширина зображення!", "Invalid width"), 150);
          drawCentered(L("Чомусь прописано 0 пікселів", "Width set as 0 px"), 180);
          break;
        }
      }
      else if (offset == 0x16) //height
      {
        fread ((uint8_t *)&height, 1/*char=1byte*/, 4/*read 4 bytes*/, /*FILE*/f );  //https://cplusplus.com/reference/cstdio/
        offset += 4;
        bmp_y = height-1;
        offset_y = (H-height)/2 ;
        if (height == 0)
        { // should be not 0
          draw_ic24_bad_file(170, 90, black);
          drawCentered(L("Некоректна висота зображення!", "Invalid height"), 150);
          drawCentered(L("Чомусь прописано 0 пікселів", "Height set as 0 px"), 180);
          break;
        }
      }
      else if (offset == 0x1C) //bitsPerPixel
      {
        fread ((uint8_t *)&bitsPerPixel, 1/*char=1byte*/, 2/*read 2 bytes*/, /*FILE*/f );  //https://cplusplus.com/reference/cstdio/
        offset += 2;
        if (bitsPerPixel != 1)
        { // should be 1
          draw_ic24_bad_file(170, 90, black);
          drawCentered(L("Некоректний кольоровий формат!", "Wrong color format"), 150);
          drawCentered(L("Має бути монохромне зображення", "Has to be monochrome image"), 180);
          break;
        }
      }
      else if (offset == 0x1E) //compression method
      {
        fread ((uint8_t *)&compressionMethod, 1/*char=1byte*/, 4/*read 4 bytes*/, /*FILE*/f );  //https://cplusplus.com/reference/cstdio/
        offset += 4;
        if (compressionMethod != 0)
        { // should be 0
          draw_ic24_bad_file(170, 90, black);
          drawCentered(L("Файл стиснений!", "File compressed"), 150);
          drawCentered(L("Має бути без компресії", "Only non compressed supported"), 180);
          break;
        }
      }
      else if (offset >= imgdata_offset) //actual image
      {
        uint32_t img_buffer = 0;
        fread ((uint8_t *)&img_buffer, 4/*size*/, 1/*cnt*/, /*FILE*/f );  //https://cplusplus.com/reference/cstdio/
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
        fgetc(f);
        offset++;
      }
    }

    if(false) //DEBUG
    {  
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
    fclose(f);
  }
  lcd()->sendBuffer();
}

void modeFileReaderBmpExit()
{
  free(modeFileReaderBmpPath);
  modeExit = 0;
  exitFat();
}

void modeFileReaderBmpButtonUp()
{

}
void modeFileReaderBmpButtonCenter()
{
  if(modeFileReaderBmpBack != 0)
  {
    modeFileReaderBmpBack();
    modeFileReaderBmpBack = 0;
  }
  else{
    setmodeFileManager(); // exit
  }
}
void modeFileReaderBmpButtonDown()
{

}

#endif