Runnable questionModeOnYes = 0;
Runnable questionModeOnNo = 0;
String questionModeText = "";

void questionModeSet(String _text, Runnable _onYes, Runnable _onNo){
  Serial.println(F("Set mode: Question"));
  modeSetup = doNothing;
  modeLoop = questionModeLoop;
  modeButtonUp = questionModeOnButtonUp;
  modeButtonCenter = doNothing;
  modeButtonDown = questionModeOnButtonDown;
  //modeButtonUpLong = modeWatchfaceButtonUp;
  //modeButtonCenterLong = modeWatchfaceButtonUp;
  //modeButtonDownLong = modeWatchfaceButtonUp;
  questionModeText = _text;
  questionModeOnYes = _onYes;
  questionModeOnNo = _onNo;
}

void questionModeLoop(){
  draw_ic24_check2(lx(), ly1(), black);
  draw_ic24_empty(lx(), ly2(), black);
  draw_ic24_cancel(lx(), ly3(), black);
  drawMessage(questionModeText);
}


void questionModeOnButtonUp(){
  if(questionModeOnYes != 0)
    questionModeOnYes();
}

void questionModeOnButtonDown(){
  if(questionModeOnNo != 0)
    questionModeOnNo();
}