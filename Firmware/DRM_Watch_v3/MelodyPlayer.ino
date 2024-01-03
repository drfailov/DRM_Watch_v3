const byte groovyBlue[] = {
  125,
  0b10011101, 0b00100100, 0b00100100, 0b00100100, 0b00100100,
  0b10011000, 0b10011011, 0b10011000, 0b10011011, 0b10011101,
  0b00100100, 0b00100100, 0b00100100, 0b00100100, 0b10011000,
  0b10011011, 0b10011000, 0b10011011, 0b01011101, 0b10010001,
  0b01100100, 0b10010001, 0b01100100, 0b10010001, 0b01100100,
  0b10010001, 0b01100100, 0b10010001, 0b01100100, 0b10010001,
  0b01100100, 0b10011000, 0b10011011, 0b10011000, 0b10011011,
  0b01011101, 0b10011101, 0b01100100, 0b10011101, 0b01100100,
  0b10011101, 0b01100100, 0b10011101, 0b01100100, 0b10011101,
  0b01100100, 0b10011101, 0b01100100, 0b10011000, 0b10011011,
  0b10011000, 0b10011011, 0b01011101, 0b10011101, 0b01100000,
  0b10011101, 0b01100010, 0b10011101, 0b01100100, 0b10011101,
  0b01100000, 0b10011101, 0b10100010, 0b10100100, 0b10011101,
  0b01100100, 0b10011000, 0b10011011, 0b10011000, 0b10011011,
  0b01011101, 0b10011101, 0b01100000, 0b10011101, 0b01100010,
  0b10011101, 0b01100100, 0b10011101, 0b01100000, 0b10011101,
  0b10100010, 0b10100100, 0b10011101, 0b01100100, 0b10011000,
  0b10011011, 0b10011000, 0b10011011, 0b01011101, 0b10011101,
  0b01100000, 0b10011101, 0b01100011, 0b10011101, 0b01100010,
  0b10011101, 0b01100001, 0b10011101, 0b01100000, 0b10011101,
  0b01011111, 0b00100100, 0b10011000, 0b10011011, 0b10011000,
  0b10011011, 0b01011101, 0b11111111,
};
const byte* const getMelodyGroovyBlue() {
  return groovyBlue;
}


const byte blue[]  = {
    140,
    0b00001001, 0b00001010, 0b01000111, 0b01001010, 0b01001100, 
    0b01000101, 0b01001001, 0b00001010, 0b01000111, 0b01001010, 
    0b01001110, 0b00001111, 0b01001110, 0b01001100, 0b00001010, 
    0b01000111, 0b01001010, 0b01001100, 0b01000101, 0b01001001, 
    0b00001010, 0b01000111, 0b01001010, 0b01001110, 0b00001111, 
    0b01001110, 0b01001100, 0b00001010, 0b01000111, 0b01001010, 
    0b01001100, 0b01000101, 0b01001001, 0b00001010, 0b01000111, 
    0b01001010, 0b01001110, 0b00001111, 0b01001110, 0b01001100, 
    0b00001010, 0b01000111, 0b01001010, 0b01001001, 0b01000101, 
    0b01000101, 0b00000111, 0b11111111, 
};

const byte* getMelodyBlue() {
  return blue;
}




//Максимальная длина строки:                  |          |
const char modeMenuMelodiesItem1[]  = "Entertainer";
const char modeMenuMelodiesItem2[]  = "Badinerie";
const char modeMenuMelodiesItem3[]  = "Nokia Tune";
const char modeMenuMelodiesItem4[]  = "Mozart";
const char modeMenuMelodiesItem5[]  = "Mario";
const char modeMenuMelodiesItem6[]  = "Groovy Blue";
const char modeMenuMelodiesItem7[]  = "Ukraine";
const char modeMenuMelodiesItem8[]  = "Blue";
const char modeMenuMelodiesItem9[]  = "Was Wollen";


const char*  modeMenuMelodiesItems[]  = {
  modeMenuMelodiesItem1,
  modeMenuMelodiesItem2,
  modeMenuMelodiesItem3,
  modeMenuMelodiesItem4,
  modeMenuMelodiesItem5,
  modeMenuMelodiesItem6,
  modeMenuMelodiesItem7,
  modeMenuMelodiesItem8,
  modeMenuMelodiesItem9
};

