#ifndef WATCHFACESIDE_H
#define WATCHFACESIDE_H

#include "DrmPreferences.h"
#include <Arduino.h>
#include "ScreenBuffer.h"
#include "ModeLife.h"
#include "ModeCalendar.h"
#include "AnalogClock.h"

void drawWatchfaceSide(bool firstDraw){
  int x_del = 240;

  if(getWatchfaceLifeBackgroundEnabled()){       //LIFE
    if(firstDraw && millis()>2000)
      randScreenBuffer();
    lifeStep();
    drawScreenBuffer(0,0,x_del/BUFF_SCALE,BUFF_H);
  }
  else if(getWatchfaceFireBackgroundEnabled()){    //FIRE
    fireStep(x_del/BUFF_SCALE + 1);
    drawScreenBuffer(0,0,x_del/BUFF_SCALE,BUFF_H);
  }
  else if(getWatchfaceLavaBackgroundEnabled()){    //LAVA
    lavaStep(x_del/BUFF_SCALE + 1);
    drawScreenBuffer(0,0,x_del/BUFF_SCALE,BUFF_H);
  }
  else if(getWatchfaceRandomBackgroundEnabled()){    //RANDOM
    randScreenBuffer();
    drawScreenBuffer(0,0,x_del/BUFF_SCALE,BUFF_H);
  }
  else if(getWatchfaceDotsBackgroundEnabled()){  //DOTS
    if(firstDraw && millis()>2000)
      zeroScreenBuffer();
    dotsStep();
    drawScreenBuffer(0,0,x_del/BUFF_SCALE,BUFF_H);
  }
  else if(getWatchfaceAntBackgroundEnabled()){    //ANT
    if(firstDraw && millis()>2000)
      antSpawn();
    int speed = getAntSpeed();
    if(speed < 1) speed = 1;
    if(speed > 9999) speed = 9999;
    for(int i=0; i<speed; i++)
      antStep();
    drawScreenBuffer(0,0,x_del/BUFF_SCALE,BUFF_H);
  }
  else if(getWatchfaceFileBackgroundEnable()){
    strcpy(modeFileReaderBmpPath, getWatchfaceFileBackgroundPath().c_str());
    drawBmp(modeFileReaderBmpPath);
  }
  else{
    lcd()->setColorIndex(white);
    lcd()->drawBox(0, 0, 400, 240);
  }

  lcd()->setColorIndex(white);
  lcd()->drawBox(x_del, 0, W-x_del, 240);
  if(firstDraw) lcd()->sendBuffer();
  
  if(getWatchfaceTemperatureEnabled())
    drawTemperature(x_del+4, 3);

  if(getWatchfaceTemperatureGraphEnabled())
    drawPlot<float>(/*x*/ 0, /*y*/ -14, /*w*/ x_del+2, /*h*/ 76, /*thickness*/ 1, /*legend*/ false, /*rangeValues*/ temperatureLog, /*values*/ temperatureLog, /*length*/ temperatureLogLength, /*highlight*/ -1);

  if(getWatchfaceDayOfWeekEnabled())
    drawDayOfWeek(395, 20);
  if(firstDraw) lcd()->sendBuffer();
  
  if(getWatchfaceDateEnabled())
    drawDate(x_del+30, 57);

  lcd()->setColorIndex(black);
  lcd()->drawBox(x_del, 0, 4, 240);

  if(firstDraw) lcd()->sendBuffer();


  int h = rtcGetHour();
  int m = rtcGetMinute();
  int s = rtcGetSecond();

  if(getWatchfaceDigitalEnabled()){
    lcd()->setColorIndex(black);
    lcd()->setFont(u8g2_font_logisoso38_tn);//u8g2_font_inr24_t_cyrillic
    sprintf(buffer, "%02d:%02d", h,m);
    int width = lcd()->getStrWidth(buffer);
    lcd()->setCursor(x_del+((W-x_del)-width)/2, 140);
    lcd()->print(buffer); 

    if(isAwake()){
      lcd()->setFont(u8g2_font_10x20_t_cyrillic);  //ok
      lcd()->print(s); 
    }
    
  }

  if(getWatchfaceDjiLogoEnabled())
    draw_ic28_dji(x_del+((W-x_del)-48)/2, 175, black);
  
  if(getWatchfaceStatusbarEnabled())
    drawStatusbar(395, 214, getWatchfaceStatusbarDigitalEnabled());

  if(getWatchfaceAnalogEnabled()){
    float centerX = 120;
    float centerY = 120;
    float clockRadius = 90;
    if(firstDraw)
      animateClock(/*centerX*/centerX, /*centerY*/centerY, /*clockRadius*/clockRadius, /*hour*/h, /*minute*/m);
    drawClock(/*centerX*/centerX, /*centerY*/centerY, /*clockRadius*/clockRadius, /*hour*/h, /*minute*/m);
  }
  else if(getWatchfaceCalendarEnabled()){
    drawCalendar(/*x*/45, /*y*/69, /*month 01-12*/rtcGetMonth()+1, rtcGetYear(), /*compact*/true);
  }
  
  
  /*getWatchfaceCalendarEnabled*/
}


#endif
