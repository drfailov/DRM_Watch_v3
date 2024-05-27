#ifndef MODECALENDAR_H
#define MODECALENDAR_H

#include <time.h>
int year = 0;
int monthToDraw = 0;/*01-12*/
String days[7] = { "Пн", "Вт", "Ср", "Чт", "Пт", "Сб", "Нд" };
String months[12] = { "Січень", "Лютий", "Березень", "Квітень", "Травень", "Червень", "Липень", "Серпень", "Вересень", "Жовтень", "Листопад", "Грудень"};
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
  lcd()->drawRBox(/*x*/xoffset-frameMargin, /*y*/ylegend-frameMargin, /*w*/frameDoWs*dayw+frameMargin*2, /*h*/frameWeeks*dayh+frameMargin*2, roundness);
  lcd()->setColorIndex(black);
  lcd()->drawRFrame(/*x*/xoffset-frameMargin, /*y*/ylegend-frameMargin, /*w*/frameDoWs*dayw+frameMargin*2, /*h*/frameWeeks*dayh+frameMargin*2, roundness);
  lcd()->drawLine(/*x*/xoffset-frameMargin, ylegend+legendToCalendarOffset-2,/*x*/xoffset+frameMargin+frameDoWs*dayw-1, ylegend+legendToCalendarOffset-2);

  //legend
  lcd()->setColorIndex(black);
  if(compact) lcd()->setFont(u8g2_font_unifont_t_cyrillic);  //ok
  else lcd()->setFont(u8g2_font_10x20_t_cyrillic);  //ok
  for(int DoW=0; DoW<7; DoW++){
    int x = xoffset + (DoW)*dayw; //center point of text
    int dayTextWidth = lcd()->getUTF8Width(days[DoW].c_str());
    lcd()->setCursor(x-dayTextWidth/2, ylegend); 
    lcd()->print(days[DoW]);
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

      if(month == monthCurrent && drawday == dayCurrent){
        lcd()->setColorIndex(black);
        lcd()->drawRBox(/*x*/x-dayFrameMargin-dayTextWidth/2, /*y*/y-dayFrameTextHeight-dayFrameMargin, /*w*/dayTextWidth + dayFrameMargin*2, /*h*/dayFrameTextHeight + dayFrameMargin*2, roundness);
        lcd()->setColorIndex(white);
        lcd()->setCursor(x-dayTextWidth/2, y); 
        lcd()->print(drawday);
      }
      else{
        lcd()->setColorIndex(black);
        lcd()->setCursor(x-dayTextWidth/2, y); 
        lcd()->print(drawday);
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
  lcd()->print("Календар");


  int xheader = 100; //px
  int yheader = 45; //px
  lcd()->setColorIndex(black);
  lcd()->setCursor(xheader, yheader); 
  lcd()->print("("); lcd()->print(monthToDraw); lcd()->print(") "); 
  lcd()->print(months[monthToDraw-1]); lcd()->print(" ");  lcd()->print(year);
  
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