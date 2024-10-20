#ifndef SCREENBUFFER_H
#define SCREENBUFFER_H

#include <Arduino.h>
#include "Global.h"
#include "Lcd.h"

RTC_DATA_ATTR byte screenBuffer[BUFF_W*BUFF_H/8]; 

void randScreenBuffer(){
  //randomSeed(readSensUsbRaw());
  for(int i=0; i<BUFF_W*BUFF_H/8; i++)
    screenBuffer[i] = (micros()+rand())%255;//rand()%255;
}
void zeroScreenBuffer(){
  for(int i=0; i<BUFF_W*BUFF_H/8; i++)
    screenBuffer[i] = 0;
}
bool getScreenBuffer(int x, int y){
  if(x<0 || y<0 || x>=BUFF_W || y>=BUFF_H) return false;
  int addr = BUFF_W*y+x;
  int cellIndex = addr/8;
  int bitIndex = addr%8;
  byte cell = screenBuffer[cellIndex];
  bool value = bitRead(cell, bitIndex);
  return value;
}
void setScreenBuffer(int x, int y, bool val){
  if(x<0 || y<0 || x>=BUFF_W || y>=BUFF_H) return;
  int addr = BUFF_W*y+x;
  int cellIndex = addr/8;
  int bitIndex = addr%8;
  byte cell = screenBuffer[cellIndex];
  bitWrite(cell, bitIndex, val);
  screenBuffer[cellIndex] = cell;
}
void drawScreenBuffer(int xstart, int ystart, int xend, int yend){ //IN BUFFER COORDS
  if(xstart < 0) xstart=0;
  if(ystart < 0) ystart=0;
  if(xend >= BUFF_W) xend=BUFF_W-1;
  if(yend >= BUFF_H) yend=BUFF_H-1;

  lcd()->setColorIndex(white);
  lcd()->drawBox(xstart*BUFF_SCALE, ystart*BUFF_SCALE, (xend-xstart)*BUFF_SCALE, (yend-xstart)*BUFF_SCALE);
  lcd()->setColorIndex(black);
  for(int y=ystart; y<=yend; y++){
    for(int x=xstart; x<=xend; x++){
      bool value = getScreenBuffer(x,y);
      if(value){
        lcd()->drawBox(x*BUFF_SCALE, y*BUFF_SCALE, BUFF_SCALE, BUFF_SCALE);
      }
    }
  }
}
void drawScreenBuffer(){
  drawScreenBuffer(0,0,BUFF_W,BUFF_H);
}


#endif