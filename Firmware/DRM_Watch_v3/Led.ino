int ledTopValue = 0;
int ledBottomValue = 0;

void initLed(){
  //test LEDs
  pinMode(LED_TOP_PIN, OUTPUT);
  pinMode(LED_BOTTOM_PIN, OUTPUT);
  pinMode(LED_STATUS_PIN, OUTPUT);
  if(esp_sleep_get_wakeup_cause() == 0)/*If first start*/ { 
    digitalWrite(LED_TOP_PIN, HIGH); delay(150);
    digitalWrite(LED_BOTTOM_PIN, HIGH); delay(150);
    digitalWrite(LED_STATUS_PIN, HIGH); delay(150);
    digitalWrite(LED_TOP_PIN, LOW); delay(150);
    digitalWrite(LED_BOTTOM_PIN, LOW); delay(150);
    digitalWrite(LED_STATUS_PIN, LOW); delay(150);
  }
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