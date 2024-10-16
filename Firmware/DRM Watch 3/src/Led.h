#ifndef LED_H
#define LED_H

/*PROTOTYPES*/
bool isFlasthilghTopOn();
bool isFlasthilghBottomOn();
bool isStatusLedOn();
bool isFlashlightOn();
void ledFlashlightOnTop();
void ledFlashlightOnBottom();
void ledStatusOn();
void ledFlashlightOffTop();
void ledFlashlightOffBottom();
void ledFlashlightDimTop();
void ledFlashlightDimBottom();
void ledFlashlightOffAll();
void ledStatusOff();
void ledFlashlightToggleTop();
void ledFlashlightToggleBottom();
void ledFlashlightToggle();
void ledStatusBlink(int times, int speedMs);

#include <Arduino.h>
#include "Global.h"
#include "Button.h"

const int LED_TOP = 0;
const int LED_BOTTOM = 1;
const int LED_STATUS = 2;
const int LED_CNT = 3;
float ledActual[LED_CNT];
float ledTarget[LED_CNT];
hw_timer_t *Led_Timer_Cfg;
float led_coef = 0.07;

void IRAM_ATTR Led_ISR()
{
  for (int i = 0; i < LED_CNT; i++)
  {
    float dv = ledTarget[i] - ledActual[i];
    if (abs(dv) < 2)
      ledActual[i] = ledTarget[i];
    else
      ledActual[i] += dv * led_coef;
  }
  analogWrite(LED_TOP_PIN, (int)ledActual[LED_TOP]);
  analogWrite(LED_BOTTOM_PIN, (int)ledActual[LED_BOTTOM]);
  analogWrite(LED_STATUS_PIN, (int)ledActual[LED_STATUS]);

  unsigned long warningTime = 20; // s
  if (isFlasthilghTopOn())
  {
    if (!dontSleep && sinceLastAction() > autoSleepDefaultTimeWhenFlashlightOn - (warningTime * 1000))
    { // now is moment to warn user about flashlight will turn off soom
      if (millis() % 2000 > 1000)
        ledFlashlightDimTop(); // blink light to warn user
      else
        ledFlashlightOnTop();
    }
    else
    {
      ledFlashlightOnTop();
    }
  }
  if (isFlasthilghBottomOn())
  {
    if (!dontSleep && sinceLastAction() > autoSleepDefaultTimeWhenFlashlightOn - (warningTime * 1000))
    { // now is moment to warn user about flashlight will turn off soom
      if (millis() % 2000 > 1000)
        ledFlashlightDimBottom(); // blink light to warn user
      else
        ledFlashlightOnBottom();
    }
    else
    {
      ledFlashlightOnBottom();
    }
  }
}

void initLed()
{
  pinMode(LED_TOP_PIN, OUTPUT);
  pinMode(LED_BOTTOM_PIN, OUTPUT);
  pinMode(LED_STATUS_PIN, OUTPUT);
  ledFlashlightOffAll();
  Led_Timer_Cfg = timerBegin(0, 80, true); //// Timer0, делитель 80 (для 1MHz тактової частоти), лічильник вгору
  timerAttachInterrupt(Led_Timer_Cfg, &Led_ISR, true);
  timerAlarmWrite(Led_Timer_Cfg, 3000, true);
  timerAlarmEnable(Led_Timer_Cfg);
}

void ledSelftest()
{
  ledFlashlightOnTop();
  delay(50);
  ledFlashlightOnBottom();
  delay(50);
  ledStatusOn();
  delay(50);
  ledFlashlightOffTop();
  delay(50);
  ledFlashlightOffBottom();
  delay(50);
  ledStatusOff();
  delay(50);
}

bool isFlasthilghTopOn() { return ledTarget[LED_TOP] > 20; }
bool isFlasthilghBottomOn() { return ledTarget[LED_BOTTOM] > 20; }
bool isStatusLedOn() { return ledTarget[LED_STATUS] > 20; }
bool isFlashlightOn() { return (isFlasthilghTopOn() || isFlasthilghBottomOn()); }

void ledFlashlightOnTop() { ledTarget[LED_TOP] = 255; }
void ledFlashlightOnBottom() { ledTarget[LED_BOTTOM] = 255; }
void ledStatusOn() { ledTarget[LED_STATUS] = 255; }

void ledFlashlightDimTop() { ledTarget[LED_TOP] = 50; }
void ledFlashlightDimBottom() { ledTarget[LED_BOTTOM] = 50; }
void ledStatusDim() { ledTarget[LED_STATUS] = 50; }

void ledFlashlightOffTop() { ledTarget[LED_TOP] = (esp_sleep_get_wakeup_cause() == ESP_SLEEP_WAKEUP_TIMER || isOff()) ? 0 : 1; }
void ledFlashlightOffBottom() { ledTarget[LED_BOTTOM] = (esp_sleep_get_wakeup_cause() == ESP_SLEEP_WAKEUP_TIMER || isOff()) ? 0 : 0; }
void ledFlashlightOffAll()
{
  ledFlashlightOffBottom();
  ledFlashlightOffTop();
}
void ledStatusOff() { ledTarget[LED_STATUS] = 0; }

void ledFlashlightToggleTop()
{
  if (isFlasthilghTopOn())
    ledFlashlightOffTop();
  else
    ledFlashlightOnTop();
}
void ledFlashlightToggleBottom()
{
  if (isFlasthilghBottomOn())
    ledFlashlightOffBottom();
  else
    ledFlashlightOnBottom();
}

unsigned long lastFlashlightToggle = 0;
void ledFlashlightToggle()
{
  bool quick = millis() - lastFlashlightToggle < 3000;
  if (!isFlashlightOn())
  { // all off  -> ON red
    ledFlashlightOnTop();
  }
  else if (isFlasthilghTopOn() && !isFlasthilghBottomOn() && quick)
  { // only red and quick  -> ON white
    ledFlashlightOnBottom();
    //ledStatusOn();
  }
  else
  { // other -> OFF all
    ledFlashlightOffAll();
    //ledStatusOff();
  }
  lastFlashlightToggle = millis();
}
void ledStatusBlink(int times, int speedMs)
{
  for (int i = 0; i < times; i++)
  {
    ledStatusOn();
    delay(speedMs);
    ledStatusOff();
    if (i < times - 1)
      delay(speedMs * 2);
  }
}

#endif