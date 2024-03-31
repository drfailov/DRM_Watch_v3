#include "driver/rtc_io.h"
//RTC_DATA_ATTR 
U8G2_LS027B7DH01_400X240_F_4W_HW_SPI u8g2(U8G2_R0, /* cs=*/ 5, /* dc=*/ U8X8_PIN_NONE, /* reset=*/ U8X8_PIN_NONE); //on/*CLK*/7, /*SDA*/11, /*CS*/5   //WORKS!!!!!!!!

  //"sdsdd" = const char [5]

  ////lcd()->setFont(u8g2_font_unifont_t_cyrillic);   //+   small
  //lcd()->setFont(u8g2_font_cu12_t_cyrillic);  //small
  //lcd()->setFont(u8g2_font_inr24_t_cyrillic);  //big
  //lcd()->setFont(u8g2_font_10x20_t_cyrillic);  //+   ok
  

void lcdInit(){
  if(isOff())
    return;
  lcdPowerOn();
  if(esp_sleep_get_wakeup_cause() == 0)/*If first start*/
    u8g2.begin();
  else
    u8g2.initInterface();
  u8g2.setBusClock(2300000);   //1MHz is default, 2MHz is max recommended, 3.5MHz is still OK, 4MHz is glitchy, 5MHZ not working at all
  u8g2.enableUTF8Print();  
  u8g2.setBitmapMode(0); //1-is transparent   0-not transparent
}

void lcdPowerOn(){
  pinMode(LCD_EN, OUTPUT);
  digitalWrite(LCD_EN, HIGH);
  
  rtc_gpio_hold_dis(LCD_EN);
}
void lcdPowerOff(){
  pinMode(LCD_EN, OUTPUT);
  digitalWrite(LCD_EN, LOW);
  //pinMode(LCD_EN, INPUT_PULLDOWN);
  //gpio_hold_dis(LCD_EN);
  //gpio_deep_sleep_hold_dis();
  rtc_gpio_hold_en(LCD_EN);

  //pinMode(LCD_EN, OUTPUT);
  //digitalWrite(LCD_EN, LOW);
}

U8G2_LS027B7DH01_400X240_F_4W_HW_SPI* lcd(){
  return &u8g2;
}

void clearScreenAnimation(){
  if(esp_sleep_get_wakeup_cause() == ESP_SLEEP_WAKEUP_TIMER)/*periodical wakeup*/
    return;
  if(millis()<1000)
    return;
  firstDraw = true;

  //clearScreenAnimationWhiteLeftToRight();
  clearScreenAnimationCircleFromRight();
  //clearScreenAnimationWhiteFromRight();
}

void clearScreenAnimationCircleFromRight(){
  int curr = 0;
  float targ = 80;
  while(targ < W){
    targ *=1.6;
    lcd()->setColorIndex(white);
    for(; curr<targ; curr+=1){
      lcd()->drawCircle(/*x0*/400, /*y0*/H/2, /*rad*/curr, /*opt*/U8G2_DRAW_UPPER_LEFT|U8G2_DRAW_LOWER_LEFT);   //drawDisc     U8G2_DRAW_ALL
    }
    lcd()->setColorIndex(black);
    lcd()->drawCircle(/*x0*/400, /*y0*/H/2, /*rad*/curr+2, /*opt*/U8G2_DRAW_UPPER_LEFT|U8G2_DRAW_LOWER_LEFT);   //drawDisc     U8G2_DRAW_ALL
    lcd()->sendBuffer();
  }
}
void clearScreenAnimationWhiteFromRight(){
  lcd()->setColorIndex(white);
  int step = 90;
  for(int x=W-step; x>0; x-=step){
    lcd()->drawBox(/*x*/x, /*y*/0, /*w*/step, /*h*/H);
    lcd()->sendBuffer();
  }
}
void clearScreenAnimationWhiteLeftToRight(){
  lcd()->setColorIndex(white);
  int step = 100;
  for(int x=0; x<W; x+=step){
    lcd()->drawBox(/*x*/x, /*y*/0, /*w*/step, /*h*/H);
    lcd()->sendBuffer();
  }
}

