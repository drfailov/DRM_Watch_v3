/*! @file DS3231M.h

 @mainpage Arduino Library Header to access the Maxim DS3231M Real Time Clock

 @section intro_section Description

 Class definition header for the DS3231M real-time-clock from Maxim integrated, described at
 https://www.maximintegrated.com/en/products/digital/real-time-clocks/DS3231M.html. The DS3231M's
 data sheet is located at https://datasheets.maximintegrated.com/en/ds/DS3231M.pdf \n\n

 The most recent version of this library can be found at https://github.com/Zanduino/DS3231M and a
 detailed library description is on the library's wiki at https://github.com/Zanduino/DS3231M/wiki
 \n\n

 Use is made of portions of Adafruit's RTClib Version 1.2.0 at https://github.com/adafruit/RTClib
 which is a fork of the original RTClib from Jeelabs. The code encompasses simple classes for time
 and date.

 @section clang-format
 Part of the GitHub actions for CI is running every source file through "clang-format" to ensure
 that coding formatting is done the same for all files. The configuration file ".clang-format" is
 located at https://github.com/Zanduino/Common/tree/main/clang-format and this is used for CI tests
 when pushing to GitHub. The local file, if present in the root directory, is ignored when
 committing and uploading.

 @section license GNU General Public License v3.0

 This program is free software: you can redistribute it and/or modify it under the terms of the GNU
 General Public License as published by the Free Software Foundation, either version 3 of the
 License, or (at your option) any later version. This program is distributed in the hope that it
 will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more details. You should
 have received a copy of the GNU General Public License along with this program.  If not, see
 <http://www.gnu.org/licenses/>.

 @section author Author

 Written by Arnd <Zanshin_Github@sv-zanshin.com> / https://www.github.com/SV-Zanshin

 @section versions Changelog

 Version| Date       | Developer  | Comments
 ------ | ---------- | ---------- | --------
 1.0.5  | 2020-09-28 | gvisca44   | Issue #13 - add function "isStopped()"
 1.0.5  | 2020-09-28 | SV-Zanshin | Issue #14 - Reformat using clang-format
 1.0.4  | 2020-05-17 | neocao123  | Issue #12 - overflow on hours was allowed
 1.0.4  | 2020-05-17 | SV-Zanshin | Cleaned up comments
 1.0.4  | 2019-08-18 | lagg070988 | Issue  #9 - PinAlarm & PinSquareWave logic corrected
 1.0.4  | 2019-02-02 | SV-Zanshin | Issue  #8 - converted documentation to doxygen
 1.0.3  | 2019-01-27 | SV-Zanshin | Issue  #4 - incorrect datatype for I2C causes overflow
 1.0.2  | 2018-07-02 | SV-Zanshin | Added guard code against multiple I2C Speed definitions
 1.0.2  | 2018-06-30 | SV-Zanshin | Issue  #3 - Allow 400KHz I2C bus speed to be set
 1.0.0  | 2017-08-19 | SV-Zanshin | Initial release
 1.0.0b | 2017-08-13 | SV-Zanshin | Initial coding
*/

#include <Wire.h>  // Standard I2C "Wire" library

#include "Arduino.h"  // Arduino data type definitions
#ifndef DS3231M_h
/*! @brief Guard code definition to prevent multiple definitions */
#define DS3231M_h

/**************************************************************************************************
** Declare enumerated types                                                                      **
* ************************************************************************************************/
/*! @brief Declare the enumerated type for the various type of alarm that can be set */
enum alarmTypes {
  everySecond,
  secondsMatch,
  secondsMinutesMatch,
  secondsMinutesHoursMatch,
  secondsMinutesHoursDateMatch,
  secondsMinutesHoursDayMatch,
  everyMinute,
  minutesMatch,
  minutesHoursMatch,
  minutesHoursDateMatch,
  minutesHoursDayMatch,
  UnknownAlarm
};

/**************************************************************************************************
** Declare classes used in within the class                                                      **
**************************************************************************************************/
class TimeSpan;  ///< TimeSpan class definition

