#ifndef MODEOFF_H
#define MODEOFF_H

void setModeOff();
bool isOff();
void modeOffLoop();
void turnOn();
void turnOff();
void reboot();

#include "Global.h"
#include "AutoSleep.h"
#include "Button.h"
#include "Lcd.h"

void setModeOff()
{
  lcd()->setColorIndex(white);
  lcd()->drawBox(0, 0, 400, 240);
  lcd()->sendBuffer();
  delay(10);
  lcdPowerOff();
  modeSetup = setModeOff;
  modeLoop = modeOffLoop;
  modeButtonUp = 0;
  modeButtonCenter = 0;
  modeButtonDown = 0;
  modeButtonUpLong = 0;
  modeButtonCenterLong = 0;
  modeButtonDownLong = 0;
  registerAction();
  enableAutoReturn = false;
  enableAutoSleep = false;
  autoReturnTime = autoReturnDefaultTime;
  autoSleepTime = autoSleepDefaultTime;
  if (esp_sleep_get_wakeup_cause() == ESP_SLEEP_WAKEUP_EXT0)
  { /*By button*/
    lcdPowerOn();
    lcd()->initInterface();
  }
}
void modeOffLoop()
{
  if (esp_sleep_get_wakeup_cause() == ESP_SLEEP_WAKEUP_EXT0)
  { /*By button*/
    {
      lcd()->setColorIndex(white);
      lcd()->drawBox(0, 0, 400, 240);
      lcd()->setColorIndex(black);
      lcd()->setFont(u8g2_font_10x20_t_cyrillic);
      lcd()->setCursor(90, 105);
      lcd()->enableUTF8Print();
      lcd()->print("Тримайте щоб увімкнути");

      drawBattery(190, 130);
      lcd()->sendBuffer();
    }
    if (!isButtonCenterPressed())
    {
      lcdPowerOff();
      goToSleep();
    }
    if (millis() > 4000)
    {
      turnOn();
    }
  }
  else
  {
    lcdPowerOff();
    goToSleep();
  }
}

bool isOff()
{
  return modeSetup == setModeOff; // deviceOff;
}

void turnOn()
{
  ESP.restart();
}
void turnOff()
{
  if (!isOff())
  {
    clearScreenAnimation();
    drawMessage("Вимкнення...");
    delay(500);
  }
  setModeOff();
}

void reboot(){
  clearScreenAnimation();
  Wire.flush();
  delay(50);
  Wire.end();
  delay(50);
  esp_restart();
}

#endif