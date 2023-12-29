#include <WiFi.h>
#include "time.h"

const char* ssid     = "Dr. Failov:VPN";
const char* password = "Manuleee";

const char* ntpServer = "time.windows.com"; 
//const char* ntpServer = "pool.ntp.org"; 
//const char* ntpServer = "time.google.com"; 
const long  gmtOffset_sec = 7200;
const int   daylightOffset_sec = 3600;

void setModeTimeSync(){
  Serial.println(F("Set mode: TimeSync"));
  modeSetup = setModeTimeSync;
  modeLoop = modeTimeSyncLoop;
  modeButtonUp = doNothing;
  modeButtonCenter = modeTimeSyncButtonCenter;
  modeButtonDown = doNothing;
  //modeButtonUpLong = modeWatchfaceButtonUp;
  //modeButtonCenterLong = modeWatchfaceButtonUp;
  //modeButtonDownLong = modeWatchfaceButtonUp;
}


void modeTimeSyncLoop(){
  drawMessage("З'єднання з Wi-Fi...");
    // Connect to Wi-Fi
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  drawMessage("З'єднання з сервером...");
  Serial.println("");
  Serial.println("WiFi connected.");
  
  // Init and get the time
  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
  printLocalTime();
  

  drawMessage("Вимкнення Wi-Fi...");
  //disconnect WiFi as it's no longer needed
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