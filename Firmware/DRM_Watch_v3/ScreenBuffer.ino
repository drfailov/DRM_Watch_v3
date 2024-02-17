byte* screenBuffer; //min: 1073340148   max: 1073646548
void initScreenBuffer(){
  if(screenBuffer == nullptr)
    screenBuffer = new byte[lcd_w*lcd_h];
}
void randScreenBuffer(){
  initScreenBuffer();
  for(int i=0; i<lcd_w*lcd_h; i++)
    screenBuffer[i] = rand()%2==1;
}
void zeroScreenBuffer(){
  initScreenBuffer();
  for(int i=0; i<lcd_w*lcd_h; i++)
    screenBuffer[i] = 0;
}
bool getScreenBuffer(int x, int y){
  //initScreenBuffer();
  if(x<0 || y<0 || x>=lcd_w || y>=lcd_h) return false;
  return screenBuffer[lcd_w*y+x];
}
void setScreenBuffer(int x, int y, bool val){
  //initScreenBuffer();    //yes: 149ms   no: 110ms
  screenBuffer[lcd_w*y+x] = val;
}
void drawScreenBuffer(){
  initScreenBuffer();
  for(int y=0; y<lcd_h; y++){
    for(int x=0; x<lcd_w; x++){
      lcd()->setColorIndex(getScreenBuffer(x,y)?black:white);
      lcd()->drawPixel(x,y);
    }
  }
}