void drawMessage(String text){
  Serial.println(text);
  int x = 15;
  int y = 79;
  int width = 345;
  int height = 70;
  lcd()->setColorIndex(white);
  lcd()->drawBox(/*x*/x+1, /*y*/y+1, /*w*/width-2, /*h*/height-2);
  lcd()->setColorIndex(black);
  lcd()->drawFrame(/*x*/x, /*y*/y, /*w*/width, /*h*/height);
  lcd()->drawFrame(/*x*/x+1, /*y*/y+1, /*w*/width-2, /*h*/height-2);
  draw_ic24_about(x+10, y+22, black);

  lcd()->setColorIndex(black);
  lcd()->setFont(u8g2_font_10x20_t_cyrillic);
  lcd()->setCursor(x+45, y+39); 
  for(int i=0; i<text.length(); i++){
    lcd()->print(text.charAt(i));
    if(i%3==1)
      lcd()->sendBuffer();
  }  
  lcd()->sendBuffer();
  delay(200);
}

void drawMessage(String text, String text2){
  drawMessage(text, text2, false);
}
void drawMessage(String text, String text2, bool animate){
  Serial.println(text);
  Serial.println(text2);
  int x = 15;
  int y = 79;
  int width = 345;
  int height = 70;
  lcd()->setColorIndex(white);
  lcd()->drawBox(/*x*/x+1, /*y*/y+1, /*w*/width-2, /*h*/height-2);
  lcd()->setColorIndex(black);
  lcd()->drawFrame(/*x*/x, /*y*/y, /*w*/width, /*h*/height);
  lcd()->drawFrame(/*x*/x+1, /*y*/y+1, /*w*/width-2, /*h*/height-2);
  draw_ic24_about(x+10, y+22, black);
  lcd()->setColorIndex(black);

  lcd()->setFont(u8g2_font_10x20_t_cyrillic);
  lcd()->setCursor(x+45, y+29); 
  if(animate){
    for(int i=0; i<text.length(); i++){
      lcd()->print(text.charAt(i));
      if(i%3==1)
        lcd()->sendBuffer();
    }
  }
  else{
    lcd()->print(text);
  }
  
  lcd()->setFont(u8g2_font_unifont_t_cyrillic);  //ok
  lcd()->setCursor(x+45, y+49);
  if(animate){
    for(int i=0; i<text2.length(); i++){
      lcd()->print(text2.charAt(i));
      if(i%3==1)
        lcd()->sendBuffer();
    }
  }
  else{
    lcd()->print(text2);
  }
  
  if(animate){
    lcd()->sendBuffer();
    delay(300);
  }
}

void drawQuestion(String text, String text2){
  drawQuestion(text, text2, false);
}

void drawQuestion(String text, String text2, bool animate){
  Serial.println(text);
  Serial.println(text2);
  int x = 15;
  int y = 79;
  int width = 345;
  int height = 70;
  lcd()->setColorIndex(white);
  lcd()->drawBox(/*x*/x+1, /*y*/y+1, /*w*/width-2, /*h*/height-2);
  lcd()->setColorIndex(black);
  lcd()->drawFrame(/*x*/x, /*y*/y, /*w*/width, /*h*/height);
  lcd()->drawFrame(/*x*/x+1, /*y*/y+1, /*w*/width-2, /*h*/height-2);
  draw_ic24_question(x+10, y+22, black);

  lcd()->setColorIndex(black);

  lcd()->setFont(u8g2_font_10x20_t_cyrillic);
  lcd()->setCursor(x+45, y+29); 
  if(animate){
    for(int i=0; i<text.length(); i++){
      lcd()->print(text.charAt(i));
      if(i%3==1)
        lcd()->sendBuffer();
    }
  }
  else
    lcd()->print(text);
  
  lcd()->setFont(u8g2_font_unifont_t_cyrillic);  //ok
  lcd()->setCursor(x+45, y+49); 
  if(animate){
    for(int i=0; i<text2.length(); i++){
      lcd()->print(text2.charAt(i));
      if(i%3==1)
        lcd()->sendBuffer();
    }
  }
  else
    lcd()->print(text2);

  lcd()->sendBuffer();
}