byte getMelodyCount(){ return 9;}
const char* getMelodyName(byte index){
  return (modeMenuMelodiesItems[index+1]);
}
const byte* getMelodyByIndex(byte index){
  // if (index == 0) 
    // return getMelodyEntertainer();
  // if (index == 1)  
  //   return getMelodyBadinerie();
  // if (index == 2)  
  //   return getMelodyNokiaTune();
  // if (index == 3) 
  //   return getMelodyMozart();
  // if (index == 4) 
  //   return getMelodyMario();
  if (index == 5) 
    return getMelodyGroovyBlue();
  // if (index == 6) 
  //   return getMelodyUkraine();
  // if (index == 7) 
  //   return getMelodyBlue();  
  // if (index == 8) 
  //   return getMelodyWasWollenWirTrinken();  
  return 0;  
}







bool melodyPlayerLoopMelody = false;

//return true if was played completely or false if interrupted
bool melodyPlayerPlayMelody(const byte* melody) {
  //displayTransition();
  melodyPlayerLoopMelody = false;
  do{
    melodyPlayerDrawScreen();
    //pinMode(pinBuzzer, OUTPUT);
    byte length = melodyPlayerGetLength(melody);
    float tempo = (melody[0]);
    float whole_notes_per_second = tempo / 240.0;
    for (byte i = 1; i < length - 1; i++) {
      byte b = (melody[i]);
      byte duration = 0;
      if (bitRead(b, 7) == 0 && bitRead(b, 6) == 0) duration = 4;
      if (bitRead(b, 7) == 0 && bitRead(b, 6) == 1) duration = 8;
      if (bitRead(b, 7) == 1 && bitRead(b, 6) == 0) duration = 16;
      if (bitRead(b, 7) == 1 && bitRead(b, 6) == 1) duration = 32;
      float timeMs = 1000.0 / (whole_notes_per_second * duration);
      byte noteNumberByte = 0;
      for (byte i = 0; i < 6; i++)
        bitWrite(noteNumberByte, i, bitRead(b, i));
      float noteNumber = noteNumberByte;
      // Note frequency is calculated as (F*2^(n/12)),
      // We can use C2=65.41, or C3=130.81. C2 is a bit shorter.
      float frequency = 0;
      //290 is ok
      if (noteNumber < 36)
        frequency = 250.0 * pow(2.0, (noteNumber / 12.0));
  
      long noteStarted = millis();
      if (frequency != 0){
        //displayBacklightOn();
        buzTone(frequency, timeMs);
      }
      else{
        buzNoTone();
      }
      while(millis() - noteStarted < timeMs);
      buzNoTone();
      //displayBacklightOff();
      delay(13);
      
    //   if (isButtonUpPressed()){
    //     if(isButtonUpHold()){
    //       melodyPlayerLoopMelody = !melodyPlayerLoopMelody;
    //       melodyPlayerDrawScreen();
    //     }
    //     else{
    //       displayBacklightOn();
    //       return false;
    //     }
    //   }
    }
    
    buzNoTone();
    //pinMode(pinBuzzer, INPUT);
    delay(1000);
  }while(melodyPlayerLoopMelody);
  //displayBacklightOn();
  return true;
}

void melodyPlayerDrawScreen() {
//   displayClear();
//   displayDrawVector(/*path*/getPathZubat(), /*x*/24, /*y*/10, /*animate*/false, /*color*/1);
// #ifdef LANG_EN
//   displayDrawText(20, 60, 1, F("Playing..."));
// #endif
// #ifdef LANG_RU
//   displayDrawText(17, 60, 1, F("Пoпиликaeм!"));
// #endif
// #ifdef LANG_UA
//   displayDrawText(20, 60, 1, F("Виконую..."));
// #endif

//   if(!melodyPlayerLoopMelody){
//     byte hour = rtcGetHours();
//     byte minute = rtcGetMinutes();
//     sprintf(buffer, timeFormat, hour, minute);
//     displayDrawText(0, 0, 1, buffer);
//   }
//   else{
//     displayDrawIconFlip(0, 0, 1);
//   }
  
//   displayDrawBattery(79, 0);
//   displayUpdate();
}

void printBits(byte myByte) {
  for (byte mask = 0x80; mask; mask >>= 1) {
    if (mask & myByte)
      Serial.print('1');
    else
      Serial.print('0');
  }
}

int melodyPlayerGetLength(const byte* melody) {
  for (byte i = 0; i < 254; i++) {
    byte b = (melody[i]);
    if (b == 0b11111111) {
      return i + 1;
    }
  }
  return 0;
}