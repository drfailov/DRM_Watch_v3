/*
CTRL+K CTRL+M  - fold
CTRL+K CTRL+J  - unfold
*/
#ifndef LCD_H
#define LCD_H

#include <U8g2lib.h>

U8G2_LS027B7DH01_400X240_F_4W_HW_SPI *lcd();
void lcdPowerOn();
void lcdPowerOff();
void clearScreenAnimationCircleFromRight();
void clearScreenAnimationWhiteFromRight();
void clearScreenAnimationWhiteLeftToRight();
void clearScreenAnimationGlitch();
void clearScreenAnimationNoize();
void drawMessage(String text, String text2, bool animate);
void drawMessage(String text, String text2);
void drawCentered(String str);
void drawCentered(const char *str);
void drawCentered(String str, int y);
void drawCentered(const char *str, int y);
void drawDashedLine(u8g2_uint_t x1, u8g2_uint_t y1, u8g2_uint_t x2, u8g2_uint_t y2, u8g2_int_t d);
void drawPlot(int x, int y, int w, int h, int thickness, bool legend, int16_t* rangeValues, int16_t* values, int length, int highlightIndex);
void print(char *arr, int length);
const char* L(const char* ua, const char* en);
const char* langName(int lang);
int langCnt();
void waitOk();
void drawDim();
void drawWarningFrame();


#include "Global.h"
#include "ModeOff.h"
#include "AutoSleep.h"
#include "DrmPreferences.h"
#include "Led.h"
#include "Button.h"
#include "Battery.h"
#include "Icons.h"

#include "driver/rtc_io.h"
// RTC_DATA_ATTR
U8G2_LS027B7DH01_400X240_F_4W_HW_SPI u8g2(U8G2_R0, /* cs=*/5, /* dc=*/U8X8_PIN_NONE, /* reset=*/U8X8_PIN_NONE); // on/*CLK*/7, /*SDA*/11, /*CS*/5   //WORKS!!!!!!!!

//"sdsdd" = const char [5]

////lcd()->setFont(u8g2_font_unifont_t_cyrillic);   //+   small
// lcd()->setFont(u8g2_font_cu12_t_cyrillic);  //small
// lcd()->setFont(u8g2_font_inr24_t_cyrillic);  //big
// lcd()->setFont(u8g2_font_10x20_t_cyrillic);  //+   ok

void lcdInit()
{
  if (isOff())
    return;
  lcdPowerOn();
  if (esp_sleep_get_wakeup_cause() == 0) /*If first start*/
    u8g2.begin();
  else
    u8g2.initInterface();
  unsigned long spiSpeed = getLcdSpiSpeed();
  if(!isAwake() || isOff())
    if(spiSpeed > 1000000)
      spiSpeed = 1000000;
  u8g2.setBusClock(spiSpeed);
  u8g2.enableUTF8Print();
  u8g2.setBitmapMode(0); // 1-is transparent   0-not transparent
}

void lcdPowerOn()
{
  pinMode(LCD_EN, OUTPUT);
  digitalWrite(LCD_EN, HIGH);

  rtc_gpio_hold_dis(LCD_EN);
}
void lcdPowerOff()
{
  pinMode(LCD_EN, OUTPUT);
  digitalWrite(LCD_EN, LOW);
  // pinMode(LCD_EN, INPUT_PULLDOWN);
  // gpio_hold_dis(LCD_EN);
  // gpio_deep_sleep_hold_dis();
  rtc_gpio_hold_en(LCD_EN);

  // pinMode(LCD_EN, OUTPUT);
  // digitalWrite(LCD_EN, LOW);
}

U8G2_LS027B7DH01_400X240_F_4W_HW_SPI *lcd()
{
  return &u8g2;
}

void clearScreenAnimation()
{
  if (!isAwake()) /*periodical wakeup*/
    return;
  if (millis() < 1000)
    return;
  firstDraw = getEnterAnimationValue();

  switch (getClearAnimation())
  {
  case 1:
    clearScreenAnimationWhiteLeftToRight();
    break;
  case 2:
    clearScreenAnimationWhiteFromRight();
    break;
  case 3:
    clearScreenAnimationCircleFromRight();
    break;
  case 4:
    clearScreenAnimationGlitch();
    break;
  case 5:
    clearScreenAnimationNoize();
    break;
  }
}

