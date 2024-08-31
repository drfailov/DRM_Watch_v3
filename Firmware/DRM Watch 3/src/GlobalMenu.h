#ifndef MODEMAINMENU_H
#define MODEMAINMENU_H

/*PROTOTYPES*/
void globalMenuButtonUp();
void globalMenuButtonDown();
void drawMenuItem(byte index, Drawable drawIcon, const char* name, bool animate, int x, int y, int width, int height);
void drawMenuItem(byte index, Drawable drawIcon, const char* name, bool animate, int x, int y);
void drawMenuItem(byte index, Drawable drawIcon, const char* name, bool animate);
void drawTextFrame(byte index, const char* text, const char* name, bool editMode, bool animate, int x, int y, int width);
void drawListItem(byte index, Drawable drawIcon, const char* name, const char* description, bool animate);
void drawListCheckbox(byte index, Drawable drawIcon, const char* name, const char* description, bool value, bool animate);
void drawListValue(byte index, Drawable drawIcon, const char* name, const char* description, int value, bool animate);


#include "Global.h"
#include "AutoSleep.h"
#include "Button.h"
#include "SettingsMenu/ModeMenuSettings.h"
#include "ModeMenuApps.h"
#include "ModeAbout.h"

const int itemBack=0;


void globalMenuButtonUp(){
  selected--;
  if(selected<0)
    selected=items-1;
}


void globalMenuButtonDown(){
  selected++;
  if(selected>=items)
    selected=0;
}


void drawMenuItem(byte index, Drawable drawIcon, const char* name, bool animate){
  drawMenuItem(index, drawIcon, name, animate, -1, -1, 75, 50);
}
void drawMenuItem(byte index, Drawable drawIcon, const char* name, bool animate, int x, int y){
  drawMenuItem(index, drawIcon, name, animate, x, y, 75, 50);
}

void drawMenuItem(byte index, Drawable drawIcon, const char* name, bool animate, int x, int y, int width, int height){
  if(x == -1 || y == -1){
    int lines = 3;
    int cols = 4;
    if(selected/(lines*cols) != index/(lines*cols)) return;
    const int xOffset = 15;
    const int yOffset = 40;
    const int margin = 8;
    x = xOffset + 6 + (width+margin) * (index%cols);
    y = yOffset + (height+margin) * ((index%(lines*cols))/cols);
  }  
  
  if(selected == index){
    lcd()->setColorIndex(black);
    lcd()->drawRBox(/*x*/x, /*y*/y, /*w*/width, /*h*/height, roundness);
  }
  else{
    lcd()->setColorIndex(black);
    lcd()->drawRBox(/*x*/x, /*y*/y, /*w*/width, /*h*/height, roundness);
    lcd()->setColorIndex(white);
    lcd()->drawRBox(/*x*/x+frame, /*y*/y+frame, /*w*/width-frame*2, /*h*/height-frame*2, roundness);
  }
  int iconsize = 24;
  int iconx = x+(width-iconsize)/2;
  int icony = y+(height-iconsize)/2;
  drawIcon(iconx, icony, selected == index?white:black);
  if(selected == index){
    lcd()->setCursor(8, 232);
    lcd()->setColorIndex(black);
    lcd()->setFont(u8g2_font_10x20_t_cyrillic);  //ok
    lcd()->print(selected+1);
    lcd()->print("/");
    lcd()->print(items);
    lcd()->print(": ");
    lcd()->print(name);
  }
  if(animate)
    lcd()->sendBuffer();
}
void drawNumberFrame(byte index, int number, const char* name, bool editMode, bool animate, int x, int y, int width){
  if(number > -10 && number < 100)
   sprintf(buffer, "%02d", number);
  else
    itoa(number,buffer,10); //(integer, yourBuffer, base)

  drawTextFrame(index, buffer, name, editMode, animate, x, y, width);
}

