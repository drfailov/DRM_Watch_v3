void drawWatchfaceBig(bool firstDraw){
  //Serial.println("clear screen...");
  lcd()->setColorIndex(white);
  lcd()->drawBox(0, 0, 400, 240);
  //Serial.println("draw time...");
  lcd()->setColorIndex(black);
  lcd()->setFont(u8g2_font_logisoso92_tn);
  int h = rtcGetHour();
  int m = rtcGetMinute();
  sprintf(buffer, "%02d:%02d", h,m);
  int width = lcd()->getStrWidth(buffer);
  lcd()->setCursor((400-width)/2, 160);
  lcd()->print(buffer); 
  //Serial.println("draw date...");
  drawDate(5, 20);
  drawDayOfWeek(375, 20);
  //Serial.println("draw temperature...");
  drawTemperature(5, 213);
  drawStatusbar(395, 214, false);
}