void clearScreenAnimationCircleFromRight()
{
  int curr = 0;
  float targ = 80;
  while (targ < W)
  {
    targ *= 1.6;
    lcd()->setColorIndex(white);
    for (; curr < targ; curr += 1)
    {
      lcd()->drawCircle(/*x0*/ 400, /*y0*/ H / 2, /*rad*/ curr, /*opt*/ U8G2_DRAW_UPPER_LEFT | U8G2_DRAW_LOWER_LEFT); // drawDisc     U8G2_DRAW_ALL
    }
    lcd()->setColorIndex(black);
    lcd()->drawCircle(/*x0*/ 400, /*y0*/ H / 2, /*rad*/ curr + 2, /*opt*/ U8G2_DRAW_UPPER_LEFT | U8G2_DRAW_LOWER_LEFT); // drawDisc     U8G2_DRAW_ALL
    lcd()->sendBuffer();
  }
}
void clearScreenAnimationWhiteFromRight()
{
  lcd()->setColorIndex(white);
  int step = 90;
  for (int x = W - step; x > 0; x -= step)
  {
    lcd()->drawBox(/*x*/ x, /*y*/ 0, /*w*/ step, /*h*/ H);
    lcd()->sendBuffer();
  }
}
void clearScreenAnimationWhiteLeftToRight()
{
  lcd()->setColorIndex(white);
  int step = 100;
  for (int x = 0; x < W; x += step)
  {
    lcd()->drawBox(/*x*/ x, /*y*/ 0, /*w*/ step, /*h*/ H);
    lcd()->sendBuffer();
  }
}
void clearScreenAnimationGlitch()
{
  for (int i = 0; i < 5; i++)
  {
    lcd()->setColorIndex(i == 1 ? black : white);
    for (int j = 0; j < 20; j++)
    {
      lcd()->drawBox(/*x*/ 0, /*y*/ random(H), /*w*/ W, /*h*/ i);
    }
    lcd()->sendBuffer();
  }
}
void clearScreenAnimationNoize()
{
  for (int i = 0; i < 4; i++)
  {
    lcd()->setColorIndex(i == 1 ? black : white);
    for (int j = 0; j < 5000; j++)
    {
      lcd()->drawBox(/*x*/ random(W), /*y*/ random(H), 2, 2);
    }
    lcd()->sendBuffer();
  }
}

const char* L(const char* ua, const char* en)
{
  switch (getLang())
  {
  case LANG_UA:
    return ua;
  case LANG_EN:
    return en;
  
  default:
    return ua;
  }
}
const char* langName(int lang)
{
  switch (getLang())
  {
  case LANG_UA:
    return "Українська";
  case LANG_EN:
    return "Engligh";
  
  default:
    return "---";
  }
}
int langCnt()
{
  return 2; //2 is [0,1]      4 is [0,1,2,3] ...
}

void print(char *arr, int length)
{
  for (int i = 0; i < length; i++)
    lcd()->print(arr[i]);
}
void drawCentered(String str)
{
  drawCentered(str, H / 2 - 10);
}
void drawCentered(const char *str)
{
  drawCentered(str, H / 2 - 10);
}
void drawCentered(String str, int y)
{
  lcd()->setFont(u8g2_font_10x20_t_cyrillic); // ok
  int width = lcd()->getUTF8Width(str.c_str());
  lcd()->setCursor((W - 30 - width) / 2, y);
  lcd()->print(str);
}
void drawCentered(const char *str, int y)
{
  lcd()->setFont(u8g2_font_10x20_t_cyrillic); // ok
  int width = lcd()->getUTF8Width(str);
  lcd()->setCursor((W - 30 - width) / 2, y);
  lcd()->print(str);
}
void drawCentered(const char *str, int x, int y)
{
  lcd()->setFont(u8g2_font_10x20_t_cyrillic); // ok
  int width = lcd()->getUTF8Width(str);
  lcd()->setCursor(x - (width / 2), y);
  lcd()->print(str);
}

