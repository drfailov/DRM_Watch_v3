struct Key {
    int row;
    int col;
    int width = 0;
    char letter;
    char letterCaps;
    char letterSym;
    Drawable drawIcon;
    Drawable drawIconCaps;
    Drawable drawIconSym;
    Runnable onClick;

    Key(int _row, int _col, int _width, char _letter, char _letterCaps, char _letterSym, Drawable _drawIcon, Drawable _drawIconCaps, Drawable _drawIconSym, Runnable _onClick): 
    row(_row), col(_col), width(_width), letter(_letter), letterCaps(_letterCaps), letterSym(_letterSym), drawIcon(_drawIcon), drawIconCaps(_drawIconCaps), drawIconSym(_drawIconSym), onClick(_onClick) {}
  };

const int KEYBOARD_MODE_NORMAL = 0;
const int KEYBOARD_MODE_CAPS = 1;
const int KEYBOARD_MODE_SYM = 2;

int keyboardMode = KEYBOARD_MODE_NORMAL;
String text = "";
int selectedRow = 0;
int selectedCol = 0;
int keyDefaultWidth = 28;

Key keys[] = {
  {/*row*/0, /*col*/0, /*width*/keyDefaultWidth, /*letter*/'1', /*letterCaps*/'1', /*letterSym*/'1', /*drawable*/0, /*drawableCaps*/0, /*drawableSym*/0, /*onClick*/addLetter},
  {/*row*/0, /*col*/1, /*width*/keyDefaultWidth, /*letter*/'2', /*letterCaps*/'2', /*letterSym*/'2', /*drawable*/0, /*drawableCaps*/0, /*drawableSym*/0, /*onClick*/addLetter},
  {/*row*/0, /*col*/2, /*width*/keyDefaultWidth, /*letter*/'3', /*letterCaps*/'3', /*letterSym*/'3', /*drawable*/0, /*drawableCaps*/0, /*drawableSym*/0, /*onClick*/addLetter},

  {/*row*/1, /*col*/0, /*width*/keyDefaultWidth, /*letter*/'q', /*letterCaps*/'Q', /*letterSym*/'%', /*drawable*/0, /*drawableCaps*/0, /*drawableSym*/0, /*onClick*/addLetter},
  {/*row*/1, /*col*/1, /*width*/keyDefaultWidth, /*letter*/'w', /*letterCaps*/'W', /*letterSym*/'\\',/*drawable*/0, /*drawableCaps*/0, /*drawableSym*/0, /*onClick*/addLetter},
  {/*row*/1, /*col*/2, /*width*/keyDefaultWidth, /*letter*/'e', /*letterCaps*/'E', /*letterSym*/'|', /*drawable*/0, /*drawableCaps*/0, /*drawableSym*/0, /*onClick*/addLetter},
  {/*row*/1, /*col*/3, /*width*/keyDefaultWidth, /*letter*/'r', /*letterCaps*/'R', /*letterSym*/'=', /*drawable*/0, /*drawableCaps*/0, /*drawableSym*/0, /*onClick*/addLetter},
  {/*row*/1, /*col*/4, /*width*/keyDefaultWidth, /*letter*/'t', /*letterCaps*/'T', /*letterSym*/'[', /*drawable*/0, /*drawableCaps*/0, /*drawableSym*/0, /*onClick*/addLetter},
  {/*row*/1, /*col*/5, /*width*/keyDefaultWidth, /*letter*/'y', /*letterCaps*/'Y', /*letterSym*/']', /*drawable*/0, /*drawableCaps*/0, /*drawableSym*/0, /*onClick*/addLetter},
  {/*row*/1, /*col*/6, /*width*/keyDefaultWidth, /*letter*/'u', /*letterCaps*/'U', /*letterSym*/'<', /*drawable*/0, /*drawableCaps*/0, /*drawableSym*/0, /*onClick*/addLetter},
  {/*row*/1, /*col*/7, /*width*/keyDefaultWidth, /*letter*/'i', /*letterCaps*/'I', /*letterSym*/'>', /*drawable*/0, /*drawableCaps*/0, /*drawableSym*/0, /*onClick*/addLetter},
  {/*row*/1, /*col*/8, /*width*/keyDefaultWidth, /*letter*/'o', /*letterCaps*/'O', /*letterSym*/'{', /*drawable*/0, /*drawableCaps*/0, /*drawableSym*/0, /*onClick*/addLetter},
  {/*row*/1, /*col*/9, /*width*/keyDefaultWidth, /*letter*/'p', /*letterCaps*/'P', /*letterSym*/'}', /*drawable*/0, /*drawableCaps*/0, /*drawableSym*/0, /*onClick*/addLetter},

  
  {/*row*/2, /*col*/0, /*width*/keyDefaultWidth, /*letter*/'a', /*letterCaps*/'A', /*letterSym*/'@', /*drawable*/0, /*drawableCaps*/0, /*drawableSym*/0, /*onClick*/addLetter},
  {/*row*/2, /*col*/1, /*width*/keyDefaultWidth, /*letter*/'s', /*letterCaps*/'S', /*letterSym*/'#', /*drawable*/0, /*drawableCaps*/0, /*drawableSym*/0, /*onClick*/addLetter},
  {/*row*/2, /*col*/2, /*width*/keyDefaultWidth, /*letter*/'d', /*letterCaps*/'D', /*letterSym*/'$', /*drawable*/0, /*drawableCaps*/0, /*drawableSym*/0, /*onClick*/addLetter},


  {/*row*/3, /*col*/0, /*width*/36, /*letter*/0, /*letterCaps*/0, /*letterSym*/0, /*drawable*/draw_ic16_shift_normal, /*drawableCaps*/draw_ic16_shift_cap, /*drawableSym*/draw_ic16_shift_sym, /*onClick*/shift},
  {/*row*/3, /*col*/1, /*width*/36, /*letter*/0, /*letterCaps*/0, /*letterSym*/0, /*drawable*/draw_ic16_backspace, /*drawableCaps*/draw_ic16_backspace, /*drawableSym*/draw_ic16_backspace, /*onClick*/backspace},
  
  {/*row*/4, /*col*/1, /*width*/36, /*letter*/0, /*letterCaps*/0, /*letterSym*/0, /*drawable*/draw_ic16_check, /*drawableCaps*/draw_ic16_check, /*drawableSym*/draw_ic16_check, /*onClick*/setModeMainMenu},
  
  };


