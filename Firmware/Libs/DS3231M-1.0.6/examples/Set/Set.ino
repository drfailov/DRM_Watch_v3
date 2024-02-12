/*! @file Set.ino

@section Set_intro_section Description

Example program for using the DS3231M library which allows access to the DS3231M real-time-clock
chip. The library as well as the most current version of this program is available at GitHub using
the address https://github.com/Zanduino/DS3231M and a more detailed description of this program
(and the library) can be found at https://github.com/Zanduino/DS3231M/wiki/Demo.ino \n\n

The DS3231M library uses the standard SPI Wire library for communications with the RTC chip and has
also used the class definitions of the standard RTClib library from Adafruit/Jeelabs. The data sheet
for the DS3231M is located at https://datasheets.maximintegrated.com/en/ds/DS3231M.pdf.

@section Setlicense __**GNU General Public License v3.0**__

This program is free software: you can redistribute it and/or modify it under the terms of the GNU
General Public License as published by the Free Software Foundation, either version 3 of the
License, or (at your option) any later version. This program is distributed in the hope that it will
be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more details. You should have
received a copy of the GNU General Public License along with this program.  If not, see
<http://www.gnu.org/licenses/>.

@section Setauthor Author

 Written by Arnd <Zanshin_Github@sv-zanshin.com> / https://www.github.com/SV-Zanshin

@section Setversions Changelog

Version | Date       | Developer  | Comments
------- | ---------- | ---------- | ---------------------------------------------
1.0.3   | 2019-10-16 | simmunity  | Corrected "inputBuffer" issue
1.0.2   | 2019-02-02 | SV-Zanshin | Issue #7 - convert documentation to Doxygen
1.0.1   | 2019-01-27 | SV-Zanshin | Issue #5. Changes to sscanf() for Esp32
1.0.0   | 2017-08-13 | SV-Zanshin | Initial coding
*/
#include <DS3231M.h>  // Include the DS3231M RTC library
/***************************************************************************************************
** Declare all program constants                                                                  **
***************************************************************************************************/
const uint32_t SERIAL_SPEED{115200};     ///< Set the baud rate for Serial I/O
const uint8_t  SPRINTF_BUFFER_SIZE{32};  ///< Buffer size for sprintf()
const uint8_t  LED_PIN{13};              ///< Built-in Arduino green LED pin

/***************************************************************************************************
** Declare global variables and instantiate classes                                               **
***************************************************************************************************/
DS3231M_Class DS3231M;  ///< Create an instance of the DS3231M class

void setup() {
  /*!
   @brief    Arduino method called once at startup to initialize the system
   @details  This is an Arduino IDE method which is called first upon boot or restart. It is only
             called one time and then control goes to the main "loop()" method, from which control
             never returns
   @return   void
  */
  pinMode(LED_PIN, OUTPUT);  // Make the LED light an output pin
  Serial.begin(SERIAL_SPEED);
#ifdef __AVR_ATmega32U4__  // If this is a 32U4 processor, then wait for serial to initialize
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
  while (!DS3231M.begin())  // Initialize RTC communications
  {
    Serial.println(F("Unable to find DS3231MM. Checking again in 3s."));
    delay(3000);
  }                         // of loop until device is located
  DS3231M.pinSquareWave();  // Make INT/SQW pin toggle at 1Hz
  Serial.println(F("DS3231M initialized."));
  DS3231M.adjust();  // Set to library compile Date/Time
  Serial.print(F("DS3231M chip temperature is "));
  Serial.print(DS3231M.temperature() / 100.0, 1);  // Value is in 100ths of a degree
  Serial.println(
      "\xC2\xB0"
      "C");
  Serial.println(F("\nEnter the following serial command:"));
  Serial.println(F("SETDATE yyyy-mm-dd hh:mm:ss"));
}  // of method setup()

