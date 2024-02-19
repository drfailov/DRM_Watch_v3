RTC_DATA_ATTR byte screenBuffer[BUFF_W*BUFF_H/8]; 

void randScreenBuffer(){
  for(int i=0; i<BUFF_W*BUFF_H/8; i++)
    screenBuffer[i] = rand()%255;
}
void zeroScreenBuffer(){
  for(int i=0; i<BUFF_W*BUFF_H/8; i++)
    screenBuffer[i] = 0;
}
bool getScreenBuffer(int x, int y){
  if(x<0 || y<0 || x>=BUFF_W || y>=BUFF_H) return false;
  int addr = BUFF_W*y+x;
  int cellIndex = addr/8;
  int bitIndex = addr%8;
  byte cell = screenBuffer[cellIndex];
  bool value = bitRead(cell, bitIndex);
  return value;
}
void setScreenBuffer(int x, int y, bool val){
  if(x<0 || y<0 || x>=BUFF_W || y>=BUFF_H) return;
  int addr = BUFF_W*y+x;
  int cellIndex = addr/8;
  int bitIndex = addr%8;
  byte cell = screenBuffer[cellIndex];
  bitWrite(cell, bitIndex, val);
  screenBuffer[cellIndex] = cell;
}
void drawScreenBuffer(){
  for(int y=0; y<BUFF_H; y++){
    for(int x=0; x<BUFF_W; x++){
      lcd()->setColorIndex(getScreenBuffer(x,y)?black:white);
      lcd()->drawBox(x*BUFF_SCALE, y*BUFF_SCALE, BUFF_SCALE, BUFF_SCALE);
    }
  }
}