/**************************************************************************************************
** Declare constants used in the class                                                           **
**************************************************************************************************/
#ifndef I2C_MODES
/*! @brief Guard code definition to prevent multiple definitions */
#define I2C_MODES
const uint32_t I2C_STANDARD_MODE = 100000;  ///< Default normal I2C 100KHz speed
const uint32_t I2C_FAST_MODE     = 400000;  ///< Fast mode with 400KHz
#endif
const uint32_t SECONDS_PER_DAY           = 86400;      ///< 60 secs * 60 mins * 24 hours
const uint32_t SECONDS_FROM_1970_TO_2000 = 946684800;  ///< Seconds between 1970/1/1 to 2000/1/1
const uint8_t  DS3231M_ADDRESS           = 0x68;       ///< Fixed I2C Address for DS3231M
const uint8_t  DS3231M_RTCSEC            = 0x00;       ///< DS3231 RTCSEC      Register Address
const uint8_t  DS3231M_RTCMIN            = 0x01;       ///< DS3231 RTCMIN      Register Address
const uint8_t  DS3231M_RTCHOUR           = 0x02;       ///< DS3231 RTCHOUR     Register Address
const uint8_t  DS3231M_RTCWKDAY          = 0x03;       ///< DS3231 RTCWKDAY    Register Address
const uint8_t  DS3231M_RTCDATE           = 0x04;       ///< DS3231 RTCDATE     Register Address
const uint8_t  DS3231M_RTCMTH            = 0x05;       ///< DS3231 RTCMTH      Register Address
const uint8_t  DS3231M_RTCYEAR           = 0x06;       ///< DS3231 RTCYEAR     Register Address
const uint8_t  DS3231M_ALM1SEC           = 0x07;       ///< DS3231 ALM1SEC     Register Address
const uint8_t  DS3231M_ALM1MIN           = 0x08;       ///< DS3231 ALM1MIN     Register Address
const uint8_t  DS3231M_ALM1HOUR          = 0x09;       ///< DS3231 ALM1HOUR    Register Address
const uint8_t  DS3231M_ALM1DATE          = 0x0A;       ///< DS3231 ALM1DATE    Register Address
const uint8_t  DS3231M_ALM2MIN           = 0x0B;       ///< DS3231 ALM2SEC     Register Address
const uint8_t  DS3231M_ALM2HOUR          = 0x0C;       ///< DS3231 ALM2HOUR    Register Address
const uint8_t  DS3231M_ALM2DATE          = 0x0D;       ///< DS3231 ALM2DATE    Register Address
const uint8_t  DS3231M_CONTROL           = 0x0E;       ///< DS3231 CONTROL     Register Address
const uint8_t  DS3231M_STATUS            = 0x0F;       ///< DS3231 STATUS      Register Address
const uint8_t  DS3231M_AGING             = 0x10;       ///< DS3231 AGING       Register Address
const uint8_t  DS3231M_TEMPERATURE       = 0x11;       ///< DS3231 TEMPERATURE Register Address

/*!
 @brief    Simple general-purpose date/time class
 @details  Class has no TZ / DST / leap second handling. Copied from RTClib. For further
           information on this implementation see
 https://github.com/Zanduino/DS3231M/wiki/DateTimeClass
*/
class DateTime {
 public:
  DateTime(uint32_t t = 0);
  DateTime(uint16_t year, uint8_t month, uint8_t day, uint8_t hour = 0, uint8_t min = 0,
           uint8_t sec = 0);  //                                  //
  DateTime(const DateTime& copy);
  DateTime(const char* date, const char* time);
  DateTime(const __FlashStringHelper* date, const __FlashStringHelper* time);
  uint16_t year() const { return 2000 + yOff; }  ///< Return the year
  uint8_t  month() const { return m; }           ///< Return the month
  uint8_t  day() const { return d; }             ///< Return the day
  uint8_t  hour() const { return hh; }           ///< Return the hour
  uint8_t  minute() const { return mm; }         ///< Return the minute
  uint8_t  second() const { return ss; }         ///< Return the second
  uint8_t  dayOfTheWeek() const;
  long     secondstime() const;
  uint32_t unixtime(void) const;
  DateTime operator+(const TimeSpan& span);   // addition
  DateTime operator-(const TimeSpan& span);   // subtraction
  TimeSpan operator-(const DateTime& right);  // subtraction
 protected:
  uint8_t yOff,  ///< Year Offset
      m,         ///< Months
      d,         ///< Days
      hh,        ///< Hours
      mm,        ///< Minutes
      ss;        ///< Seconds
};               // of class DateTime definition

