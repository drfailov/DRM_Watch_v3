#include <time.h>
int year = 0;
int month = 0;/*01-12*/
int day = 0;
String days[7] = { "Пн", "Вт", "Ср", "Чт", "Пт", "Сб", "Нд" };
String months[12] = { "Січень", "Лютий", "Березень", "Квітень", "Травень", "Червень", "Липень", "Серпень", "Вересень", "Жовтень", "Листопад", "Грудень"};
tm _tm;


void setModeCalendar(){
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
  month = rtcGetMonth()+1;/*00-11*/
  day = rtcGetDay();
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
  int xoffset = 28; //px
  int yoffset = 90; //px
  int ylegend = 80; //px
  int dayw = 38;//px
  int dayh = 23;//px  
  lcd()->setColorIndex(black);
  lcd()->setCursor(xheader, yheader); 
  lcd()->print("("); lcd()->print(month); lcd()->print(") "); 
  lcd()->print(months[month-1]); lcd()->print(" ");  lcd()->print(year);
  for(int DoW=0; DoW<7; DoW++){
    int x = xoffset + (DoW+1)*dayw;
    lcd()->setCursor(x, ylegend); 
    lcd()->print(days[DoW]);
  }
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
      int x = xoffset + DoW*dayw;
      int y = yoffset + week*dayh;
      lcd()->setCursor(x, y); 
      lcd()->print(drawday);
      if(DoW == 7) week++;
    }
  }
  lcd()->setColorIndex(black);
  lcd()->drawBox(369, 0, 2, 260);
  draw_ic16_arrow_up(lx(), ly1(), black);
  draw_ic16_back(lx(), ly2(), black);
  draw_ic16_arrow_up(lx(), ly3(), black);
  lcd()->sendBuffer();
}

void ModeCalendarButtonUp(){
  month--;
  if(month < 1){
    year--;
    month = 12;
  }
}

void ModeCalendarButtonCenter(){
  setModeAppsMenu();
}

void ModeCalendarButtonDown(){
  month++;
  if(month > 12){
    year++;
    month = 1;
  }
}


//----------------------//----------------------//----------------------//----------------------//----------------------//----------------------//----------------------