void readCommand() {
  /*!
   @brief    Read incoming data from the Serial port
   @details  This function checks the serial port to see if there has been any input. If there is
             data it is read until a terminator is discovered and then the command is parsed and
             acted upon
   @return   void
  */
  static char    text_buffer[SPRINTF_BUFFER_SIZE];  ///< Buffer for sprintf()/sscanf()
  static uint8_t text_index = 0;                    ///< Variable for buffer position
  while (Serial.available())                        // Loop while there is incoming serial data
  {
    text_buffer[text_index] = Serial.read();  // Get the next byte of data
    // keep on reading until a newline shows up or the buffer is full
    if (text_buffer[text_index] != '\n' && text_index < SPRINTF_BUFFER_SIZE) {
      text_index++;
    } else {
      text_buffer[text_index] = 0;              // Add the termination character
      for (uint8_t i = 0; i < text_index; i++)  // Convert the whole input buffer to uppercase
      {
        text_buffer[i] = toupper(text_buffer[i]);
      }  // for-next all characters in buffer
      Serial.print(F("\nCommand \""));
      Serial.write(text_buffer);
      Serial.print(F("\" received.\n"));
      /*********************************************************************************************
      ** Parse the single-line command and perform the appropriate action. The current list of    **
      ** commands understood are as follows:                                                      **
      **                                                                                          **
      ** SETDATE      - Set the device time                                                       **
      **                                                                                          **
      *********************************************************************************************/
      enum commands { SetDate, Unknown_Command };  // enumerate all commands
      commands command;                            // declare enumerated type
      char     workBuffer[SPRINTF_BUFFER_SIZE];    // Buffer to hold string compare
      sscanf(text_buffer, "%s %*s", workBuffer);   // Parse the string for first word
      if (!strcmp(workBuffer, "SETDATE")) {
        command = SetDate;  // Set command number when found
      } else {
        command = Unknown_Command;  // Otherwise set to not found
      }                             // if-then-else a known command
      unsigned int tokens, year, month, day, hour, minute,
          second;  // Variables to hold parsed date/time
      switch (command) {
        /*********************************
        ** Set the device time and date **
        *********************************/
        case SetDate:
          // Use sscanf() to parse the date/time into component variables
          tokens = sscanf(text_buffer, "%*s %u-%u-%u %u:%u:%u;", &year, &month, &day, &hour,
                          &minute, &second);
          if (tokens != 6)  // Check to see if it was parsed correctly
          {
            Serial.print(F("Unable to parse date/time\n"));
          } else {
            DS3231M.adjust(
                DateTime(year, month, day, hour, minute, second));  // Adjust the RTC date/time
            Serial.print(F("Date has been set."));
          }  // of if-then-else the date could be parsed
          break;
        /********************
        ** Unknown command **
        ********************/
        case Unknown_Command:  // Show options on bad command
        default:
          Serial.println(F("Unknown command. Valid commands are:"));
          Serial.println(F("SETDATE yyyy-mm-dd hh:mm:ss"));
      }                // of switch statement to execute commands
      text_index = 0;  // reset the counter
    }                  // of if-then-else we've received full command
  }                    // of if-then there is something in our input buffer
}  // of method readCommand

void loop() {
  /*!
   @brief    Arduino method for the main program loop
   @details  This is the main program for the Arduino IDE, it is an infinite loop and keeps on
             repeating
   @return   void
  */
  static uint8_t secs;
  DateTime       now = DS3231M.now();  // get the current time from device
  if (secs != now.second())            // Output if seconds have changed
  {
    // Use sprintf() to pretty print the date/time with leading zeros
    char output_buffer[SPRINTF_BUFFER_SIZE];  ///< Temporary buffer for sprintf()
    sprintf(output_buffer, "%04d-%02d-%02d %02d:%02d:%02d", now.year(), now.month(), now.day(),
            now.hour(), now.minute(), now.second());
    Serial.println(output_buffer);
    secs = now.second();  // Set the counter variable
  }                       // of if the seconds have changed
  readCommand();          // See if serial port has incoming data
}  // of method loop()
