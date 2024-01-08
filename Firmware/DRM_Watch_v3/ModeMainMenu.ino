const int itemBack=0;
const int itemApps=1;
const int itemSettings=2;
const int itemAbout=3;
const int itemDebug=4;

void setModeMainMenu(){
  Serial.println(F("Set mode: Main Menu"));
  modeSetup = setModeTest;
  modeLoop = modeMainMenuLoop;
  modeButtonUp = modeMainMenuButtonUp;
  modeButtonCenter = modeMainMenuButtonCenter;
  modeButtonDown = modeMainMenuButtonDown;
  modeButtonUpLong = 0;
  modeButtonCenterLong = 0;
  modeButtonDownLong = 0;
  registerAction();
  enableAutoReturn = true;
  selected = 0;
  items = 5;
}

void modeMainMenuLoop(){
  lcd()->setColorIndex(white);
  lcd()->drawBox(0, 0, 400, 240);

  lcd()->setFont(u8g2_font_10x20_t_cyrillic);  //ok
  lcd()->setColorIndex(black);
  lcd()->setCursor(5, 18); 
  lcd()->print("Головне меню");
  
  drawMenuItem(itemBack, draw_ic24_arrow_left, "Назад", false);
  drawMenuItem(itemApps, draw_ic24_apps, "Програми", false);
  drawMenuItem(itemSettings, draw_ic24_settings, "Нашаштування", false);
  drawMenuItem(itemAbout, draw_ic24_about, "Про програму", false);
  drawMenuItem(itemDebug, draw_ic24_bug, "Інженерне меню", false);

  
  //drawTemperature(5, 5);
  drawStatusbar(363, 1, true);
  
  drawMenuLegend();


  lcd()->sendBuffer(); 
}

void modeMainMenuButtonUp(){
  selected--;
  if(selected<0)
    selected=items-1;
}

void modeMainMenuButtonCenter(){
  if(selected == itemBack){
    setModeWatchface();
    return;
  }
  if(selected == itemApps){
    setModeAppsMenu();
    return;
  }
  if(selected == itemSettings){
    setModeSettingsMenu();
    return;
  }
  
  if(selected == itemAbout){
    modeAboutSetup();
    return;
  }
  
  if(selected == itemDebug){
    setModeTest();
    return;
  }
}

void modeMainMenuButtonDown(){
  selected++;
  if(selected>=items)
    selected=0;
}


void drawMenuItem(byte index, Drawable drawIcon, const char* name, bool animate){
  int lines = 2;
  int cols = 4;
  if(selected/(lines*cols) != index/(lines*cols)) return;
  const int xOffset = 35;
  const int yOffset = 60;
  const int width=62;
  const int height=42;
  const int margin = 15;
  int x = xOffset + 6 + (width+margin) * (index%cols);
  int y = yOffset + (height+margin) * ((index%(lines*cols))/cols);
  //void(* drawR) (int x, int y, int w, int h) = selected == index?lcd()->drawBox:lcd()->drawFrame;
  //lcd()->drawFrame(u8g2_uint_t x, u8g2_uint_t y, u8g2_uint_t w, u8g2_uint_t h)
  
  lcd()->setColorIndex(black);
  if(selected == index)
    lcd()->drawBox(/*x*/x, /*y*/y, /*w*/width, /*h*/height);
  else{
    lcd()->drawFrame(/*x*/x, /*y*/y, /*w*/width, /*h*/height);
    lcd()->drawFrame(/*x*/x+1, /*y*/y+1, /*w*/width-2, /*h*/height-2);
  }
  drawIcon(x + 19, y+9, selected == index?white:black);
  if(selected == index){
    lcd()->setCursor(8, 232);
    lcd()->setColorIndex(black);
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
  
  lcd()->setColorIndex(black);
  if(selected == index)
    lcd()->drawBox(/*x*/x, /*y*/y, /*w*/width, /*h*/height);
  else{
    lcd()->drawFrame(/*x*/x, /*y*/y, /*w*/width, /*h*/height);
    lcd()->drawFrame(/*x*/x+1, /*y*/y+1, /*w*/width-2, /*h*/height-2);
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
void drawMenuLegend(){
  lcd()->setColorIndex(black);
  lcd()->drawLine(369, 0, 369, 260);
  lcd()->drawLine(370, 0, 370, 260);
  draw_ic16_arrow_up(lx(), ly1(), black);
  draw_ic16_check(lx(), ly2(), black);
  draw_ic16_arrow_down(lx(), ly3(), black);
}