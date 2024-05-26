/*! @file SetAlarms.ino

@section SetAlarms_intro_section Description

Example program for using the DS3231M library which allows access to the DS3231M real-time-clock
chip. The library as well as the most current version of this program is available at GitHub using
the address https://github.com/Zanduino/DS3231M and a more detailed description of this program (and
the library) can be found at https://github.com/Zanduino/DS3231M/wiki/Demo.ino \n\n

The DS3231M library uses the standard SPI Wire library for communications with the RTC chip and has
also used the class definitions of the standard RTClib library from Adafruit/Jeelabs. The data sheet
for the DS3231M is located at https://datasheets.maximintegrated.com/en/ds/DS3231M.pdf.

@section SetAlarmslicense __**GNU General Public License v3.0**__

This program is free software: you can redistribute it and/or modify it under the terms of the GNU
General Public License as published by the Free Software Foundation, either version 3 of the
License, or (at your option) any later version. This program is distributed in the hope that it will
be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more details. You should have
received a copy of the GNU General Public License along with this program.  If not, see
<http://www.gnu.org/licenses/>.

@section SetAlarmsauthor Author

 Written by Arnd <Zanshin_Github@sv-zanshin.com> / https://www.github.com/SV-Zanshin

@section SetAlarmsversions Changelog

Version | Date       | Developer  | Comments
------- | ---------- | ---------- | ---------------------------------------------------
1.0.2   | 2020-12-12 | SV-Zanshin | reformatted to clang-format
1.0.1   | 2019-02-02 | SV-Zanshin | Issue #7 - convert documentation to Doxygen
1.0.0   | 2017-08-19 | SV-Zanshin | Initial coding
*/
#include <DS3231M.h>  // Include the DS3231M RTC library
/***************************************************************************************************
** Declare all program constants                                                                  **
***************************************************************************************************/

const uint32_t SERIAL_SPEED{115200};     ///< Set the baud rate for Serial I/O
const uint8_t  LED_PIN{13};              ///< Arduino built-in LED pin number
const uint8_t  SPRINTF_BUFFER_SIZE{32};  ///< Buffer size for sprintf()

/***************************************************************************************************
** Declare global variables and instantiate classes                                               **
***************************************************************************************************/
DS3231M_Class DS3231M;                           ///< Create an instance of the DS3231M Class
char          inputBuffer[SPRINTF_BUFFER_SIZE];  ///< Buffer for sprintf()/sscanf()

void setup() {
  /*!
    @brief    Arduino method called once at startup to initialize the system
    @details  This is an Arduino IDE method which is called first upon boot or restart. It is only
              called one time and then control goes to the main "loop()" method, from which control
              never returns
    @return   void
  */
  Serial.begin(SERIAL_SPEED);
#ifdef __AVR_ATmega32U4__  // If 32U4 then wait for the serial interface to initialize
  delay(3000);
#endif
  Serial.print(F("\nStarting Set program\n"));
  Serial.print(F("- Compiled with c++ version "));
  Serial.print(F(__VERSION__));
  Serial.print(F("\n- On "));
  Serial.print(F(__DATE__));
  Serial.print(F(" at "));
  Serial.print(F(__TIME__));
  Serial.print(F("\n"));
  while (!DS3231M.begin())  // Initialize communications with the RTC
  {
    Serial.println(F("Unable to find DS3231MM. Checking again in 3s."));
    delay(3000);
  }  // of loop until device is located
  Serial.println(F("DS3231M initialized."));
  DS3231M.adjust();  // Set to library compile Date/Time
  Serial.print(F("Date/Time set to compile time: "));
  DateTime now = DS3231M.now();  // get the current time
  // Use sprintf() to pretty print the date/time with leading zeros
  sprintf(inputBuffer, "%04d-%02d-%02d %02d:%02d:%02d", now.year(), now.month(), now.day(),
          now.hour(), now.minute(), now.second());
  Serial.println(inputBuffer);
  Serial.print(F("DS3231M chip temperature is "));
  Serial.print(DS3231M.temperature() / 100.0, 1);  // Value is in 100ths of a degree
  Serial.println(
      "\xC2\xB0"
      "C");
  Serial.println(F("Setting alarm to go off in 12 seconds."));
  DS3231M.setAlarm(secondsMinutesHoursDateMatch,
                   now + TimeSpan(0, 0, 0, 12));  // Alarm goes off in 12 seconds
  Serial.println(F("Setting INT/SQW pin to toggle at 1Hz."));
  DS3231M.pinSquareWave();  // Make 1Hz signal on INT/SQW pin
}  // of method setup()

void loop() {
  /*!
    @brief    Arduino method for the main program loop
    @details  This is the main program for the Arduino IDE, it is an infinite loop and keeps on
              repeating.
    @return   void
  */
  static uint8_t secs;
  DateTime       now = DS3231M.now();  // get the current time
  if (secs != now.second())            // Output if seconds have changed
  {
    sprintf(inputBuffer, "%04d-%02d-%02d %02d:%02d:%02d", now.year(), now.month(), now.day(),
            now.hour(), now.minute(), now.second());
    Serial.println(inputBuffer);  // Display the current date/time
    secs = now.second();          // Set the counter variable
  }                               // of if the seconds have changed
  if (DS3231M.isAlarm())          // If the alarm bit is set
  {
    Serial.println("Alarm has gone off.");
    DS3231M.clearAlarm();
    // Alarm in 12 seconds. This will also reset the alarm state
    DS3231M.setAlarm(secondsMinutesHoursDateMatch, now + TimeSpan(0, 0, 0, 12));
  }  // of if-then an alarm has triggered
}  // of method loop()
