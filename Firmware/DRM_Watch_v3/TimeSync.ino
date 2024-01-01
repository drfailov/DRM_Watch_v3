#include <WiFi.h>
#include "time.h"

const char* ssid     = "Dr. Failov:VPN";
const char* password = "Manuleee";
const long  gmtOffset_sec = 7200;
const int   daylightOffset_sec = 3600;

const char* ntpServer = "time.windows.com"; 
const char* ntpServer1 = "pool.ntp.org"; 
const char* ntpServer2 = "time.google.com"; 


// void setModeTimeSync(){
//   Serial.println(F("Set mode: TimeSync"));
//   modeSetup = setModeTimeSync;
//   modeLoop = modeTimeSyncLoop;
//   modeButtonUp = doNothing;
//   modeButtonCenter = modeTimeSyncButtonCenter;
//   modeButtonDown = doNothing;
//   //modeButtonUpLong = modeWatchfaceButtonUp;
//   //modeButtonCenterLong = modeWatchfaceButtonUp;
//   //modeButtonDownLong = modeWatchfaceButtonUp;
// }
void initTime(){
  //src: https://gist.github.com/alwynallan/24d96091655391107939 
  setTimezone("EET-2EEST,M3.5.0/3,M10.5.0/4"); //kyiv
}


void setTimezone(String timezone){
  Serial.printf("  Setting Timezone to %s\n",timezone.c_str());
  setenv("TZ",timezone.c_str(),1);  //  Now adjust the TZ.  Clock settings are adjusted to show the new local time
  tzset();
}


void timeSync(){
  if(connectToKnownWifi()){
    drawMessage("Скидання часу...");
    rtc()->setTime(0);
    drawMessage("З'єднання з сервером...");
    configTime(0, 0, ntpServer);
    printLocalTime();
    initTime();
  }
  drawMessage("Вимкнення Wi-Fi...");
  WiFi.disconnect(true);
  WiFi.mode(WIFI_OFF);
  resetTemperatureSensor();
  setModeWatchface();
  drawMessage("Готово.");
  delay(500);

}

void printLocalTime()
{
  struct tm timeinfo;
  if(!getLocalTime(&timeinfo)){
    Serial.println("Failed to obtain time");
    return;
  }
  Serial.println(&timeinfo, "%A, %B %d %Y %H:%M:%S");
}

void modeTimeSyncButtonCenter(){
  setModeWatchface();
}