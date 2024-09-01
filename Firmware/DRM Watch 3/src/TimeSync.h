#ifndef TIMESYNC_H
#define TIMESYNC_H


/*PROTOTYPES*/
void timeSync();
void setTimezone(String timezone);
void printLocalTime();

#include <WiFi.h>
#include "time.h"
#include <Arduino.h>
#include "DrmPreferences.h"
#include "RTC.h"
#include "Lcd.h"
#include "SettingsMenu/ModeWiFiScanner.h"

//const long  gmtOffset_sec = 7200;
//const int   daylightOffset_sec = 3600;

const char* ntpServer = "time.windows.com"; 
const char* ntpServer1 = "pool.ntp.org"; 
const char* ntpServer2 = "time.google.com"; 

void initTime(){
  //src: https://gist.github.com/alwynallan/24d96091655391107939 
  //setTimezone("EET-2EEST,M3.5.0/3,M10.5.0/4"); //kyiv
}

/*Test I made:
21:25 - success auto sync
21:30 - success auto sync
21:32 - me: Deleted Wi-Fi network
21:35 - unsuccessful auto sync
21:36 - unsuccessful auto sync
21:37 - unsuccessful auto sync
21:40 - me: reset all time
21:41 - unsuccessful auto sync
21:45 - me: Updated code
21:46 - unsuccessful auto sync
21:47 - unsuccessful auto sync
21:48 - unsuccessful auto sync
21:49 - unsuccessful auto sync
21:50 - me: Added wifi
21:51 - success auto sync
21:56 - success auto sync
22:01 - success auto sync*/
void loopTimeAutoSync(){
  if(!getTimeSyncEnabled())
    return;
  long intervalBetweenSuccessSync = 22*60*60;//s    : 20h
  long intervalBetweenFailedSync = 4*60*60;//s    : 4h
  long now = _rtcInternal()->getEpoch();
  long sinceLastSync = now - getLastTimeSync();
  long sinceLastTrySync = now - getLastTryTimeSync();
  if((now < getLastTryTimeSync() || sinceLastTrySync > intervalBetweenFailedSync) 
   && (now < getLastTimeSync() || sinceLastSync > intervalBetweenSuccessSync)){
    timeSync();
  }
}

void setTimezone(String timezone){
  Serial.printf("  Setting Timezone to %s\n",timezone.c_str());
  setenv("TZ",timezone.c_str(),1);  //  Now adjust the TZ.  Clock settings are adjusted to show the new local time
  tzset();
}

void timeSync(){
  saveLastTryTimeSync(_rtcInternal()->getEpoch());
  drawDim();
  if(connectToKnownWifi()){
    unsigned long syncStartedMillis = millis();     //millis
    _rtcInternal()->setTime(0);
    drawMessageAnimated(L("Синхронізація годинника...", "Clock sync..."));
    configTime(0, 0, ntpServer, ntpServer1, ntpServer2);
    printLocalTime();
    delay(500);
    unsigned long thisSyncTime = _rtcInternal()->getEpoch();
    saveLastTryTimeSync(thisSyncTime);
    saveLastTimeSync(thisSyncTime);

    bool thisSyncValid = thisSyncTime > 1609459200;  // 1st Jan 2021 00:00:00;
    if(thisSyncValid){
      unsigned long actualTime = _rtcInternal()->getEpoch(); //s
      adjustExternalRtc(actualTime);
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


#endif