void drawMessageAnimated(String text)
{
  Serial.println(text);
  ledStatusBlink(/*times*/ 1, /*speedMs*/ 100);
  int x = 15;
  int y = 79;
  int width = 345;
  int height = 70;
  lcd()->setColorIndex(black);
  lcd()->drawRBox(/*x*/ x, /*y*/ y, /*w*/ width, /*h*/ height, getRoundness());
  lcd()->setColorIndex(white);
  lcd()->drawRBox(/*x*/ x + frame, /*y*/ y + frame, /*w*/ width - frame * 2, /*h*/ height - frame * 2, getRoundness());
  draw_ic24_about(x + 10, y + 22, black);

  lcd()->setColorIndex(black);
  lcd()->setFont(u8g2_font_10x20_t_cyrillic);
  lcd()->setCursor(x + 45, y + 39);
  for (int i = 0; i < text.length(); i++)
  {
    lcd()->print(text.charAt(i));
    if (i % 3 == 1)
      lcd()->sendBuffer();
  }
  lcd()->sendBuffer();
  delay(200);
}

void drawMessage(String text, String text2)
{
  drawMessage(text, text2, false);
}
void drawMessage(String text, String text2, bool animate)
{
  Serial.println(text);
  Serial.println(text2);
  ledStatusBlink(/*times*/ 1, /*speedMs*/ 100);
  int x = 15;
  int y = 79;
  int width = 345;
  int height = 70;
  // lcd()->setColorIndex(white);
  // lcd()->drawBox(/*x*/x+1, /*y*/y+1, /*w*/width-2, /*h*/height-2);
  // lcd()->setColorIndex(black);
  // lcd()->drawFrame(/*x*/x, /*y*/y, /*w*/width, /*h*/height);
  // lcd()->drawFrame(/*x*/x+1, /*y*/y+1, /*w*/width-2, /*h*/height-2);

  lcd()->setColorIndex(black);
  lcd()->drawRBox(/*x*/ x, /*y*/ y, /*w*/ width, /*h*/ height, getRoundness());
  lcd()->setColorIndex(white);
  lcd()->drawRBox(/*x*/ x + frame, /*y*/ y + frame, /*w*/ width - frame * 2, /*h*/ height - frame * 2, getRoundness());

  draw_ic24_about(x + 10, y + 22, black);
  lcd()->setColorIndex(black);

  lcd()->setFont(u8g2_font_10x20_t_cyrillic);
  lcd()->setCursor(x + 45, y + 29);
  if (animate)
  {
    for (int i = 0; i < text.length(); i++)
    {
      lcd()->print(text.charAt(i));
      if (i % 3 == 1)
        lcd()->sendBuffer();
    }
  }
  else
  {
    lcd()->print(text);
  }

  lcd()->setFont(u8g2_font_unifont_t_cyrillic); // ok
  lcd()->setCursor(x + 45, y + 49);
  if (animate)
  {
    for (int i = 0; i < text2.length(); i++)
    {
      lcd()->print(text2.charAt(i));
      if (i % 3 == 1)
        lcd()->sendBuffer();
    }
  }
  else
  {
    lcd()->print(text2);
  }

  if (animate)
  {
    lcd()->sendBuffer();
    delay(400);
  }
}


