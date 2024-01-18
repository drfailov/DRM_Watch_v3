int ledTopValue = 0;
int ledBottomValue = 0;


void initLed(){
  //test LEDs
  pinMode(LED_TOP_PIN, OUTPUT);
  pinMode(LED_BOTTOM_PIN, OUTPUT);
  pinMode(LED_STATUS_PIN, OUTPUT);
  ledFlashlightOffAll();
}

void ledSelftest(){
    ledFlashlightOnTop(); delay(50);
    ledFlashlightOnBottom(); delay(50);
    digitalWrite(LED_STATUS_PIN, HIGH); delay(50);
    ledFlashlightOffTop(); delay(50);
    ledFlashlightOffBottom(); delay(50);
    digitalWrite(LED_STATUS_PIN, LOW); delay(50);
}

void ledStatusOn(){
  digitalWrite(LED_STATUS_PIN, HIGH);
}
void ledStatusOff(){
  digitalWrite(LED_STATUS_PIN, LOW);
}

bool isFlasthilghTopOn(){
  return ledTopValue > 20;
}

bool isFlasthilghBottomOn(){
  return ledBottomValue > 20;
}

bool isFlashlightOn(){
  return (isFlasthilghTopOn() ||  isFlasthilghBottomOn());
}

void ledFlashlightSetValueTop(int ledTopValue){
  analogWrite(LED_TOP_PIN, ledTopValue); 
}

void ledFlashlightOnTop(){
  ledTopValue = 255;
  analogWrite(LED_TOP_PIN, ledTopValue); 
}

void ledFlashlightOffTop(){
  ledTopValue = 7;
  if(esp_sleep_get_wakeup_cause() == ESP_SLEEP_WAKEUP_TIMER || isOff())/*periodical wakeup*/
    ledTopValue = 0;
  analogWrite(LED_TOP_PIN, ledTopValue); 
}

void ledFlashlightOnBottom(){
  ledBottomValue = 255;
  analogWrite(LED_BOTTOM_PIN, ledBottomValue); 
}

void ledFlashlightOffBottom(){
  ledBottomValue = 1;
  if(esp_sleep_get_wakeup_cause() == ESP_SLEEP_WAKEUP_TIMER || isOff())/*periodical wakeup*/
    ledBottomValue = 0;
  analogWrite(LED_BOTTOM_PIN, ledBottomValue); 
}

void ledFlashlightToggleTop(){
  if(isFlasthilghTopOn())
    ledFlashlightOffTop();
  else
    ledFlashlightOnTop();
}

void ledFlashlightToggleBottom(){
  if(isFlasthilghBottomOn())
    ledFlashlightOffBottom();
  else
    ledFlashlightOnBottom();
}

void ledFlashlightOffAll(){
  ledFlashlightOffBottom();
  ledFlashlightOffTop();
}