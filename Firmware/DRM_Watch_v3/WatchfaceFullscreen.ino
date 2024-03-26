void drawWatchfaceFullscreen(bool firstDraw){

  if(getWatchfaceLifeBackgroundEnabled()){
    if(firstDraw && millis()>2000)
      randScreenBuffer();
    lifeStep();
    drawScreenBuffer();
  }
  else{
    lcd()->setColorIndex(white);
    lcd()->drawBox(0, 0, 400, 240);
  }

  if(getWatchfaceDigitalEnabled()){
    //Serial.println("draw time...");
    lcd()->setColorIndex(black);
    lcd()->setFont(u8g2_font_logisoso92_tn);
    int h = rtcGetHour();
    int m = rtcGetMinute();
    sprintf(buffer, "%02d:%02d", h,m);
    int width = lcd()->getStrWidth(buffer);
    lcd()->setCursor((400-width)/2, 160);
    lcd()->print(buffer); 
  }
  else if(getWatchfaceAnalogEnabled()){
    float h = rtcGetHour();
    float m = rtcGetMinute();
    float clockRadius = ((float)H)*0.40;
    int centerX = W/2;
    int centerY = H/2;
    if(firstDraw)
      animateClock(/*centerX*/centerX, /*centerY*/centerY, /*clockRadius*/clockRadius, /*hour*/h, /*minute*/m);
    drawClock(/*centerX*/centerX, /*centerY*/centerY, /*clockRadius*/clockRadius, /*hour*/h, /*minute*/m);
  }
  
  if(getWatchfaceDateEnabled())
    drawDate(5, 20);
  
  if(getWatchfaceDjiLogoEnabled())
    draw_ic28_dji(320, 110, black);

  if(getWatchfaceDayOfWeekEnabled())
    drawDayOfWeek(375, 20);

  if(getWatchfaceTemperatureEnabled())
    drawTemperature(5, 213);

  if(getWatchfaceStatusbarEnabled()){
    lcd()->setColorIndex(white);
    lcd()->drawBox(0, 215, 400, 25);
    drawStatusbar(395, 214, false);
  }
}