#ifndef WATCHFACEFULLSCREEN_H
#define WATCHFACEFULLSCREEN_H

#include "DrmPreferences.h"
#include <Arduino.h>
#include "ScreenBuffer.h"
#include "ModeLife.h"
#include "ModeCalendar.h"
#include "AnalogClock.h"


void drawWatchfaceFullscreen(bool firstDraw){

  if(getWatchfaceLifeBackgroundEnabled()){
    if(firstDraw && millis()>2000)
      randScreenBuffer();
    lifeStep();
    drawScreenBuffer();
  }
  else if(getWatchfaceFireBackgroundEnabled()){
    fireStep();
    drawScreenBuffer();
  }
  else if(getWatchfaceLavaBackgroundEnabled()){
    lavaStep();
    drawScreenBuffer();
  }
  else if(getWatchfaceRandomBackgroundEnabled()){
    randScreenBuffer();
    drawScreenBuffer();
  }
  else if(getWatchfaceAntBackgroundEnabled()){    //ANT
    if(firstDraw && millis()>2000)
      antSpawn();
    int speed = getAntSpeed();
    if(speed < 1) speed = 1;
    if(speed > 9999) speed = 9999;
    for(int i=0; i<speed; i++)
      antStep();
    drawScreenBuffer();
  }
  else{
    lcd()->setColorIndex(white);
    lcd()->drawBox(0, 0, 400, 240);
  }
  //lcd()->clearBuffer();   //fills actually black

  if(getWatchfaceDigitalEnabled()){
    lcd()->setFont(u8g2_font_logisoso92_tn);
    int h = rtcGetHour();
    int m = rtcGetMinute();
    sprintf(buffer, "%02d:%02d", h,m);
    int width = lcd()->getStrWidth(buffer);
    int tx = (400-width)/2;
    int ty = 160;
    int margin = 5;
    lcd()->setColorIndex(white);
    lcd()->drawBox(tx-margin, ty-92-margin, width+margin*2, 92+margin*2);
    lcd()->setColorIndex(black);
    lcd()->setCursor(tx, ty);
    lcd()->print(buffer); 
  }
  else if(getWatchfaceAnalogEnabled()){
    float h = rtcGetHour();
    float m = rtcGetMinute();
    float clockRadius = ((float)H)*0.35;
    int centerX = W/2;
    int centerY = H/2-18;
    if(firstDraw)
      animateClock(/*centerX*/centerX, /*centerY*/centerY, /*clockRadius*/clockRadius, /*hour*/h, /*minute*/m);
    drawClock(/*centerX*/centerX, /*centerY*/centerY, /*clockRadius*/clockRadius, /*hour*/h, /*minute*/m);
  }
  else if(getWatchfaceCalendarEnabled()){
    drawCalendar(/*x*/125, /*y*/68, /*month 01-12*/rtcGetMonth()+1, rtcGetYear(), /*compact*/true);
  }

  if(getWatchfaceDateEnabled())
    drawDate(5, 20);
  
  if(getWatchfaceDjiLogoEnabled())
    draw_ic28_dji(178, 209, black);

  if(getWatchfaceDayOfWeekEnabled())
    drawDayOfWeek(375, 20);

  if(getWatchfaceTemperatureEnabled())
    drawTemperature(5, 213);

  if(getWatchfaceStatusbarEnabled()){
    drawStatusbar(395, 214, getWatchfaceStatusbarDigitalEnabled());
  }
}


#endif