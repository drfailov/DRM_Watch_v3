#ifndef MODECALENDAR_H
#define MODECALENDAR_H

void setModeCalendar();
void ModeCalendarLoop();
void ModeCalendarButtonUp();
void ModeCalendarButtonCenter();
void ModeCalendarButtonDown();

#include "Lcd.h"
#include "Global.h"
#include "AutoSleep.h"
#include "Button.h"
#include "RTC.h"
#include "ModeWatchface.h"
#include "ModeMenuApps.h"
#include "Icons.h"
#include "RTC.h"

#include <time.h>
int year = 0;
int monthToDraw = 0;/*01-12*/
const char* days(int d) {
    if(d == -1) return L("Нд", "Sun"); 
    if(d == 0) return L("Пн", "Mon"); 
    if(d == 1) return L("Вт", "Tue");
    if(d == 2) return L("Ср", "Wed");
    if(d == 3) return L("Чт", "Thu");
    if(d == 4) return L("Пт", "Fri");
    if(d == 5) return L("Сб", "Sat");
    if(d == 6) return L("Нд", "Sun"); 
    return "---";
};
const char* months(int m){ 
  if(m==0) return L("Січень", "January");
  if(m==1) return L("Лютий", "February");
  if(m==2) return L("Березень", "March");
  if(m==3) return L("Квітень", "April");
  if(m==4) return L("Травень", "May");
  if(m==5) return L("Червень", "June");
  if(m==6) return L("Липень", "July");
  if(m==7) return L("Серпень", "August");
  if(m==8) return L("Вересень", "September");
  if(m==9) return L("Жовтень", "October");
  if(m==10) return L("Листопад", "November");
  if(m==11) return L("Грудень", "December");
  return "---";
}
  tm _tm;



void drawCalendar(int xoffset/*px*/, int ylegend/*px*/, int month /*01-12*/, int year /*ex:2024*/, bool compact){
  int monthCurrent = rtcGetMonth()+1;/*00-11 -> 01-12*/
  int dayCurrent = rtcGetDay();
  int legendToCalendarOffset = (compact?8:12);
  int yoffset = ylegend + legendToCalendarOffset; //px
  int dayw = compact?25:38;//px
  int dayh = compact?17:23;//px  
  int dayFrameMargin = compact?3:5;
  int dayFrameTextHeight = compact?10:13;
  int frameMargin = 20;
  int frameDoWs = 6;
  int frameWeeks = 6;
  
  //background
  lcd()->setColorIndex(white);
  lcd()->drawRBox(/*x*/xoffset-frameMargin, /*y*/ylegend-frameMargin, /*w*/frameDoWs*dayw+frameMargin*2, /*h*/frameWeeks*dayh+frameMargin*2, getRoundness()); //white background
  lcd()->setColorIndex(black);
  lcd()->drawRFrame(/*x*/xoffset-frameMargin, /*y*/ylegend-frameMargin, /*w*/frameDoWs*dayw+frameMargin*2, /*h*/frameWeeks*dayh+frameMargin*2, getRoundness()); //black frame
  lcd()->drawLine(/*x*/xoffset-frameMargin, yoffset-2,/*x*/xoffset+frameMargin+frameDoWs*dayw-1, ylegend+legendToCalendarOffset-2);  //top legend separator line
  drawDashedLine(/*x1*/ xoffset+(dayw*4.5), /*y1*/ yoffset-2, /*x2*/ xoffset+(dayw*4.5), /*y2*/ ylegend-2+frameWeeks*dayh+frameMargin, 1); //weekend separator line

  //legend
  lcd()->setColorIndex(black);
  if(compact) lcd()->setFont(u8g2_font_unifont_t_cyrillic);  //ok
  else lcd()->setFont(u8g2_font_10x20_t_cyrillic);  //ok
  for(int DoW=0; DoW<7; DoW++){
    int x = xoffset + (DoW)*dayw; //center point of text
    int dayTextWidth = lcd()->getUTF8Width(days(DoW));
    lcd()->drawUTF8(x-dayTextWidth/2, ylegend, days(DoW));
    if(DoW > 4){ //highlight weekeng as bold text
      lcd()->setFontMode(/*is_transparent*/ true);
      lcd()->drawUTF8(1+x-dayTextWidth/2, ylegend, days(DoW));
      lcd()->setFontMode(/*is_transparent*/ false);
    }
  }

  //calendar
  int week = 1;
  for(int drawday=1; drawday<=31; drawday++){
    _tm.tm_hour = 12;
    _tm.tm_min = 0;
    _tm.tm_sec = 0;
    _tm.tm_mday = drawday;
    _tm.tm_mon = month-1;
    _tm.tm_year = year - 1900;
    time_t timeSinceEpoch = mktime(&_tm);
    if(_tm.tm_mon == month-1){
      int DoW = _tm.tm_wday;//Days since Sunday [0-6].
      if(DoW == 0) DoW = 7;
      int x = xoffset + (DoW-1)*dayw; //center point of text
      int y = yoffset + week*dayh; //bottom point of text
      sprintf(buffer, "%d", drawday);
      int dayTextWidth = lcd()->getUTF8Width(buffer);

      if(month == monthCurrent && drawday == dayCurrent){ //highlight current day
        lcd()->setColorIndex(black);
        lcd()->drawRBox(/*x*/x-dayFrameMargin-dayTextWidth/2, /*y*/y-dayFrameTextHeight-dayFrameMargin, /*w*/dayTextWidth + dayFrameMargin*2 + (DoW>5), /*h*/dayFrameTextHeight + dayFrameMargin*2, getRoundness(6));
        lcd()->setColorIndex(white);
        lcd()->drawUTF8(x-dayTextWidth/2, y, buffer);
        if(DoW > 5){ //highlight weekeng as bold text
          lcd()->setFontMode(/*is_transparent*/ true);
          lcd()->drawUTF8(1+x-dayTextWidth/2, y, buffer);
          lcd()->setFontMode(/*is_transparent*/ false);
        }
      }
      else{
        lcd()->setColorIndex(black);
        lcd()->drawUTF8(x-dayTextWidth/2, y, buffer);
        if(DoW > 5){ //highlight weekeng as bold text
          lcd()->setFontMode(/*is_transparent*/ true);
          lcd()->drawUTF8(1+x-dayTextWidth/2, y, buffer);
          lcd()->setFontMode(/*is_transparent*/ false);
        }
      }
      if(DoW == 7) {
        week++;
        if(firstDraw) lcd()->sendBuffer();
      }
    }
  }
}