void displayPrintSecondsAsTime(unsigned long dd){
  unsigned long spday = (24*60*60);
  unsigned long days = dd / spday;
  dd -= days*spday;
  unsigned long sphour = (60*60);
  unsigned long hours = dd / sphour;
  dd -= hours*sphour;
  unsigned long spminute = 60;
  unsigned long minutes = dd / spminute;
  dd -= minutes*spminute;
  unsigned long seconds = dd;
  
  if(days>0){
    lcd()->print(days); 
    lcd()->print("d "); 
  }
  if(hours < 10) lcd()->print("0"); 
  lcd()->print(hours); 
  lcd()->print(":"); 
  if(minutes < 10) lcd()->print("0"); 
  lcd()->print(minutes); 
  lcd()->print(":"); 
  if(seconds < 10) lcd()->print("0"); 
  lcd()->print(seconds); 
}



//Рисование векторной картинки. Формат вектора описан в комментарии ниже.
//max number of points for one path is 255.
//Array type: const PROGMEM byte path[] = {...);
//Array format: {numberOfPoints, x0, y0, x1, y1, x2, y2, ...}
//255,255 coordinates is skipped, so it can be used for separate paths
//animate: 0-noAnimation, 1...-speed,low is slow.
void displayDrawVector(const byte* data_array, int X, int Y, float scale, int thickness, byte animate, bool color){
  lcd()->setColorIndex(color);
  byte numberOfPoints = (data_array[0]);
  byte lx = (data_array[1]);
  byte ly = (data_array[2]);
  int currentIndex = 3;
  for(byte i=1; i < numberOfPoints; i++){
    byte x = (data_array[currentIndex]);
    byte y = (data_array[currentIndex + 1]);
    if(x != 255 && y != 255 && lx != 255 && ly != 255){
      for(int ox = X; ox < X+thickness; ox++)
        for(int oy = Y; oy < Y+thickness; oy++)
          lcd()->drawLine(/*X1*/lx*scale + ox, /*Y1*/ly*scale + oy, /*X2*/x*scale + ox, /*Y2*/y*scale + oy);
    }
    if(animate!=0 && i%animate==0){
      if (!isAnyButtonPressed()) {
        lcd()->sendBuffer();
      }
    }
    currentIndex += 2;
    lx = x;
    ly = y;
  }
}






















//----------------------//----------------------//----------------------//----------------------//----------------------//----------------------//----------------------



  // u8g2.clearBuffer();

  // u8g2.sendBuffer();
  // drawLogo();
  // u8g2.sendBuffer();
  // drawURL();
  // u8g2.sendBuffer();
  // u8g2.drawBox(30, 40, 300, 150);
  // u8g2.sendBuffer();
  // delay(1000);
void drawLogo(void)
{
    u8g2.setFontMode(1);	// Transparent
#ifdef MINI_LOGO

    u8g2.setFontDirection(0);
    u8g2.setFont(u8g2_font_inb16_mf);
    u8g2.drawStr(0, 22, "U");
    
    u8g2.setFontDirection(1);
    u8g2.setFont(u8g2_font_inb19_mn);
    u8g2.drawStr(14,8,"8");
    
    u8g2.setFontDirection(0);
    u8g2.setFont(u8g2_font_inb16_mf);
    u8g2.drawStr(36,22,"g");
    u8g2.drawStr(48,22,"\xb2");
    
    u8g2.drawHLine(2, 25, 34);
    u8g2.drawHLine(3, 26, 34);
    u8g2.drawVLine(32, 22, 12);
    u8g2.drawVLine(33, 23, 12);
#else

    u8g2.setFontDirection(0);
    u8g2.setFont(u8g2_font_inb24_mf);
    u8g2.drawStr(0, 30, "U");
    
    u8g2.setFontDirection(1);
    u8g2.setFont(u8g2_font_inb30_mn);
    u8g2.drawStr(21,8,"8");
        
    u8g2.setFontDirection(0);
    u8g2.setFont(u8g2_font_inb24_mf);
    u8g2.drawStr(51,30,"g");
    u8g2.drawStr(67,30,"\xb2");
    
    u8g2.drawHLine(2, 35, 47);
    u8g2.drawHLine(3, 36, 47);
    u8g2.drawVLine(45, 32, 12);
    u8g2.drawVLine(46, 33, 12);
    
#endif
}

void drawURL(void)
{
#ifndef MINI_LOGO
  u8g2.setFont(u8g2_font_4x6_tr);
  if ( u8g2.getDisplayHeight() < 59 )
  {
    u8g2.drawStr(89,20,"github.com");
    u8g2.drawStr(73,29,"/olikraus/u8g2");
  }
  else
  {
    u8g2.drawStr(1,54,"github.com/olikraus/u8g2");
  }
#endif
}