/// @brief Draw plot on screen
/// @param x   Top left corner X
/// @param y   Top left corner Y
/// @param w   winth in pixels
/// @param h   height in pixels
/// @param thickness   thickness of line (0...3)
/// @param legend    If draw min, max and frame
/// @param rangeValues  Array of [length] items which range will be used as min and max. By default use same array as "values"  
/// @param values    Array of data to be displayed
/// @param length   Length of data. If set incorrectly - will be memory error
/// @param highlightIndex   //Index in element to be highlighted (1...lingth-2)
/// @example drawPlot (/*x*/10, /*y*/40, /*w*/360, /*h*/90, /*thickness*/2, /*legend*/true,  /*rangeValues*/ModeTestBatteryAnalysisHistoryDeviation, /*values*/ModeTestBatteryAnalysisHistoryDeviation,      /*length*/ModeTestBatteryAnalysisHistorySize, /*highlight*/0);
void drawPlot(int x, int y, int w, int h, int thickness, bool legend, int16_t* rangeValues, int16_t* values, int length, int highlightIndex)
{
  //config
  int padding = 15;

  //frame
  // lcd()->setColorIndex(white);
  // lcd()->drawBox(x, y, w, h);
  if(legend){
    lcd()->setColorIndex(black);
    lcd()->drawFrame(x, y, w, h);
    lcd()->drawFrame(x-1, y-1, w+2, h+2);
  }

  if(length == 0 && legend)
  {
    lcd()->setColorIndex(black);
    lcd()->setFont(u8g2_font_10x20_t_cyrillic); // ok
    strcpy(buffer, L("Немає даних", "No data"));
    int width = lcd()->getUTF8Width(buffer);
    lcd()->setCursor((W - 30 - width) / 2, y+h/2+5);
    lcd()->print(buffer);
    return;
  }
  
  //min and max
  int16_t min = rangeValues[0];
  int16_t max = rangeValues[0];
  for(int i = 1; i < length; i++){
    if(rangeValues[i] < min)
      min = rangeValues[i];
    if(rangeValues[i] > max)
      max = rangeValues[i];
  }
  if(legend){
    lcd()->setColorIndex(black);
    lcd()->setFont(u8g2_font_unifont_t_cyrillic); //smalll
    lcd()->setCursor(x+2, y+12);
    lcd()->print(max);
    lcd()->setCursor(x+2, y+h-2);
    lcd()->print(min);
  }
  
  //plot line
  float maxPoint = y+padding;
  float minPoint = y+h-padding;
  float leftPoint = x;
  float rightPoint = x+w;
  float lastX = map(0, 0, length, leftPoint, rightPoint);
  float lastY = map(values[0], min, max, minPoint, maxPoint);
  for(int i = 1; i < length; i++)
  {
    float cx = map(i, 0, length, leftPoint, rightPoint);
    float cy = map(values[i], min, max, minPoint, maxPoint);
    cy = constrain(cy, y, y+h);
    if(thickness == 0)
      drawDashedLine(cx, cy, lastX, lastY, 1);
    if (thickness > 0)
      lcd()->drawLine(cx, cy, lastX, lastY);
    if (thickness > 1)
      lcd()->drawLine(cx, cy+1, lastX, lastY+1);
    if (thickness > 2)
      lcd()->drawLine(cx, cy-1, lastX, lastY-1);
    lastX = cx;
    lastY = cy;
  }

  //highlight mark
  if(length > 1 && highlightIndex >= 0 && highlightIndex < length)
  {
    float cy = map(values[highlightIndex], min, max, minPoint, maxPoint);
    float cx = map(highlightIndex, 0, length, leftPoint, rightPoint);
    drawDashedLine(cx, y+padding, cx, y+h-padding-1, 1);
    lcd()->drawDisc(cx, cy, 2+thickness);
  }
}

//user must press OK to continue
void waitOk(){
  draw_ic16_empty(lx(), ly1(), black);
  draw_ic16_check(lx(), ly2(), black);
  draw_ic16_empty(lx(), ly3(), black);
  lcd()->sendBuffer();
  unsigned long started = millis();
  while(true){
    if(isButtonCenterPressed()){
      buttonBeep();
      while(isButtonCenterPressed());
      break;
    }
    if(millis()-started > 60000)
      break;
  }
}