/*!
 @brief    Simple general-purpose TimeSpan
 @details  Timespan class which can represent changes in time with seconds accuracy. Copied from
           RTClib. For further infos see **
 https://github.com/Zanduino/DS3231M/wiki/TimeSpanClass
*/
class TimeSpan {
 public:
  TimeSpan(int32_t seconds = 0);
  TimeSpan(int16_t days, int8_t hours, int8_t minutes, int8_t seconds);
  TimeSpan(const TimeSpan& copy);
  int16_t  days() const { return _seconds / 86400L; }      ///< Return days in span
  int8_t   hours() const { return _seconds / 3600 % 24; }  ///< Return hours in span
  int8_t   minutes() const { return _seconds / 60 % 60; }  ///< Return minutes in span
  int8_t   seconds() const { return _seconds % 60; }       ///< Return seconds in span
  int32_t  totalseconds() const { return _seconds; }       ///< Return total span in seconds
  TimeSpan operator+(const TimeSpan& right);
  TimeSpan operator-(const TimeSpan& right);

 protected:
  int32_t _seconds;  ///< internal seconds variable
};                   // of class TimeSpan definition

/*!
 @brief    Main DS3231M class definition for the Real-Time clock
*/
class DS3231M_Class {
 public:
  DS3231M_Class();
  ~DS3231M_Class();                                             // Class destructor
  bool     begin(const uint32_t i2cSpeed = I2C_STANDARD_MODE);  // Start I2C Communications
  void     adjust();                                            // Set the date and time to compile
  void     adjust(const DateTime& dt);                          // Set the date and time
  DateTime now();                                               // return time
  int32_t  temperature();                                       // return clock temp in 100x �C
  bool     isStopped();                                         // Return true if Oscillator stopped
  void     setAlarm(const uint8_t alarmType, const DateTime dt,
                    const bool state = true);         // Set an Alarm
  bool     isAlarm();                                 // Return if alarm is triggered
  void     clearAlarm();                              // Clear the alarm state flag
  void     kHz32(const bool state);                   // Turn 32kHz output on or off
  int8_t   getAgingOffset();                          // Get the clock's aging offset
  int8_t   setAgingOffset(const int8_t agingOffset);  // Set the clock's aging offset
  uint8_t  weekdayRead();                             // Read weekday from RTC
  uint8_t  weekdayWrite(const uint8_t dow);           // Write weekday to RTC
  void     pinAlarm();                                // Make INTSQW go up on alarm
  void     pinSquareWave();                           // Make INTSQW be a 1Hz signal
 private:
  uint8_t  readByte(const uint8_t addr);                       // Read 1 byte from I2C address
  void     writeByte(const uint8_t addr, const uint8_t data);  // Write 1 byte at I2Caddress
  uint8_t  bcd2int(const uint8_t bcd);                         // convert BCD digits to integer
  uint8_t  int2bcd(const uint8_t dec);                         // convert integer to BCD
  uint8_t  _TransmissionStatus = 0;                            ///< Status of I2C transmission
  uint32_t _SetUnixTime        = 0;                            ///< UNIXtime for clock last set
  uint8_t  _ss, _mm, _hh, _d, _m;                              ///< Define date components
  uint16_t _y;                                                 ///< Define date components
};                                                             // of DS3231M class definition
#endif
