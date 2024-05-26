RTC_DATA_ATTR int antX = 0; // BUFFER COORDINATES!!!
RTC_DATA_ATTR int antY = 0;  // BUFFER COORDINATES!!!
RTC_DATA_ATTR int antDir = 0;  //0 1 2 3

void setModeAnt(){
  clearScreenAnimation();
  Serial.println(F("Set mode: Ant"));
  modeSetup = setModeAnt;
  modeLoop = ModeAntLoop;
  modeButtonUp = ModeAntButtonUp;
  modeButtonCenter = ModeAntButtonCenter;
  modeButtonDown = ModeAntButtonDown;
  modeButtonUpLong = 0;
  modeButtonCenterLong = 0;
  modeButtonDownLong = 0;
  registerAction();
  enableAutoReturn = false;
  enableAutoSleep = false; 
  autoReturnTime = autoReturnDefaultTime;
  autoSleepTime = autoSleepDefaultTime;
  antSpawn();
}

void ModeAntLoop(){ 
  //getAntSpeed   saveAntSpeed

  
  drawScreenBuffer();
  lcd()->sendBuffer();
  int speed = getAntSpeed();
  if(speed < 1) speed = 1;
  if(speed > 9999) speed = 9999;
  for(int i=0; i<speed; i++)
    antStep();
}

void ModeAntButtonUp(){
  int speed = getAntSpeed();
  if(speed < 1) speed = 1;
  if(speed > 9999) speed = 9999;
  speed++;
  Serial.print("Speed="); Serial.println(speed);
  saveAntSpeed(speed);
}

void ModeAntButtonCenter(){
  setModeAppsMenu();
}

void ModeAntButtonDown(){
  int speed = getAntSpeed();
  if(speed < 1) speed = 1;
  if(speed > 9999) speed = 9999;
  speed--;
  Serial.print("Speed="); Serial.println(speed);
  saveAntSpeed(speed);
}


void antSpawn(){
  zeroScreenBuffer();
  antX = BUFF_W/2;
  antY = BUFF_H/2;
  antDir = rand()%4;
}

void antStep(){
  bool cur = getScreenBuffer(antX,antY);
  setScreenBuffer(antX, antY, !cur);

  if(cur) antDir --; 
  else    antDir ++; 
  if(antDir < 0) antDir = 3;
  if(antDir > 3) antDir = 0;

  if(antDir == 0) antY --; //TOP
  if(antDir == 1) antX ++; //RIGHT
  if(antDir == 2) antY ++; //BOTTOM
  if(antDir == 3) antX --; //LEFT
  if(antX < 0) antX = BUFF_W-1;
  if(antX >= BUFF_W) antX = 0;
  if(antY < 0) antY = BUFF_H-1;
  if(antY >= BUFF_H) antY = 0;
}



