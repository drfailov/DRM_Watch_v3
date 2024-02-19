void drawWatchfaceLife(bool firstDraw){
  int x_del = 240;
  if(firstDraw)
    randScreenBuffer();
  lifeStep();
  drawScreenBuffer(0,0,x_del/BUFF_SCALE,BUFF_H);

  lcd()->setColorIndex(white);
  lcd()->drawBox(x_del, 0, lcd_w-x_del, 240);
  
  lcd()->setColorIndex(black);
  lcd()->drawBox(x_del, 0, 4, 240);
  //Serial.println("draw time...");
  lcd()->setColorIndex(black);
  lcd()->setFont(u8g2_font_inr24_t_cyrillic);
  int h = rtcGetHour();
  int m = rtcGetMinute();
  sprintf(buffer, "%02d:%02d", h,m);
  int width = lcd()->getStrWidth(buffer);
  lcd()->setCursor(x_del+((lcd_w-x_del)-width)/2, 130);
  lcd()->print(buffer); 
  drawTemperature(x_del+4, 3);
  drawDayOfWeek(375, 20);
  drawDate(x_del+25, 60);
  
  
  drawStatusbar(395, 214, false);
}