void setModeCalendar(){
  clearScreenAnimation();
  Serial.println(F("Set mode: Calendar"));
  modeSetup = setModeCalendar;
  modeLoop = ModeCalendarLoop;
  modeButtonUp = ModeCalendarButtonUp;
  modeButtonCenter = ModeCalendarButtonCenter;
  modeButtonDown = ModeCalendarButtonDown;
  modeButtonUpLong = 0;
  modeButtonCenterLong = 0;
  modeButtonDownLong = 0;
  registerAction();
  enableAutoReturn = true;
  enableAutoSleep = false; 
  autoReturnTime = autoReturnDefaultTime;
  autoSleepTime = autoSleepDefaultTime;
  year = rtcGetYear();
  monthToDraw = rtcGetMonth()+1;/*00-11 -> 01-12*/
}

void ModeCalendarLoop(){ 
  lcd()->setColorIndex(white);
  lcd()->drawBox(0, 0, 400, 240);
  drawStatusbar(363, 1, true);
  lcd()->setColorIndex(black);
  lcd()->setFont(u8g2_font_10x20_t_cyrillic);  //ok
  lcd()->setCursor(5, 18); 
  lcd()->print(L("Календар", "Calendar"));

  lcd()->setColorIndex(black);
  sprintf(buffer, "(%02d) %s %04d", monthToDraw, months(monthToDraw-1), year);
  drawCentered(buffer, 45);
  
  lcd()->setColorIndex(black);
  lcd()->drawBox(369, 0, 2, 260);
  draw_ic16_arrow_up(lx(), ly1(), black);
  draw_ic16_back(lx(), ly2(), black);
  draw_ic16_arrow_down(lx(), ly3(), black);

  drawCalendar(/*x*/67, /*y*/77, /*month 01-12*/monthToDraw, year, /*compact*/false);

  lcd()->sendBuffer();
}

void ModeCalendarButtonUp(){
  monthToDraw--;
  if(monthToDraw < 1){
    year--;
    monthToDraw = 12;
  }
}

void ModeCalendarButtonCenter(){
  setModeAppsMenu();
}

void ModeCalendarButtonDown(){
  monthToDraw++;
  if(monthToDraw > 12){
    year++;
    monthToDraw = 1;
  }
}



//----------------------//----------------------//----------------------//----------------------//----------------------//----------------------//----------------------



#endif