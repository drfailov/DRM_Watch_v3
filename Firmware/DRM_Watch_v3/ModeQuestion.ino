Runnable questionModeOnYes = 0;
Runnable questionModeOnNo = 0;
String questionModeText1 = "";
String questionModeText2 = "";

void questionModeSet(String _text1, String _text2, Runnable _onYes, Runnable _onNo){
  Serial.println(F("Set mode: Question"));
  modeSetup = doNothing;
  modeLoop = questionModeLoop;
  modeButtonUp = questionModeOnButtonUp;
  modeButtonCenter = doNothing;
  modeButtonDown = questionModeOnButtonDown;
  modeButtonUpLong = 0;
  modeButtonCenterLong = 0;
  modeButtonDownLong = 0;
  registerAction();
  enableAutoReturn = true;
  enableAutoSleep = false; 
  autoReturnTime = autoReturnDefaultTime;
  autoSleepTime = autoSleepDefaultTime;

  questionModeText1 = _text1;
  questionModeText2 = _text2;
  questionModeOnYes = _onYes;
  questionModeOnNo = _onNo;
}

void questionModeLoop(){
  draw_ic16_check(lx(), ly1(), black);
  draw_ic16_empty(lx(), ly2(), black);
  draw_ic16_cancel(lx(), ly3(), black);
  drawQuestion(questionModeText1, questionModeText2);

}


void questionModeOnButtonUp(){
  if(questionModeOnYes != 0)
    questionModeOnYes();
}

void questionModeOnButtonDown(){
  if(questionModeOnNo != 0)
    questionModeOnNo();
}