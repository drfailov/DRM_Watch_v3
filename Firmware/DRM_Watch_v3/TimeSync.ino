#include <WiFi.h>
#include "time.h"

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
    drawMessage("Збір даних...");
    unsigned long lastSyncTime = getLastTimeSync(); //s
    unsigned long measuredTime = rtc()->getEpoch(); //s
    unsigned long syncStartedMillis = millis();     //millis
    drawMessage("Скидання часу...");
    rtc()->setTime(0);
    drawMessage("З'єднання з сервером...");
    configTime(0, 0, ntpServer, ntpServer1, ntpServer2);
    printLocalTime();
    delay(500);
    saveLastTimeSync(rtc()->getEpoch());
    drawMessage("Відновлення часового поясу...");
    initTime();
    unsigned long timeForSyncMillis = (millis()-syncStartedMillis) ;  //ms
    measuredTime += timeForSyncMillis / 1000; //s
    unsigned long sinceLastSync = measuredTime-lastSyncTime;  //s

    bool lastSyncValid = lastSyncTime > 1609459200;  // 1st Jan 2021 00:00:00;
    bool measuredTimeValid = measuredTime > lastSyncTime;
    bool sinceLastSyncValid = sinceLastSync > 60*60*10; //60*60*10  = 10h
    
    drawMessage(String("timeForSyncMillis=")+timeForSyncMillis);
    drawMessage(String("measuredTime=")+measuredTime);
    drawMessage(String("lastSyncTime=")+lastSyncTime);
    drawMessage(String("sinceLastSync=")+sinceLastSync);

    drawMessage(String("lastSyncValid=")+lastSyncValid);
    drawMessage(String("measuredTimeValid=")+measuredTimeValid);
    drawMessage(String("sinceLastSyncValid=")+sinceLastSyncValid);
    if(lastSyncValid && measuredTimeValid && sinceLastSyncValid){
      drawMessage("Аналіз похибки...");
      unsigned long actualTime = rtc()->getEpoch(); //s
      long delta = actualTime-measuredTime;  //s
      double deltaDouble = delta;
      double sinceLastSyncDouble = sinceLastSync;
      double coefficient = deltaDouble/sinceLastSyncDouble; 
      if(saveTimeCoef(coefficient))
        drawMessage(String("SAVED coefficient=")+String(coefficient, 6));
      else
        drawMessage(String("ERROR SAVING coefficient=")+String(coefficient, 6));

      drawMessage(String("actualTime=")+actualTime);
      drawMessage(String("delta=")+delta);
      drawMessage(String("deltaDouble=")+deltaDouble);
      drawMessage(String("sinceLastSyncDouble=")+sinceLastSyncDouble);
    }
    wifiOff();
    setModeWatchface();
  }
  
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