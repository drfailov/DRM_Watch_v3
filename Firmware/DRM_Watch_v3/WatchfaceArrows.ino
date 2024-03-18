void drawWatchfaceArrows(bool firstDraw){  
  //Serial.println("clear screen...");
  //lcd()->setColorIndex(white);
  //lcd()->drawBox(0, 0, 400, 240);

  if(firstDraw && millis()>2000)
    randScreenBuffer();
  lifeStep();
  drawScreenBuffer();

  lcd()->setColorIndex(white);
  lcd()->drawBox(0, 215, 400, 25);
  drawStatusbar(395, 214, false);

  float h = rtcGetHour();
  float m = rtcGetMinute();
  float clockRadius = ((float)H)*0.35;
  int centerX = W/2;
  int centerY = H/2-10;
  if(firstDraw)
    animateClock(/*centerX*/centerX, /*centerY*/centerY, /*clockRadius*/clockRadius, /*hour*/h, /*minute*/m);
  drawClock(/*centerX*/centerX, /*centerY*/centerY, /*clockRadius*/clockRadius, /*hour*/h, /*minute*/m);
}

void animateClock(int centerX, int centerY, float clockRadius, float h, float m){
  float ch = 0;
  float cm = 0;
  float hour12 = fmod(h,12);
  const float coef = 0.28;
  for(int i=0; i<14; i++){
    float dh = hour12-ch;
    float dm = m-cm;
    ch+=dh*coef;
    cm+=dm*coef;
    if(i>4){ //пропустити кілька кроків анімації щоб вона виглядала більш ефектно
      drawClock(/*centerX*/centerX, /*centerY*/centerY, /*clockRadius*/clockRadius, /*hour*/ch, /*minute*/cm);
      lcd()->sendBuffer();
    }
  }
}

void drawClock(int centerX, int centerY, float clockRadius, float h, float m) {
  //додати корекцію годинникової стрілки щоб вона враховувала хвилини
  float hour12 = fmod(h,12);
  float hourIncludingMinutes = hour12+((m) / 60.0);
  h=hourIncludingMinutes;

  //фон
  lcd()->setColorIndex(white);
  lcd()->drawDisc(centerX, centerY, clockRadius);

  // Малюємо контур циферблату
  float ringSize = max(clockRadius*0.05, 2.0);
  lcd()->setColorIndex(black);
  for(int i=0; i<ringSize; i++)
    lcd()->drawCircle(centerX, centerY, clockRadius+i);
  lcd()->setColorIndex(white);
  for(int i=ringSize; i<ringSize*2; i++)
    lcd()->drawCircle(centerX, centerY, clockRadius+i);

  {// Малюємо шкалу з цифрами
    lcd()->setColorIndex(black);
    lcd()->setFont(u8g2_font_unifont_t_cyrillic); //smalll
    float angle = -PI/2;
    for (int i = 0; i < 12; i++) {
      float x = centerX + cos(angle) * (clockRadius - 16);
      float y = centerY + sin(angle) * (clockRadius - 16);
      float th = 0;
      angle += (PI / 6);
      if(i==0 || i==3 || i==6 || i==9){
        lcd()->setFont(u8g2_font_10x20_t_cyrillic); //normal
        th = 15;
      }
      else{
        lcd()->setFont(u8g2_font_unifont_t_cyrillic); //smalll
        th = 12;
      }
      int num = i==0?12:i;
      itoa(num, buffer, 10);
      float tw = lcd()->getStrWidth(buffer);
      lcd()->setCursor(x-tw/2, y + th/2);
      lcd()->print(buffer);
    }  
  }

  {// Малюємо насічки
    lcd()->setColorIndex(black);
    float angle = -PI/2;
    for (int i = 0; i < 60; i++) {
      float x1 = centerX + cos(angle) * (clockRadius - 4);
      float y1 = centerY + sin(angle) * (clockRadius - 4);
      float x2 = centerX + cos(angle) * (clockRadius - 7);
      float y2 = centerY + sin(angle) * (clockRadius - 7);
      angle += (PI / 30);
      if(i==0 || i==15 || i==30 || i==45)
        drawThickLine(x1, y1, x2, y2, 2);
      else if(i==5||i==10  ||  i==20||i==25  ||  i==35||i==40  ||  i==50||i==55)
        drawThickLine(x1, y1, x2, y2, 1);
      else
        lcd()->drawLine(x1, y1, x2, y2);
    }
  }
  
  //точка в цетнрі
  lcd()->setColorIndex(black);
  lcd()->drawDisc(centerX, centerY, 7);

  // Малюємо стрілку годин
  float hourAngleRad = map(h, 0, 12, PI/2, PI/2 - 2*PI);
  int handHSize = (clockRadius-20) * 0.65;
  int hourX = centerX + handHSize * cos(hourAngleRad);
  int hourY = centerY - handHSize * sin(hourAngleRad);
  drawArrow(centerX, centerY, hourX, hourY, clockRadius*0.037);
  
  // Малюємо стрілку хвилин
  float minuteAngleRad = map(m, 0, 60, PI/2, PI/2 - 2*PI);
  int handMSize = (clockRadius-20) * 0.91;
  int minuteX = centerX + handMSize * cos(minuteAngleRad);
  int minuteY = centerY - handMSize * sin(minuteAngleRad);
  drawArrow(centerX, centerY, minuteX, minuteY, clockRadius*0.022);
}

