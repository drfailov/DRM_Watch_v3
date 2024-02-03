const int itemModeMeowBack=0;
const int itemModeMeowToneUp=1;
const int itemModeMeowToneDown=2;

long lastMeowEnd = 0;
long meowPause = 0;
float meowTone = 1.3f;

void setModeMeow(){
  Serial.println(F("Set mode: Meow"));
  modeSetup = setModeMeow;
  modeLoop = modeMeowLoop;
  modeButtonUp = modeMainMenuButtonUp;
  modeButtonCenter = modeMeowButtonCenter;
  modeButtonDown = modeMainMenuButtonDown;
  modeButtonUpLong = 0;
  modeButtonCenterLong = 0;
  modeButtonDownLong = 0;
  registerAction();
  enableAutoReturn = true;
  enableAutoSleep = false; 
  autoReturnTime = autoReturnDefaultTime;
  autoSleepTime = autoSleepDefaultTime;
  selected = 0;
  items = 3;
  lastMeowEnd = 0;
  meowPause = 0;
}


void modeMeowLoop(){
  lcd()->setColorIndex(white);
  lcd()->drawBox(0, 0, 400, 240);

  lcd()->setFont(u8g2_font_10x20_t_cyrillic);  //ok
  lcd()->setColorIndex(black);
  lcd()->setCursor(5, 18); 
  lcd()->print("Meow");

  lcd()->setCursor(210, 75); 
  lcd()->print("Meow");
  lcd()->setCursor(210, 98); 
  lcd()->print(meowTone);

  drawStatusbar(363, 1, true);  
  drawMenuLegend();

  displayDrawVector(getPathZubat(), 110, 40, 2.0, 2, false, black);

  drawMenuItem(itemModeMeowBack, draw_ic24_back, "Назад", false, 70, 160);
  drawMenuItem(itemModeMeowToneUp, draw_ic24_arrow_up, "Збільшити тон", false, 140, 160);
  drawMenuItem(itemModeMeowToneDown, draw_ic24_arrow_down, "Зменшити тон", false, 210, 160);

  lcd()->sendBuffer();
  if(millis() - lastMeowEnd > meowPause){
    meow();
    meowPause = (300 + (millis()%1000));
    lastMeowEnd = millis();
  }
}

void modeMeowButtonCenter(){
  if(selected == itemModeMeowBack){
    setModeAppsMenu();
  }
  if(selected == itemModeMeowToneUp){
    meowTone *= 1.1f;
  }
  if(selected == itemModeMeowToneDown){
    meowTone /= 1.1f;
  }
  
}

void meow(){
  byte meowLength = 7;
  int meowData[] = {2012, 2544, 2742, 2556, 2416, 2128, 1920, 1874, -1000};
  float meowCoef = 0.4f + ((millis()%50)*0.01f);
  float meowOffset = 700+(millis()%600);
  float meowTime = 5 + (millis()%2);
  float meowValue = meowData[0];
  float meowSpeed = 0;
  meowValue *= meowCoef;
  meowValue += meowOffset;
  //pinMode(pinBuzzer, OUTPUT);
  for(byte i=1; i<meowLength; i++){
    float meowValueNext = meowData[i];
    meowValueNext *= meowCoef;
    meowValueNext += meowOffset;
    meowValueNext += millis()%100;
    for(int j=0; j<7; j++){
      meowSpeed += (meowValueNext-meowValue)*0.1f;
      meowSpeed *= 0.5f;
      meowValue += meowSpeed;
      buzTone(meowValue*meowTone); 
      delay(meowTime);
    }
  }
  buzNoTone();
}