void setModeKeyboard(){
  Serial.println(F("Set mode: Keyboard"));
  modeSetup = setModeKeyboard;
  modeLoop = modeKeyboardLoop;
  modeButtonUp = modeKeyboardButtonUp;
  modeButtonCenter = modeKeyboardButtonCenter;
  modeButtonDown = modeKeyboardButtonDown;
  //modeButtonUpLong = modeWatchfaceButtonUp;
  //modeButtonCenterLong = modeWatchfaceButtonUp;
  //modeButtonDownLong = modeWatchfaceButtonUp;
  text = "";
  selectedRow = 0;
  selectedCol = 0;
}


void modeKeyboardLoop(){
  
  lcd()->setColorIndex(white);
  lcd()->drawBox(0, 0, 400, 240);

  lcd()->setFont(u8g2_font_10x20_t_cyrillic);  //ok
  lcd()->setCursor(10, 20);
  lcd()->setColorIndex(black);
  lcd()->print(text);


  int lastRow = -1;
  int x = 0;
  for(int i=0; i<cntKeys(); i++){
    Key key = keys[i];
    if(lastRow != key.row){
      x=0;
      if(key.row == 0) x = 10;
      if(key.row == 1) x = 15;
      if(key.row == 2) x = 20;
      if(key.row == 3) x = 25;
      if(key.row == 4) x = 30;
    }
    x = drawKey(i, x);
    lastRow = key.row;
  }

  
  drawBattery(339, 1);
  lcd()->setColorIndex(black);
  lcd()->drawLine(369, 0, 369, 260);
  lcd()->drawLine(370, 0, 370, 260);
  draw_ic24_arrow_right(lx(), ly1(), black);
  draw_ic24_check2(lx(), ly2(), black);
  draw_ic24_arrow_down(lx(), ly3(), black);
  
  lcd()->sendBuffer();
  if(sinceLastAction() > autoReturnTime) //auto go to watchface
    setModeWatchface();
}


