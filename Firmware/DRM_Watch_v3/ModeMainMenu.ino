const int itemBack=0;
const int itemApps=1;
const int itemSettings=2;
const int itemAbout=3;
int items = 4;
int selected = 0;

void setModeMainMenu(){
  Serial.println(F("Set mode: Main Menu"));
  modeSetup = setModeTest;
  modeLoop = modeMainMenuLoop;
  modeButtonUp = modeMainMenuButtonUp;
  modeButtonCenter = modeMainMenuButtonCenter;
  modeButtonDown = modeMainMenuButtonDown;
  //modeButtonUpLong = modeWatchfaceButtonUp;
  //modeButtonCenterLong = modeWatchfaceButtonUp;
  //modeButtonDownLong = modeWatchfaceButtonUp;
  selected = 0;
}

void modeMainMenuLoop(){
  lcd()->setColorIndex(white);
  lcd()->drawBox(0, 0, 400, 240);


  
  drawMenuItem(itemBack, draw_ic24_arrow_left, "Назад", false);
  drawMenuItem(itemApps, draw_ic24_apps, "Програми", false);
  drawMenuItem(itemSettings, draw_ic24_settings, "Нашаштування", false);
  drawMenuItem(itemAbout, draw_ic24_about, "Про програму", false);

  lcd()->setColorIndex(black);
  lcd()->drawLine(369, 0, 369, 260);
  lcd()->drawLine(370, 0, 370, 260);
  
  drawTemperature(5, 5);
  drawBattery(339, 1);
  draw_ic24_arrow_up(lx(), ly1(), black);
  draw_ic24_check2(lx(), ly2(), black);
  draw_ic24_arrow_down(lx(), ly3(), black);
  


  lcd()->sendBuffer();
  
  if(sinceLastAction() > autoReturnTime) //auto go to watchface
    setModeWatchface();
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
    playMelody();
    return;
  }
  if(selected == itemSettings){
    setModeTimeSync();
    return;
  }
  
  if(selected == itemAbout){
    setModeTest();
    return;
  }
}

void modeMainMenuButtonDown(){
  selected++;
  if(selected>=items)
    selected=0;
}


void drawMenuItem(byte index, void (*drawIcon)(int x,int y, bool color), const char* name, bool animate){
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