#include <ToneESP32.h>

ToneESP32 buzzer(BUZZER_PIN, /*BUZZER_CHANNEL*/0);


void buzzerInit(){
  if(esp_sleep_get_wakeup_cause() == 0){
    buzzer.tone(3000, 50);
    buzzer.tone(2800, 50);
  }
}

void buzTone(int freq){
  buzzer.tone(freq);
  //delay(duration);
}
void buzTone(int freq, int duration){
  buzzer.tone(freq, duration);
  //delay(duration);
}
void buzNoTone(){
    buzzer.noTone(); 
}

void buttonBeep(){
  buzzer.tone(2000, 50);
}
void buttonLongBeep(){
  buzzer.tone(2000, 150);
}



void playInit(){
  for(byte i=2; i<5; i++){
    buzzer.tone(i*1000, 100);
    buzzer.noTone(); 
    delay(100);
  }
  for(byte i=0; i<6; i++){
    ledStatusOn();
    buzzer.tone(5000, 30);  
    ledStatusOff();
    buzzer.noTone(); 
    delay(30);
  }
}





  
void playMelody(){
  //buzzer.tone(1000, 250);

  //tempo: 120

//Note: 8a1, index: 0, tonS: a, durS: 8, octS: 1, nAoS: a1
//NoteNumber: 9, timeMs: 250, frq: 437,26613593193156
//noteData: [01001001]
buzzer.tone(  437,250);
noTone( 10);

//Note: 8a1, index: 1, tonS: a, durS: 8, octS: 1, nAoS: a1
//NoteNumber: 9, timeMs: 250, frq: 437,26613593193156
//noteData: [01001001]
buzzer.tone(  437,250);
noTone( 10);

//Note: 16g1, index: 2, tonS: g, durS: 16, octS: 1, nAoS: g1
//NoteNumber: 7, timeMs: 125, frq: 389,5598399879372
//noteData: [10000111]
buzzer.tone(  389,125);
noTone( 10);

//Note: 16#f1, index: 3, tonS: #f, durS: 16, octS: 1, nAoS: #f1
//NoteNumber: 6, timeMs: 125, frq: 367,6955262170047
//noteData: [10000110]
buzzer.tone(  367,125);
noTone( 10);

//Note: 4g1, index: 4, tonS: g, durS: 4, octS: 1, nAoS: g1
//NoteNumber: 7, timeMs: 500, frq: 389,5598399879372
//noteData: [00000111]
buzzer.tone(  389,500);
noTone( 10);

//Note: 4e1, index: 5, tonS: e, durS: 4, octS: 1, nAoS: e1
//NoteNumber: 4, timeMs: 500, frq: 327,579472972667
//noteData: [00000100]
buzzer.tone(  327,500);
noTone( 10);

//Note: 8e1, index: 6, tonS: e, durS: 8, octS: 1, nAoS: e1
//NoteNumber: 4, timeMs: 250, frq: 327,579472972667
//noteData: [01000100]
buzzer.tone(  327,250);
noTone( 10);

//Note: 8a1, index: 7, tonS: a, durS: 8, octS: 1, nAoS: a1
//NoteNumber: 9, timeMs: 250, frq: 437,26613593193156
//noteData: [01001001]
buzzer.tone(  437,250);
noTone( 10);

//Note: 8a1, index: 8, tonS: a, durS: 8, octS: 1, nAoS: a1
//NoteNumber: 9, timeMs: 250, frq: 437,26613593193156
//noteData: [01001001]
buzzer.tone(  437,250);
noTone( 10);

//Note: 8g1, index: 9, tonS: g, durS: 8, octS: 1, nAoS: g1
//NoteNumber: 7, timeMs: 250, frq: 389,5598399879372
//noteData: [01000111]
buzzer.tone(  389,250);
noTone( 10);

//Note: 8#f1, index: 10, tonS: #f, durS: 8, octS: 1, nAoS: #f1
//NoteNumber: 6, timeMs: 250, frq: 367,6955262170047
//noteData: [01000110]
buzzer.tone(  367,250);
noTone( 10);

//Note: 8a1, index: 11, tonS: a, durS: 8, octS: 1, nAoS: a1
//NoteNumber: 9, timeMs: 250, frq: 437,26613593193156
//noteData: [01001001]
buzzer.tone(  437,250);
noTone( 10);

//Note: 8a1, index: 12, tonS: a, durS: 8, octS: 1, nAoS: a1
//NoteNumber: 9, timeMs: 250, frq: 437,26613593193156
//noteData: [01001001]
buzzer.tone(  437,250);
noTone( 10);

//Note: 16g1, index: 13, tonS: g, durS: 16, octS: 1, nAoS: g1
//NoteNumber: 7, timeMs: 125, frq: 389,5598399879372
//noteData: [10000111]
buzzer.tone(  389,125);
noTone( 10);

//Note: 16f1, index: 14, tonS: f, durS: 16, octS: 1, nAoS: f1
//NoteNumber: 5, timeMs: 125, frq: 347,0583620842089
//noteData: [10000101]
buzzer.tone(  347,125);
noTone( 10);

//Note: 4g1, index: 15, tonS: g, durS: 4, octS: 1, nAoS: g1
//NoteNumber: 7, timeMs: 500, frq: 389,5598399879372
//noteData: [00000111]
buzzer.tone(  389,500);
noTone( 10);

//Note: 4e1, index: 16, tonS: e, durS: 4, octS: 1, nAoS: e1
//NoteNumber: 4, timeMs: 500, frq: 327,579472972667
//noteData: [00000100]
buzzer.tone(  327,500);
noTone( 10);

//Note: 8#f1, index: 17, tonS: #f, durS: 8, octS: 1, nAoS: #f1
//NoteNumber: 6, timeMs: 250, frq: 367,6955262170047
//noteData: [01000110]
buzzer.tone(  367,250);
noTone( 10);

//Note: 8d1, index: 18, tonS: d, durS: 8, octS: 1, nAoS: d1
//NoteNumber: 2, timeMs: 250, frq: 291,840132560437
//noteData: [01000010]
buzzer.tone(  291,250);
noTone( 10);

//Note: 4e1, index: 19, tonS: e, durS: 4, octS: 1, nAoS: e1
//NoteNumber: 4, timeMs: 500, frq: 327,579472972667
//noteData: [00000100]
buzzer.tone(  327,500);
noTone( 10);

//Note: 8-, index: 20, tonS: -, durS: 8, octS: , nAoS: -
//NoteNumber: 36, timeMs: 250, frq: 2080
//noteData: [01100100]
delay(250);

//Note: 8a2, index: 21, tonS: a, durS: 8, octS: 2, nAoS: a2
//NoteNumber: 21, timeMs: 250, frq: 874,5322718638631
//noteData: [01010101]
buzzer.tone(  874,250);
noTone( 10);

//Note: 8a2, index: 22, tonS: a, durS: 8, octS: 2, nAoS: a2
//NoteNumber: 21, timeMs: 250, frq: 874,5322718638631
//noteData: [01010101]
buzzer.tone(  874,250);
noTone( 10);

//Note: 16g2, index: 23, tonS: g, durS: 16, octS: 2, nAoS: g2
//NoteNumber: 19, timeMs: 125, frq: 779,1196799758744
//noteData: [10010011]
buzzer.tone(  779,125);
noTone( 10);

//Note: 16#f2, index: 24, tonS: #f, durS: 16, octS: 2, nAoS: #f2
//NoteNumber: 18, timeMs: 125, frq: 735,3910524340095
//noteData: [10010010]
buzzer.tone(  735,125);
noTone( 10);

//Note: 4g2, index: 25, tonS: g, durS: 4, octS: 2, nAoS: g2
//NoteNumber: 19, timeMs: 500, frq: 779,1196799758744
//noteData: [00010011]
buzzer.tone(  779,500);
noTone( 10);

//Note: 4e2, index: 26, tonS: e, durS: 4, octS: 2, nAoS: e2
//NoteNumber: 16, timeMs: 500, frq: 655,158945945334
//noteData: [00010000]
buzzer.tone(  655,500);
noTone( 10);

//Note: 8e2, index: 27, tonS: e, durS: 8, octS: 2, nAoS: e2
//NoteNumber: 16, timeMs: 250, frq: 655,158945945334
//noteData: [01010000]
buzzer.tone(  655,250);
noTone( 10);

//Note: 8a2, index: 28, tonS: a, durS: 8, octS: 2, nAoS: a2
//NoteNumber: 21, timeMs: 250, frq: 874,5322718638631
//noteData: [01010101]
buzzer.tone(  874,250);
noTone( 10);

//Note: 8a2, index: 29, tonS: a, durS: 8, octS: 2, nAoS: a2
//NoteNumber: 21, timeMs: 250, frq: 874,5322718638631
//noteData: [01010101]
buzzer.tone(  874,250);
noTone( 10);

//Note: 8g2, index: 30, tonS: g, durS: 8, octS: 2, nAoS: g2
//NoteNumber: 19, timeMs: 250, frq: 779,1196799758744
//noteData: [01010011]
buzzer.tone(  779,250);
noTone( 10);

//Note: 8#f2, index: 31, tonS: #f, durS: 8, octS: 2, nAoS: #f2
//NoteNumber: 18, timeMs: 250, frq: 735,3910524340095
//noteData: [01010010]
buzzer.tone(  735,250);
noTone( 10);

//Note: 8a2, index: 32, tonS: a, durS: 8, octS: 2, nAoS: a2
//NoteNumber: 21, timeMs: 250, frq: 874,5322718638631
//noteData: [01010101]
buzzer.tone(  874,250);
noTone( 10);

//Note: 8a2, index: 33, tonS: a, durS: 8, octS: 2, nAoS: a2
//NoteNumber: 21, timeMs: 250, frq: 874,5322718638631
//noteData: [01010101]
buzzer.tone(  874,250);
noTone( 10);

//Note: 16g2, index: 34, tonS: g, durS: 16, octS: 2, nAoS: g2
//NoteNumber: 19, timeMs: 125, frq: 779,1196799758744
//noteData: [10010011]
buzzer.tone(  779,125);
noTone( 10);

//Note: 16f2, index: 35, tonS: f, durS: 16, octS: 2, nAoS: f2
//NoteNumber: 17, timeMs: 125, frq: 694,1167241684178
//noteData: [10010001]
buzzer.tone(  694,125);
noTone( 10);

//Note: 4g2, index: 36, tonS: g, durS: 4, octS: 2, nAoS: g2
//NoteNumber: 19, timeMs: 500, frq: 779,1196799758744
//noteData: [00010011]
buzzer.tone(  779,500);
noTone( 10);

//Note: 4e2, index: 37, tonS: e, durS: 4, octS: 2, nAoS: e2
//NoteNumber: 16, timeMs: 500, frq: 655,158945945334
//noteData: [00010000]
buzzer.tone(  655,500);
noTone( 10);

//Note: 8#f2, index: 38, tonS: #f, durS: 8, octS: 2, nAoS: #f2
//NoteNumber: 18, timeMs: 250, frq: 735,3910524340095
//noteData: [01010010]
buzzer.tone(  735,250);
noTone( 10);

//Note: 8d2, index: 39, tonS: d, durS: 8, octS: 2, nAoS: d2
//NoteNumber: 14, timeMs: 250, frq: 583,680265120874
//noteData: [01001110]
buzzer.tone(  583,250);
noTone( 10);

//Note: 4e2, index: 40, tonS: e, durS: 4, octS: 2, nAoS: e2
//NoteNumber: 16, timeMs: 500, frq: 655,158945945334
//noteData: [00010000]
buzzer.tone(  655,500);
noTone( 10);

//Note: 8-, index: 41, tonS: -, durS: 8, octS: , nAoS: -
//NoteNumber: 36, timeMs: 250, frq: 2080
//noteData: [01100100]
delay(250);

//Note: 8e2, index: 42, tonS: e, durS: 8, octS: 2, nAoS: e2
//NoteNumber: 16, timeMs: 250, frq: 655,158945945334
//noteData: [01010000]
buzzer.tone(  655,250);
noTone( 10);

//Note: 8g2, index: 43, tonS: g, durS: 8, octS: 2, nAoS: g2
//NoteNumber: 19, timeMs: 250, frq: 779,1196799758744
//noteData: [01010011]
buzzer.tone(  779,250);
noTone( 10);

//Note: 8a2, index: 44, tonS: a, durS: 8, octS: 2, nAoS: a2
//NoteNumber: 21, timeMs: 250, frq: 874,5322718638631
//noteData: [01010101]
buzzer.tone(  874,250);
noTone( 10);

//Note: 4b2, index: 45, tonS: b, durS: 4, octS: 2, nAoS: b2
//NoteNumber: 23, timeMs: 500, frq: 981,6292851889613
//noteData: [00010111]
buzzer.tone(  981,500);
noTone( 10);

//Note: 4b2, index: 46, tonS: b, durS: 4, octS: 2, nAoS: b2
//NoteNumber: 23, timeMs: 500, frq: 981,6292851889613
//noteData: [00010111]
buzzer.tone(  981,500);
noTone( 10);

//Note: 8c3, index: 47, tonS: c, durS: 8, octS: 3, nAoS: c3
//NoteNumber: 24, timeMs: 250, frq: 1040
//noteData: [01011000]
buzzer.tone(  1040,250);
noTone( 10);

//Note: 8a2, index: 48, tonS: a, durS: 8, octS: 2, nAoS: a2
//NoteNumber: 21, timeMs: 250, frq: 874,5322718638631
//noteData: [01010101]
buzzer.tone(  874,250);
noTone( 10);

//Note: 2b2, index: 49, tonS: b, durS: 2, octS: 2, nAoS: b2
//NoteNumber: 23, timeMs: 1000, frq: 981,6292851889613
//noteData: [00010111]
buzzer.tone(  981,1000);
noTone( 10);

//Note: 8-, index: 50, tonS: -, durS: 8, octS: , nAoS: -
//NoteNumber: 36, timeMs: 250, frq: 2080
//noteData: [01100100]
delay(250);

//Note: 8a2, index: 51, tonS: a, durS: 8, octS: 2, nAoS: a2
//NoteNumber: 21, timeMs: 250, frq: 874,5322718638631
//noteData: [01010101]
buzzer.tone(  874,250);
noTone( 10);

//Note: 8a2, index: 52, tonS: a, durS: 8, octS: 2, nAoS: a2
//NoteNumber: 21, timeMs: 250, frq: 874,5322718638631
//noteData: [01010101]
buzzer.tone(  874,250);
noTone( 10);

//Note: 16g2, index: 53, tonS: g, durS: 16, octS: 2, nAoS: g2
//NoteNumber: 19, timeMs: 125, frq: 779,1196799758744
//noteData: [10010011]
buzzer.tone(  779,125);
noTone( 10);

//Note: 16#f2, index: 54, tonS: #f, durS: 16, octS: 2, nAoS: #f2
//NoteNumber: 18, timeMs: 125, frq: 735,3910524340095
//noteData: [10010010]
buzzer.tone(  735,125);
noTone( 10);

//Note: 4g2, index: 55, tonS: g, durS: 4, octS: 2, nAoS: g2
//NoteNumber: 19, timeMs: 500, frq: 779,1196799758744
//noteData: [00010011]
buzzer.tone(  779,500);
noTone( 10);

//Note: 4e2, index: 56, tonS: e, durS: 4, octS: 2, nAoS: e2
//NoteNumber: 16, timeMs: 500, frq: 655,158945945334
//noteData: [00010000]
buzzer.tone(  655,500);
noTone( 10);

//Note: 8e2, index: 57, tonS: e, durS: 8, octS: 2, nAoS: e2
//NoteNumber: 16, timeMs: 250, frq: 655,158945945334
//noteData: [01010000]
buzzer.tone(  655,250);
noTone( 10);

//Note: 8a2, index: 58, tonS: a, durS: 8, octS: 2, nAoS: a2
//NoteNumber: 21, timeMs: 250, frq: 874,5322718638631
//noteData: [01010101]
buzzer.tone(  874,250);
noTone( 10);

//Note: 8a2, index: 59, tonS: a, durS: 8, octS: 2, nAoS: a2
//NoteNumber: 21, timeMs: 250, frq: 874,5322718638631
//noteData: [01010101]
buzzer.tone(  874,250);
noTone( 10);

//Note: 8g2, index: 60, tonS: g, durS: 8, octS: 2, nAoS: g2
//NoteNumber: 19, timeMs: 250, frq: 779,1196799758744
//noteData: [01010011]
buzzer.tone(  779,250);
noTone( 10);

//Note: 8#f2, index: 61, tonS: #f, durS: 8, octS: 2, nAoS: #f2
//NoteNumber: 18, timeMs: 250, frq: 735,3910524340095
//noteData: [01010010]
buzzer.tone(  735,250);
noTone( 10);

//Note: 8a2, index: 62, tonS: a, durS: 8, octS: 2, nAoS: a2
//NoteNumber: 21, timeMs: 250, frq: 874,5322718638631
//noteData: [01010101]
buzzer.tone(  874,250);
noTone( 10);

//Note: 8a2, index: 63, tonS: a, durS: 8, octS: 2, nAoS: a2
//NoteNumber: 21, timeMs: 250, frq: 874,5322718638631
//noteData: [01010101]
buzzer.tone(  874,250);
noTone( 10);

//Note: 16g2, index: 64, tonS: g, durS: 16, octS: 2, nAoS: g2
//NoteNumber: 19, timeMs: 125, frq: 779,1196799758744
//noteData: [10010011]
buzzer.tone(  779,125);
noTone( 10);

//Note: 16f2, index: 65, tonS: f, durS: 16, octS: 2, nAoS: f2
//NoteNumber: 17, timeMs: 125, frq: 694,1167241684178
//noteData: [10010001]
buzzer.tone(  694,125);
noTone( 10);

//Note: 4g2, index: 66, tonS: g, durS: 4, octS: 2, nAoS: g2
//NoteNumber: 19, timeMs: 500, frq: 779,1196799758744
//noteData: [00010011]
buzzer.tone(  779,500);
noTone( 10);

//Note: 4e2, index: 67, tonS: e, durS: 4, octS: 2, nAoS: e2
//NoteNumber: 16, timeMs: 500, frq: 655,158945945334
//noteData: [00010000]
buzzer.tone(  655,500);
noTone( 10);

//Note: 8#f2, index: 68, tonS: #f, durS: 8, octS: 2, nAoS: #f2
//NoteNumber: 18, timeMs: 250, frq: 735,3910524340095
//noteData: [01010010]
buzzer.tone(  735,250);
noTone( 10);

//Note: 8d2, index: 69, tonS: d, durS: 8, octS: 2, nAoS: d2
//NoteNumber: 14, timeMs: 250, frq: 583,680265120874
//noteData: [01001110]
buzzer.tone(  583,250);
noTone( 10);

//Note: 2e2, index: 70, tonS: e, durS: 2, octS: 2, nAoS: e2
//NoteNumber: 16, timeMs: 1000, frq: 655,158945945334
//noteData: [00010000]
buzzer.tone(  655,1000);
noTone( 10);


}