void drawDim()
{
  lcd()->setColorIndex(white);
  for(int y=0; y<H; y++){
    for(int x=y%2; x<W; x+=2){
      lcd()->drawPixel(x, y);
    }
  }
}
void drawWarningFrame()
{
    int th=15;
    int tw=22;
    lcd()->setColorIndex(black);
    for(int x=0; x<W; x+=tw*2)
    {
      int y=0;
      lcd()->drawTriangle(x+th, y, x,       y+th, x+tw, y+th);
      lcd()->drawTriangle(x+th, y, x+tw+th, y   , x+tw, y+th);
      y=H-th;
      lcd()->drawTriangle(x+th, y, x,       y+th, x+tw, y+th);
      lcd()->drawTriangle(x+th, y, x+tw+th, y   , x+tw, y+th);
    }
  }

void drawQuestion(String text, String text2, bool animate)
{
  Serial.println(text);
  Serial.println(text2);
  int x = 15;
  int y = 79;
  int width = 345;
  int height = 70;
  lcd()->setColorIndex(white);
  // lcd()->drawBox(/*x*/x+1, /*y*/y+1, /*w*/width-2, /*h*/height-2);
  // lcd()->setColorIndex(black);
  // lcd()->drawFrame(/*x*/x, /*y*/y, /*w*/width, /*h*/height);
  // lcd()->drawFrame(/*x*/x+1, /*y*/y+1, /*w*/width-2, /*h*/height-2);

  lcd()->setColorIndex(black);
  lcd()->drawRBox(/*x*/ x, /*y*/ y, /*w*/ width, /*h*/ height, getRoundness());
  lcd()->setColorIndex(white);
  lcd()->drawRBox(/*x*/ x + frame, /*y*/ y + frame, /*w*/ width - frame * 2, /*h*/ height - frame * 2, getRoundness());

  draw_ic24_question(x + 10, y + 22, black);

  lcd()->setColorIndex(black);

  lcd()->setFont(u8g2_font_10x20_t_cyrillic);
  lcd()->setCursor(x + 45, y + 29);
  if (animate)
  {
    for (int i = 0; i < text.length(); i++)
    {
      lcd()->print(text.charAt(i));
      if (i % 3 == 1)
        lcd()->sendBuffer();
    }
  }
  else
    lcd()->print(text);

  lcd()->setFont(u8g2_font_unifont_t_cyrillic); // ok
  lcd()->setCursor(x + 45, y + 49);
  if (animate)
  {
    for (int i = 0; i < text2.length(); i++)
    {
      lcd()->print(text2.charAt(i));
      if (i % 3 == 1)
        lcd()->sendBuffer();
    }
  }
  else
    lcd()->print(text2);

  lcd()->sendBuffer();
}

void drawQuestion(String text, String text2)
{
  drawQuestion(text, text2, false);
}

void displayPrintSecondsAsTime(unsigned long dd)
{
  unsigned long spday = (24 * 60 * 60);
  unsigned long days = dd / spday;
  dd -= days * spday;
  unsigned long sphour = (60 * 60);
  unsigned long hours = dd / sphour;
  dd -= hours * sphour;
  unsigned long spminute = 60;
  unsigned long minutes = dd / spminute;
  dd -= minutes * spminute;
  unsigned long seconds = dd;

  if (days > 0)
  {
    lcd()->print(days);
    lcd()->print("d ");
  }
  if (hours < 10)
    lcd()->print("0");
  lcd()->print(hours);
  lcd()->print(":");
  if (minutes < 10)
    lcd()->print("0");
  lcd()->print(minutes);
  lcd()->print(":");
  if (seconds < 10)
    lcd()->print("0");
  lcd()->print(seconds);
}

