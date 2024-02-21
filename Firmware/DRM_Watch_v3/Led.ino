const int LED_TOP = 0;
const int LED_BOTTOM = 1;
const int LED_STATUS = 2;
const int LED_CNT = 3;
float ledActual[LED_CNT];
float ledTarget[LED_CNT];
hw_timer_t *Timer_Cfg;
float led_coef = 0.03;

void IRAM_ATTR Timer_ISR()
{
  for(int i=0; i<LED_CNT; i++){
    float dv = ledTarget[i]-ledActual[i];
    ledActual[i] += dv*led_coef;
  }
  analogWrite(LED_TOP_PIN, (int)ledActual[LED_TOP]); 
  analogWrite(LED_BOTTOM_PIN, (int)ledActual[LED_BOTTOM]); 
  analogWrite(LED_STATUS_PIN, (int)ledActual[LED_STATUS]);   
}

void initLed(){
  pinMode(LED_TOP_PIN, OUTPUT);
  pinMode(LED_BOTTOM_PIN, OUTPUT);
  pinMode(LED_STATUS_PIN, OUTPUT);
  ledFlashlightOffAll();
  Timer_Cfg = timerBegin(0, 80, true);
  timerAttachInterrupt(Timer_Cfg, &Timer_ISR, true);
  timerAlarmWrite(Timer_Cfg, 3000, true);
  timerAlarmEnable(Timer_Cfg);
}

void ledSelftest(){
    ledFlashlightOnTop(); delay(50);
    ledFlashlightOnBottom(); delay(50);
    ledStatusOn(); delay(50);
    ledFlashlightOffTop(); delay(50);
    ledFlashlightOffBottom(); delay(50);
    ledStatusOff(); delay(50);
}

bool isFlasthilghTopOn(){ return ledTarget[LED_TOP] > 20; }
bool isFlasthilghBottomOn(){ return ledTarget[LED_BOTTOM] > 20;}
bool isStatusLedOn(){ return ledTarget[LED_STATUS] > 20;}
bool isFlashlightOn(){ return (isFlasthilghTopOn() ||  isFlasthilghBottomOn()); }

void ledFlashlightOnTop(){ ledTarget[LED_TOP] = 255; }
void ledFlashlightOnBottom(){ ledTarget[LED_BOTTOM] = 255; }
void ledStatusOn(){ ledTarget[LED_STATUS] = 255; }

void ledFlashlightOffTop(){ ledTarget[LED_TOP] = (esp_sleep_get_wakeup_cause() == ESP_SLEEP_WAKEUP_TIMER || isOff())?0:7;}
void ledFlashlightOffBottom(){ ledTarget[LED_BOTTOM] = (esp_sleep_get_wakeup_cause() == ESP_SLEEP_WAKEUP_TIMER || isOff())?0:1;}
void ledFlashlightOffAll(){ ledFlashlightOffBottom(); ledFlashlightOffTop();}
void ledStatusOff(){ ledTarget[LED_STATUS] = 0; }

void ledFlashlightToggleTop(){ if(isFlasthilghTopOn()) ledFlashlightOffTop(); else ledFlashlightOnTop();}
void ledFlashlightToggleBottom(){ if(isFlasthilghBottomOn()) ledFlashlightOffBottom(); else ledFlashlightOnBottom();}



