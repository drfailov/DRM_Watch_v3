int modeAboutCounter = 0;
void modeAboutSetup(){
  clearScreenAnimation();
  Serial.println(F("Set mode: About"));
  modeSetup = modeAboutSetup;
  modeLoop = modeAboutLoop;
  modeButtonUp = modeAboutButton;
  modeButtonCenter = modeAboutButton;
  modeButtonDown = modeAboutButton;
  modeButtonUpLong = 0;
  modeButtonCenterLong = 0;
  modeButtonDownLong = 0;
  registerAction();
  enableAutoReturn = true;
  enableAutoSleep = false; 
  autoReturnTime = autoReturnDefaultTime;
  autoSleepTime = autoSleepDefaultTime;
  modeAboutCounter = 0;
}

void modeAboutLoop(){
  lcd()->setColorIndex(white);
  lcd()->drawBox(0, 0, 400, 240);
  lcd()->setFont(u8g2_font_10x20_t_cyrillic);  //ok
  lcd()->setColorIndex(black);
  lcd()->setCursor(5, 17);  lcd()->print("Made by Dr.Failov");
  drawStatusbar(395, 1, true);
  lcd()->setColorIndex(black);
  lcd()->setCursor(5, 234);  lcd()->print(version);
  lcd()->setCursor(135, 234);  lcd()->print("Made in Ukraine");
  lcd()->setCursor(355, 234);  lcd()->print("2024");

  if(modeAboutCounter == 0){
    displayDrawVector(getPathZubat(), 40, 60, 3.0, 3, 3, black);
    displayDrawVector(getPathDrmWatch(), 190, 60, 3.0, 2, 3, black);
  }
  else if(modeAboutCounter < 10){
    displayDrawVector(getPathZubat(), 40, 60, 3.0, 3, 0, black);
    displayDrawVector(getPathDrmWatch(), 190, 60, 3.0, 2, 0, black);
  }
  else if(modeAboutCounter%2==0){
    displayDrawVector(getPathZubat(), 40, 60, 3.0, 3, 0, black);
    displayDrawVector(getPathDrmWatch(), 190, 60, 3.0, 2, 0, black);
    displayDrawVector(getPathDrmWatch(), 190, 60, 3.0, 2, 5, white);
  }
  else{
    displayDrawVector(getPathZubat(), 40, 60, 3.0, 3, 0, black);
    displayDrawVector(getPathDrmWatch(), 190, 60, 3.0, 2, 3, black);
    lcd()->sendBuffer();
    for(unsigned long started = millis(); millis()-started<300; ) if(isAnyButtonPressed()) break;
  }
  lcd()->sendBuffer();
  modeAboutCounter++;
}

void modeAboutButton(){
  setModeMainMenu();
}
