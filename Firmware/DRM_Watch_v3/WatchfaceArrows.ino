void drawWatchfaceArrows(bool firstDraw){  
  //Serial.println("clear screen...");
  //lcd()->setColorIndex(white);
  //lcd()->drawBox(0, 0, 400, 240);

  if(firstDraw && millis()>2000)
    randScreenBuffer();
  lifeStep();
  drawScreenBuffer();

  int h = rtcGetHour();
  int m = rtcGetMinute();
  int s = rtcGetSecond();
  int clockRadius = H/3;
  int centerX = W/2;
  int centerY = H/2;
  drawClock(/*centerX*/centerX, /*centerY*/centerY, /*clockRadius*/clockRadius, /*hour*/h, /*minute*/m, /*second*/s);
}


void drawClock(int centerX, int centerY, float clockRadius, int hour, int minute, int second) {
  //фон
  lcd()->setColorIndex(white);
  lcd()->drawDisc(centerX, centerY, clockRadius);

  // Малюємо контур циферблату
  float ringSize = max(clockRadius/17.0, 2.0);
  lcd()->setColorIndex(black);
  for(int i=0; i<ringSize; i++)
    lcd()->drawCircle(centerX, centerY, clockRadius+i);
  lcd()->setColorIndex(white);
  for(int i=ringSize; i<ringSize*2; i++)
    lcd()->drawCircle(centerX, centerY, clockRadius+i);

  // Малюємо шкалу з цифрами
  lcd()->setColorIndex(black);
  lcd()->setFont(u8g2_font_unifont_t_cyrillic); //smalll
  float angle = -PI/2;
  for (int i = 0; i < 12; i++) {
    float x = centerX + cos(angle) * (clockRadius - 10);
    float y = centerY + sin(angle) * (clockRadius - 10);
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
  
  //точка в цетнрі
  lcd()->setColorIndex(black);
  lcd()->drawDisc(centerX, centerY, 4);

  // Малюємо стрілку годин
  float hour12 = hour%12;
  float hourIncludingMinutes = hour12+(((float)minute) / 60.0);
  float hourAngleRad = map(hourIncludingMinutes, 0, 12, PI/2, PI/2 - 2*PI);
  int handHSize = clockRadius * 0.5;
  int hourX = centerX + handHSize * cos(hourAngleRad);
  int hourY = centerY - handHSize * sin(hourAngleRad);
  lcd()->drawLine(centerX, centerY, hourX, hourY);
  
  // Малюємо стрілку хвилин
  float minuteIncludingSeconds = minute+(((float)second) / 60.0);
  float minuteAngleRad = map(minuteIncludingSeconds, 0, 60, PI/2, PI/2 - 2*PI);
  int handMSize = clockRadius * 0.7;
  int minuteX = centerX + handMSize * cos(minuteAngleRad);
  int minuteY = centerY - handMSize * sin(minuteAngleRad);
  lcd()->drawLine(centerX, centerY, minuteX, minuteY);


  //float hourAngle=(hour) * 2 * 3.14 / 720;
  //lcd()->drawTriangle(64 - (int)3 * sin(hourAngle + 1.57), 32 + (int)3 * cos(hourAngle + 1.57), 64 - (int)3 * sin(hourAngle - 1.57), 32 + (int)3 * cos(hourAngle - 1.57), 64 - (int)25 * sin(hourAngle), 32 + (int)25 * cos(hourAngle));

  lcd()->sendBuffer();
}

float map(float x, float in_min, float in_max, float out_min, float out_max)
{
  return (float)(x - in_min) * (out_max - out_min) / (float)(in_max - in_min) + out_min;
}