void drawTextFrame(byte index, const char* text, const char* name, bool editMode, bool animate, int x, int y, int width){
  const int height=42;
  int textMargin = 4;
  lcd()->setFont(u8g2_font_inr24_t_cyrillic);  //ok
  int textwidth = lcd()->getUTF8Width(text);
  if(textwidth > width*0.7f)
    lcd()->setFont(u8g2_font_10x20_t_cyrillic);  //ok
  textwidth = lcd()->getUTF8Width(text);  
  int textHeight = lcd() -> getAscent();
  int textOffsetX = (width - textwidth) / 2;
  int textOffsetY = textHeight + (height - textHeight) / 2;
  int textX = x + textOffsetX;
  int textY = y + textOffsetY;

  if(selected == index){
    lcd()->setColorIndex(black);
    lcd()->drawRBox(/*x*/x, /*y*/y, /*w*/width, /*h*/height, roundness);
    //lcd()->drawBox(/*x*/x, /*y*/y, /*w*/width, /*h*/height);
    if(editMode) {
      lcd()->setColorIndex(white);
      lcd()->drawRBox(/*x*/textX-textMargin, /*y*/textY-textMargin-textHeight, /*w*/textwidth + textMargin*2, /*h*/textHeight + textMargin*2, roundness);
      //lcd()->drawBox(/*x*/textX-textMargin, /*y*/textY-textMargin-textHeight, /*w*/textwidth + textMargin*2, /*h*/textHeight + textMargin*2);
    }
  }
  else{
    lcd()->setColorIndex(black);
    lcd()->drawRBox(/*x*/x, /*y*/y, /*w*/width, /*h*/height, roundness);
    lcd()->setColorIndex(white);
    lcd()->drawRBox(/*x*/x+frame, /*y*/y+frame, /*w*/width-frame*2, /*h*/height-frame*2, roundness);
  }
  lcd()->setColorIndex(selected == index?white:black);
  if(editMode) lcd()->setColorIndex(black);
  
  lcd()->setCursor(textX, textY);
  lcd()->print(text);

  if(selected == index){
    lcd()->setCursor(8, 232);
    lcd()->setColorIndex(black);
    lcd()->setFont(u8g2_font_10x20_t_cyrillic);  //ok
    lcd()->print(name);
  }
  if(animate)
    lcd()->sendBuffer();
}

void drawListItem(byte index, Drawable drawIcon, const char* name, const char* description, bool animate){
  int lines = 4;
  if(selected/(lines) != index/(lines)) return;
  const int xOffset = 10;
  const int yOffset = 28;
  const int width=345;
  const int height=42;
  const int margin = 6;
  int x = xOffset;
  int y = yOffset + (height+margin) * ((index%(lines)));
  
  if(selected == index){
    lcd()->setColorIndex(black);
    lcd()->drawRBox(/*x*/x, /*y*/y, /*w*/width, /*h*/height, roundness);
    //lcd()->drawBox(/*x*/x, /*y*/y, /*w*/width, /*h*/height);
  }
  else{
    lcd()->setColorIndex(black);
    lcd()->drawRBox(/*x*/x, /*y*/y, /*w*/width, /*h*/height, roundness);
    lcd()->setColorIndex(white);
    lcd()->drawRBox(/*x*/x+frame, /*y*/y+frame, /*w*/width-frame*2, /*h*/height-frame*2, roundness);
  }
  drawIcon(x + 9, y+9, selected == index?white:black);
  
  lcd()->setFont(u8g2_font_10x20_t_cyrillic);  //ok
  lcd()->setColorIndex(selected == index?white:black);
  lcd()->setCursor(x+45, y+20); lcd()->print(name);
  lcd()->setFont(u8g2_font_unifont_t_cyrillic);  //ok
  lcd()->setCursor(x+45, y+35); lcd()->print(description);
  if(selected == index){
    lcd()->setFont(u8g2_font_10x20_t_cyrillic);  //ok
    lcd()->setColorIndex(black);
    lcd()->setCursor(5, 235); 
    lcd()->print(selected+1);lcd()->print("/");lcd()->print(items); 
  }
  if(animate)
    lcd()->sendBuffer();
}

