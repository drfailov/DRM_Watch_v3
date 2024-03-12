void drawWatchfaceLife(bool firstDraw){
  int x_del = 240;
  if(firstDraw && millis()>2000)
    randScreenBuffer();
  lifeStep();
  drawScreenBuffer(0,0,x_del/BUFF_SCALE,BUFF_H);

  lcd()->setColorIndex(white);
  lcd()->drawBox(x_del, 0, W-x_del, 240);
  lcd()->setColorIndex(black);
  lcd()->drawBox(x_del, 0, 4, 240);
  if(firstDraw) lcd()->sendBuffer();
  
  drawTemperature(x_del+4, 3);
  drawDayOfWeek(375, 20);
  if(firstDraw) lcd()->sendBuffer();
  
  drawDate(x_del+25, 60);
  if(firstDraw) lcd()->sendBuffer();

  lcd()->setColorIndex(black);
  lcd()->setFont(u8g2_font_inr24_t_cyrillic);
  int h = rtcGetHour();
  int m = rtcGetMinute();
  sprintf(buffer, "%02d:%02d", h,m);
  int width = lcd()->getStrWidth(buffer);
  lcd()->setCursor(x_del+((W-x_del)-width)/2, 130);
  lcd()->print(buffer); 
  
  drawStatusbar(395, 214, false);
}