// Рисование векторной картинки. Формат вектора описан в комментарии ниже.
// max number of points for one path is 255.
// Array type: const PROGMEM byte path[] = {...);
// Array format: {numberOfPoints, x0, y0, x1, y1, x2, y2, ...}
// 255,255 coordinates is skipped, so it can be used for separate paths
// animate: 0-noAnimation, 1...-speed,low is slow.
void displayDrawVector(const byte *data_array, int X, int Y, float scale, int thickness, byte animate, bool color)
{
  lcd()->setColorIndex(color);
  byte numberOfPoints = (data_array[0]);
  byte lx = (data_array[1]);
  byte ly = (data_array[2]);
  int currentIndex = 3;
  for (byte i = 1; i < numberOfPoints; i++)
  {
    byte x = (data_array[currentIndex]);
    byte y = (data_array[currentIndex + 1]);
    if (x != 255 && y != 255 && lx != 255 && ly != 255)
    {
      for (int ox = X; ox < X + thickness; ox++)
        for (int oy = Y; oy < Y + thickness; oy++)
          lcd()->drawLine(/*X1*/ lx * scale + ox, /*Y1*/ ly * scale + oy, /*X2*/ x * scale + ox, /*Y2*/ y * scale + oy);
    }
    if (animate != 0 && i % animate == 0)
    {
      if (!isAnyButtonPressed())
      {
        lcd()->sendBuffer();
      }
    }
    currentIndex += 2;
    lx = x;
    ly = y;
  }
}

/*
https://github.com/olikraus/u8glib/issues/337
d parameters describes how many points in the line are plotted/missed.
So if "d" equals:
0 - ordinary solid line
1 - dotted line (odd points are printed)
2+ - dashed line ("d" points drawn then "d" points omitted)
-d - dash size is the same as for "d", but "invisible" pointsare first
*/
void drawDashedLine(u8g2_uint_t x1, u8g2_uint_t y1, u8g2_uint_t x2, u8g2_uint_t y2, u8g2_int_t d)
{
  u8g2_uint_t tmp;
  u8g2_uint_t x, y;
  u8g2_uint_t dx, dy;
  u8g2_int_t err;
  u8g2_int_t ystep;

  uint8_t swapxy = 0;

  /* no intersection check at the moment, should be added... */

  if (x1 > x2)
    dx = x1 - x2;
  else
    dx = x2 - x1;
  if (y1 > y2)
    dy = y1 - y2;
  else
    dy = y2 - y1;

  if (dy > dx)
  {
    swapxy = 1;
    tmp = dx;
    dx = dy;
    dy = tmp;
    tmp = x1;
    x1 = y1;
    y1 = tmp;
    tmp = x2;
    x2 = y2;
    y2 = tmp;
  }
  if (x1 > x2)
  {
    tmp = x1;
    x1 = x2;
    x2 = tmp;
    tmp = y1;
    y1 = y2;
    y2 = tmp;
  }
  err = dx >> 1;
  if (y2 > y1)
    ystep = 1;
  else
    ystep = -1;
  y = y1;

#ifndef U8G2_16BIT
  if (x2 == 255)
    x2--;
#else
  if (x2 == 0xffff)
    x2--;
#endif

  for (x = x1; x <= x2; x++)
  {
    if (swapxy == 0)
    {
      if (d == 0)
      {
        /* solid line */
        lcd()->drawPixel(x, y);
      }
      else if (d == 1)
      {
        /* dotted line */
        if (x % 2 == 0)
          lcd()->drawPixel(x, y);
      }
      else if (d > 1)
      {
        /* dashed line */
        if ((x / d) % 2 == 0)
          lcd()->drawPixel(x, y);
      }
      else if (d < 0)
      {
        /* dashed line inverted */
        if ((x / -d) % 2 != 0)
          lcd()->drawPixel(x, y);
      }
    }
    else
    {
      if (d == 0)
      {
        /* solid line */
        lcd()->drawPixel(y, x);
      }
      else if (d == 1)
      {
        /* dotted line */
        if (x % 2 == 0)
          lcd()->drawPixel(y, x);
      }
      else if (d > 1)
      {
        /* dashed line */
        if ((x / d) % 2 == 0)
          lcd()->drawPixel(y, x);
      }
      else if (d < 0)
      {
        /* dashed line inverted */
        if ((x / -d) % 2 != 0)
          lcd()->drawPixel(y, x);
      }
    }
    err -= (uint8_t)dy;
    if (err < 0)
    {
      y += (u8g2_uint_t)ystep;
      err += (u8g2_uint_t)dx;
    }
  }
}

#endif