void drawWatchfaceSide(bool firstDraw){
  int x_del = 240;

  if(getWatchfaceLifeBackgroundEnabled()){
    if(firstDraw && millis()>2000)
      randScreenBuffer();
    lifeStep();
    drawScreenBuffer(0,0,x_del/BUFF_SCALE,BUFF_H);
  }
  else if(getWatchfaceFireBackgroundEnabled()){
    fireStep(x_del/BUFF_SCALE + 1);
    drawScreenBuffer(0,0,x_del/BUFF_SCALE,BUFF_H);
  }
  else if(getWatchfaceLavaBackgroundEnabled()){
    lavaStep(x_del/BUFF_SCALE + 1);
    drawScreenBuffer(0,0,x_del/BUFF_SCALE,BUFF_H);
  }
  else if(getWatchfaceRandomBackgroundEnabled()){
    randScreenBuffer();
    drawScreenBuffer(0,0,x_del/BUFF_SCALE,BUFF_H);
  }
  else{
    lcd()->setColorIndex(white);
    lcd()->drawBox(0, 0, 400, 240);
  }

  lcd()->setColorIndex(white);
  lcd()->drawBox(x_del, 0, W-x_del, 240);
  lcd()->setColorIndex(black);
  lcd()->drawBox(x_del, 0, 4, 240);
  if(firstDraw) lcd()->sendBuffer();
  
  if(getWatchfaceTemperatureEnabled())
    drawTemperature(x_del+4, 3);

  if(getWatchfaceDayOfWeekEnabled())
    drawDayOfWeek(375, 20);
  if(firstDraw) lcd()->sendBuffer();
  
  if(getWatchfaceDateEnabled())
    drawDate(x_del+30, 57);

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
    drawCalendar(/*x*/45, /*y*/68, /*month 01-12*/rtcGetMonth()+1, rtcGetYear(), /*compact*/true);
  }
  
  
  /*getWatchfaceCalendarEnabled*/
}