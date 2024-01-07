
void modeAboutSetup(){
//setModeWatchface
  Serial.println(F("Set mode: About"));
  modeSetup = modeAboutSetup;
  modeLoop = modeAboutLoop;
  modeButtonUp = setModeWatchface;
  modeButtonCenter = setModeWatchface;
  modeButtonDown = setModeWatchface;
  modeButtonUpLong = 0;
  modeButtonCenterLong = 0;
  modeButtonDownLong = 0;
}

void modeAboutLoop(){
  lcd()->setColorIndex(white);
  lcd()->drawBox(0, 0, 400, 240);
  lcd()->setFont(u8g2_font_10x20_t_cyrillic);  //ok
  lcd()->setColorIndex(black);
  lcd()->setCursor(5, 17);  lcd()->print("Made by Dr.Failov");
  drawStatusbar(395, 1, true);
  lcd()->setCursor(5, 234);  lcd()->print(version);
  lcd()->setCursor(135, 234);  lcd()->print("Made in Ukraine");
  lcd()->setCursor(355, 234);  lcd()->print("2024");

  displayDrawVector(getPathZubat(), 40, 60, 3.0, 3, 0, black);
  displayDrawVector(getPathDrmWatch(), 190, 60, 3.0, 2, 4, black);
  delay(500);
  displayDrawVector(getPathDrmWatch(), 190, 60, 3.0, 2, 6, white);
  lcd()->sendBuffer();

  
  if(sinceLastAction() > autoReturnTime && !dontSleep) //auto go to watchface
    setModeWatchface();
}
