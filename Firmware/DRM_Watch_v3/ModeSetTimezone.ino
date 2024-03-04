int modeSetTimezoneIndex = 0;
bool modeSetTimezoneEditMode = false;

const int itemmodeSetTimezoneBack=0;
const int itemmodeSetTimezoneHour=1;

void setmodeSetTimezoneMenu(){
  Serial.println(F("Set mode: set timezone"));
  modeSetup = setmodeSetTimezoneMenu;
  modeLoop = modeSetTimezoneMenuLoop;
  modeButtonUp = modeSetTimezoneMenuButtonUp;
  modeButtonCenter = modeSetTimezoneMenuButtonCenter;
  modeButtonDown = modeSetTimezoneMenuButtonDown;
  modeButtonUpLong = 0;
  modeButtonCenterLong = 0;
  modeButtonDownLong = 0;
  registerAction();
  enableAutoReturn = true;
  enableAutoSleep = false; 
  autoReturnTime = autoReturnDefaultTime;
  autoSleepTime = autoSleepDefaultTime;
  selected = 0;
  items = 2;
}


void modeSetTimezoneMenuLoop(){
  lcd()->setColorIndex(white);
  lcd()->drawBox(0, 0, 400, 240);

  lcd()->setFont(u8g2_font_10x20_t_cyrillic);  //ok
  lcd()->setColorIndex(black);
  lcd()->setCursor(5, 18); 
  lcd()->print("Часовий пояс");
  
  drawMenuItem(itemmodeSetTimezoneBack, draw_ic24_back, "Назад", false, 30, 32);
  

  lcd()->setFont(u8g2_font_10x20_t_cyrillic);  //ok
  lcd()->setColorIndex(black);
  lcd()->setCursor(30, 113); 
  lcd()->print("Час");
  //drawNumberFrame(/*index*/itemmodeSetTimezoneHour, /*number*/modeSetTimezoneHourValue, /*name*/"Обрати годину", /*editMode*/modeSetTimezoneEditMode, /*animate*/false, /*x*/68, /*y*/87, /*Width*/82);
  
  long offset = getTimeOffsetSec();
  long hours = offset/(60*60);
  offset -= hours*(60*60);
  long minutes = offset/60;
  String text = "UTC ";
  text += hours>=0?"+":"";
  text += hours;
  text += ":";
  text += minutes;
  drawTextFrame(/*index*/itemmodeSetTimezoneHour, /*text*/text.c_str(), /*name*/"Обрати часовий пояс", /*editMode*/modeSetTimezoneEditMode, /*animate*/false, /*x*/30, /*y*/145, /*width*/308);

  drawStatusbar(363, 1, true);
  drawMenuLegend();
  if(modeSetTimezoneEditMode){
    draw_ic16_plus(lx(), ly1(), black);
    draw_ic16_minus(lx(), ly3(), black);
  }
  lcd()->sendBuffer();
}

void modeSetTimezoneMenuButtonUp(){
  if(!modeSetTimezoneEditMode){
    modeMainMenuButtonUp();
    return;
  }
  //if(selected == itemmodeSetTimezoneHour)  modeSetTimezoneHourValue ++;
}
void modeSetTimezoneMenuButtonDown(){
  if(!modeSetTimezoneEditMode){
    modeMainMenuButtonDown();
    return;
  }
  //if(selected == itemmodeSetTimezoneHour)  modeSetTimezoneHourValue --;
}
void modeSetTimezoneMenuButtonCenter(){
  if(selected == itemmodeSetTimezoneBack){
    setModeSettingsMenu();
    return;
  }
  if(selected==itemmodeSetTimezoneHour){
    modeSetTimezoneEditMode = !modeSetTimezoneEditMode;
    if(!modeSetTimezoneEditMode){
      //save
    }
    return;
  }
}