void modeKeyboardButtonUp(){
  selectedCol++;
  if(selectedRow == 0 && selectedCol>9)
    selectedCol = 0;
  if(selectedRow == 1 && selectedCol>10)
    selectedCol = 0;
  if(selectedRow == 2 && selectedCol>10)
    selectedCol = 0;
}

void modeKeyboardButtonCenter(){
  for(int i=0; i<cntKeys(); i++)
    if(keys[i].col == selectedCol && keys[i].row == selectedRow && keys[i].onClick != 0)
      keys[i].onClick();
  //setModeMainMenu();
}

void modeKeyboardButtonDown(){
  selectedRow++;
  if(selectedRow > 4)
    selectedRow=0;
}

int cntKeys(){
  return sizeof(keys)/sizeof(keys[0]);
}

int drawKey(int index, int startX){ //return X of key end
  Key key = keys[index];

  bool selected = selectedRow == key.row && selectedCol == key.col;
  const int yOffset = 60;
  const int height=30;
  const int marginX = 5;
  const int marginY = 5;
  int x = startX + marginX;
  int y = yOffset + (height+marginY) * key.row;
  
  lcd()->setColorIndex(black);
  if(selected)
    lcd()->drawBox(/*x*/x, /*y*/y, /*w*/key.width, /*h*/height);
  else{
    lcd()->drawFrame(/*x*/x, /*y*/y, /*w*/key.width, /*h*/height);
    lcd()->drawFrame(/*x*/x+1, /*y*/y+1, /*w*/key.width-2, /*h*/height-2);
  }
  if(keyboardMode == KEYBOARD_MODE_NORMAL && key.drawIcon != 0)
    key.drawIcon(x+(key.width/2)-8, y+8, selected?white:black);
  if(keyboardMode == KEYBOARD_MODE_CAPS && key.drawIconCaps != 0)
    key.drawIconCaps(x+(key.width/2)-8, y+8, selected?white:black);
  if(keyboardMode == KEYBOARD_MODE_SYM && key.drawIconSym != 0)
    key.drawIconSym(x+(key.width/2)-8, y+8, selected?white:black);

  lcd()->setFont(u8g2_font_10x20_t_cyrillic);  //ok
  lcd()->setCursor(x+(key.width/2)-5, y+19);
  lcd()->setColorIndex(selected?white:black);
  if(keyboardMode == KEYBOARD_MODE_NORMAL && key.letter != 0)
    lcd()->print(key.letter);
  if(keyboardMode == KEYBOARD_MODE_CAPS && key.letterCaps != 0)
    lcd()->print(key.letterCaps);
  if(keyboardMode == KEYBOARD_MODE_SYM && key.letterSym != 0)
    lcd()->print(key.letterSym);
  return x+key.width;
}


void addLetter(){
  for(int i=0; i<cntKeys(); i++){
      if(keys[i].col == selectedCol && keys[i].row == selectedRow){
        
        if(keyboardMode == KEYBOARD_MODE_NORMAL && keys[i].letter != 0)
          text = text+keys[i].letter;
        if(keyboardMode == KEYBOARD_MODE_CAPS && keys[i].letterCaps != 0)
          text = text+keys[i].letterCaps;
        if(keyboardMode == KEYBOARD_MODE_SYM && keys[i].letterSym != 0)
          text = text+keys[i].letterSym;
        
      }
  }
}

void backspace(){
  if(text.length() > 0)
    text.remove(text.length()-1);
}

void shift(){
  if(keyboardMode == KEYBOARD_MODE_NORMAL)
    keyboardMode = KEYBOARD_MODE_CAPS;
  else if(keyboardMode == KEYBOARD_MODE_CAPS)
    keyboardMode = KEYBOARD_MODE_SYM;
  else if(keyboardMode == KEYBOARD_MODE_SYM)
    keyboardMode = KEYBOARD_MODE_NORMAL;
}


//----------------------//----------------------//----------------------//----------------------//----------------------//----------------------//----------------------
void letter(char w){};