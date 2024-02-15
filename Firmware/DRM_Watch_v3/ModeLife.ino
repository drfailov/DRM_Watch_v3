const int w = 400;
const int h = 240;
bool* maps; //min: 1073340148   max: 1073646548

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
  enableAutoReturn = true;
  enableAutoSleep = false; 
  autoReturnTime = autoReturnDefaultTime;
  autoSleepTime = autoSleepDefaultTime;

  if(maps == nullptr)
    maps = new bool[w*h];
  for(int i=0; i<w*h; i++){
    maps[i] = rand()%2==1;
  }
}
bool get(int x, int y){
  if(x<0 || y<0 || x>=w || y>=h) return false;
  return maps[w*y+x];
}
void set(int x, int y, bool val){
  maps[w*y+x] = val;
}

void ModeLifeLoop(){ 
  //lcd()->setColorIndex(white);
  //lcd()->drawBox(0, 0, 400, 240);
  for(int y=0; y<h; y++){
    for(int x=0; x<w; x++){
      int neighbors = 
        get(x+1,y+1)+ get(x  ,y+1)+  get(x-1,y+1)+
        get(x+1,y-1)+ get(x  ,y-1)+  get(x-1,y-1)+
        get(x+1,y  )+ get(x-1,y  );
      bool curr = get(x,y);
      bool value = curr;
      if(!curr && neighbors==3) curr = true;
      else if(curr && (neighbors <2 || neighbors > 3)) curr = false;
      set(x,y,curr);
      lcd()->setColorIndex(value?black:white);
      lcd()->drawPixel(x,y);
    }
  }
  

  lcd()->sendBuffer();
  Serial.println((int)maps);
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