void drawArrow(float centerX, float centerY, float toX, float toY, float thickness){//draw arrow and with thickness (how many layers to add over this line )
  float dmid = thickness+10;
  float dwing = thickness+2;
  float dx = toX-centerX;
  float dy = toY-centerY;
  float d = sqrt(dx*dx+dy*dy);
  float ux = dx/d;
  float uy = dy/d;
  float midx = toX-ux*dmid;
  float midy = toY-uy*dmid;
  float winglx = midx + uy*dwing;
  float wingly = midy - ux*dwing;
  float wingrx = midx - uy*dwing;
  float wingry = midy + ux*dwing;
  float lex = midx+ux;
  float ley = midy+uy;
  lcd()->setColorIndex(black);
  lcd()->drawTriangle(/*x0*/toX, /*y0*/toY, /*x1*/winglx, /*y1*/wingly, /*x2*/wingrx, /*y2*/wingry);
  drawThickLine(centerX, centerY, lex, ley, thickness);
}

void drawThickLine(float centerX, float centerY, float toX, float toY, float thickness){ 
  float dx = toX-centerX;
  float dy = toY-centerY;
  float d = sqrt(dx*dx+dy*dy);
  float ux = dx/d;
  float uy = dy/d;
  float mux = ux*thickness;
  float muy = uy*thickness;
  float rx1 = muy;   //https://limnu.com/sketch-easy-90-degree-rotate-vectors/
  float ry1 = -mux;
  float rx2 = -muy;
  float ry2 = mux;

  float x0 = centerX+rx1;
  float y0 = centerY+ry1;
  float x1 = centerX+rx2;
  float y1 = centerY+ry2;
  float x2 = toX+rx1;
  float y2 = toY+ry1;
  float x3 = toX+rx2;
  float y3 = toY+ry2;
  lcd()->drawTriangle(/*x0*/x0, /*y0*/y0, /*x1*/x1, /*y1*/y1, /*x2*/x2, /*y2*/y2);
  lcd()->drawTriangle(/*x0*/x1, /*y0*/y1, /*x1*/x3, /*y1*/y3, /*x2*/x2, /*y2*/y2);
}

float map(float x, float in_min, float in_max, float out_min, float out_max)
{
  return (float)(x - in_min) * (out_max - out_min) / (float)(in_max - in_min) + out_min;
}