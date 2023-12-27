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