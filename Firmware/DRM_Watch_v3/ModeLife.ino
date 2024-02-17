void setModeLife(){
  Serial.println(F("Set mode: Life"));
  modeSetup = setModeLife;
  modeLoop = ModeLifeLoop;
  modeButtonUp = ModeLifeButtonUp;
  modeButtonCenter = ModeLifeButtonCenter;
  modeButtonDown = ModeLifeButtonDown;
  modeButtonUpLong = 0;
  modeButtonCenterLong = 0;
  modeButtonDownLong = 0;
  registerAction();
  enableAutoReturn = false;
  enableAutoSleep = false; 
  autoReturnTime = autoReturnDefaultTime;
  autoSleepTime = autoSleepDefaultTime;
  randScreenBuffer();
}

void ModeLifeLoop(){ 
  drawScreenBuffer();
  lcd()->sendBuffer();
  unsigned long millisStarted = millis();
  lifeStep();
  unsigned long millisEnd = millis();
  Serial.print("Life step: "); Serial.print(millisEnd-millisStarted); Serial.println("ms.");
}

void ModeLifeButtonUp(){
  setModeAppsMenu();
}

void ModeLifeButtonCenter(){
  setModeAppsMenu();
}

void ModeLifeButtonDown(){
  setModeAppsMenu();
}



void lifeStep(){
  bool lastCol [lcd_h];
  bool currCol [lcd_h];
  for(int y = 0; y < lcd_h; y++)
      currCol[y] = false;
  
  for(int x = 0; x < lcd_w; x++){
    for(int y = 0; y < lcd_h; y++)
      lastCol[y] = currCol[y];
    for(int y = 0; y < lcd_h; y++)
      currCol[y] = getScreenBuffer(x,y);
      
    for(int y = 0; y < lcd_h; y++){
      //Serial.print(x);Serial.print(", ");Serial.println(y); delay(1);
      bool topleft = y==0?false:lastCol[y-1];
      bool top = y==0?false:currCol[y-1];
      bool topright = y==0?false:getScreenBuffer(x + 1, y-1);
      
      bool left = lastCol[y];
      bool current = currCol[y];
      bool right = x>=lcd_w-1?false:getScreenBuffer(x + 1, y);
      
      bool bottomleft = y>=lcd_h-1?false:lastCol[y+1];
      bool bottom = y>=lcd_h-1?false:currCol[y+1];
      bool bottomright = y>=lcd_h-1?false:(x>=lcd_w-1?false:getScreenBuffer(x+1, y+1)); //============todo

      int neighbours = topleft+top+topright+left+right+bottomleft+bottom+bottomright;
      
      bool c;
      if(current)
        c = (neighbours == 2 || neighbours == 3);
      else
        c = neighbours == 3;
      setScreenBuffer(x, y, c);
    }
  }
}