void drawListCheckbox(byte index, Drawable drawIcon, const char* name, const char* description, bool value, bool animate){
  int lines = 4;
  if(selected/(lines) != index/(lines)) return;
  const int xOffset = 10;
  const int yOffset = 28;
  const int width=345;
  const int height=42;
  const int margin = 6;
  int x = xOffset;
  int y = yOffset + (height+margin) * ((index%(lines)));
  
  if(selected == index){
    lcd()->setColorIndex(black);
    lcd()->drawRBox(/*x*/x, /*y*/y, /*w*/width, /*h*/height, roundness);
    //lcd()->drawBox(/*x*/x, /*y*/y, /*w*/width, /*h*/height);
  }
  else{
    lcd()->setColorIndex(black);
    lcd()->drawRBox(/*x*/x, /*y*/y, /*w*/width, /*h*/height, roundness);
    lcd()->setColorIndex(white);
    lcd()->drawRBox(/*x*/x+frame, /*y*/y+frame, /*w*/width-frame*2, /*h*/height-frame*2, roundness);
  }
  drawIcon(x + 9, y+9, selected == index?white:black);

  //draw_ic24_cancel   draw_ic24_check2
  if(value)
    draw_ic24_check2(x+310, y+9, selected == index?white:black);
  else
    draw_ic24_cancel(x+310, y+9, selected == index?white:black);
  
  lcd()->setFont(u8g2_font_10x20_t_cyrillic);  //ok
  lcd()->setColorIndex(selected == index?white:black);
  lcd()->setCursor(x+45, y+20); lcd()->print(name);
  lcd()->setFont(u8g2_font_unifont_t_cyrillic);  //ok
  lcd()->setCursor(x+45, y+35); lcd()->print(description);
  if(selected == index){
    lcd()->setFont(u8g2_font_10x20_t_cyrillic);  //ok
    lcd()->setColorIndex(black);
    lcd()->setCursor(5, 235); 
    lcd()->print(selected+1);lcd()->print("/");lcd()->print(items); 
  }
  if(animate)
    lcd()->sendBuffer();
}

void drawListValue(byte index, Drawable drawIcon, const char* name, const char* description, int value, bool animate){
  int lines = 4;
  if(selected/(lines) != index/(lines)) return;
  const int xOffset = 10;
  const int yOffset = 28;
  const int width=345;
  const int height=42;
  const int margin = 6;
  int x = xOffset;
  int y = yOffset + (height+margin) * ((index%(lines)));
  
  if(selected == index){
    lcd()->setColorIndex(black);
    lcd()->drawRBox(/*x*/x, /*y*/y, /*w*/width, /*h*/height, roundness);
    //lcd()->drawBox(/*x*/x, /*y*/y, /*w*/width, /*h*/height);
  }
  else{
    lcd()->setColorIndex(black);
    lcd()->drawRBox(/*x*/x, /*y*/y, /*w*/width, /*h*/height, roundness);
    lcd()->setColorIndex(white);
    lcd()->drawRBox(/*x*/x+frame, /*y*/y+frame, /*w*/width-frame*2, /*h*/height-frame*2, roundness);
  }
  drawIcon(x + 9, y+9, selected == index?white:black);
  
  lcd()->setFont(u8g2_font_10x20_t_cyrillic);  //ok
  lcd()->setColorIndex(selected == index?white:black);
  lcd()->setCursor(x+45, y+20); lcd()->print(name);

  lcd()->setCursor(x+318, y+27); lcd()->print(value);


  lcd()->setFont(u8g2_font_unifont_t_cyrillic);  //ok
  lcd()->setCursor(x+45, y+35); lcd()->print(description);
  if(selected == index){
    lcd()->setFont(u8g2_font_10x20_t_cyrillic);  //ok
    lcd()->setColorIndex(black);
    lcd()->setCursor(5, 235); 
    lcd()->print(selected+1);lcd()->print("/");lcd()->print(items); 
  }
  if(animate)
    lcd()->sendBuffer();
}

void drawMenuLegend(){
  lcd()->setColorIndex(black);
  lcd()->drawBox(369, 0, 2, 260);
  draw_ic16_arrow_up(lx(), ly1(), black);
  draw_ic16_check(lx(), ly2(), black);
  draw_ic16_arrow_down(lx(), ly3(), black);
}


#endif