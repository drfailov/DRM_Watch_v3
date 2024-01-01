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
  //modeButtonUpLong = modeWatchfaceButtonUp;
  //modeButtonCenterLong = modeWatchfaceButtonUp;
  //modeButtonDownLong = modeWatchfaceButtonUp;
  questionModeText1 = _text1;
  questionModeText2 = _text2;
  questionModeOnYes = _onYes;
  questionModeOnNo = _onNo;
}

void questionModeLoop(){
  draw_ic24_check2(lx(), ly1(), black);
  draw_ic24_empty(lx(), ly2(), black);
  draw_ic24_cancel(lx(), ly3(), black);
  drawQuestion(questionModeText1, questionModeText2);

  
  if(sinceLastAction() > autoReturnTime) //auto go to watchface
    setModeWatchface();
}


void questionModeOnButtonUp(){
  if(questionModeOnYes != 0)
    questionModeOnYes();
}

void questionModeOnButtonDown(){
  if(questionModeOnNo != 0)
    questionModeOnNo();
}