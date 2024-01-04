int ledTopValue = 0;
int ledBottomValue = 0;


void initLed(){
  //test LEDs
  pinMode(LED_TOP_PIN, OUTPUT);
  pinMode(LED_BOTTOM_PIN, OUTPUT);
  pinMode(LED_STATUS_PIN, OUTPUT);
}

void ledSelftest(){
    digitalWrite(LED_TOP_PIN, HIGH); delay(50);
    digitalWrite(LED_BOTTOM_PIN, HIGH); delay(50);
    digitalWrite(LED_STATUS_PIN, HIGH); delay(50);
    digitalWrite(LED_TOP_PIN, LOW); delay(50);
    digitalWrite(LED_BOTTOM_PIN, LOW); delay(50);
    digitalWrite(LED_STATUS_PIN, LOW); delay(50);
}

void ledStatusOn(){
  digitalWrite(LED_STATUS_PIN, HIGH);
}
void ledStatusOff(){
  digitalWrite(LED_STATUS_PIN, LOW);
}

bool isFlashlightOn(){
  return (ledTopValue != 0 ||  ledBottomValue != 0);
}
void ledFlashlightToggleTop(){
  if(ledTopValue == 0){
    ledTopValue = 255;
  }
  else{
    ledTopValue = 0;
  }
  digitalWrite(LED_TOP_PIN, ledTopValue==0?LOW:HIGH); 
}
void ledFlashlightToggleBottom(){
  if(ledBottomValue == 0){
    ledBottomValue = 255;
  }
  else{
    ledBottomValue = 0;
  }
  analogWrite(LED_BOTTOM_PIN, ledBottomValue); 
}
void ledFlashlightOffAll(){
  analogWrite(LED_TOP_PIN, 0); 
  // analogWrite(LED_BOTTOM_PIN, 0); 
  analogWrite(LED_STATUS_PIN, 0); 
}
void ledFlashlightTopOn(){ 
  analogWrite(LED_TOP_PIN, 180); 
}
void ledFlashlightBottonOn(){
  // analogWrite(LED_BOTTOM_PIN, 180); 
  analogWrite(LED_STATUS_PIN, 50); 
}