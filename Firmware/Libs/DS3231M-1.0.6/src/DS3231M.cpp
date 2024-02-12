/*!
 @file DS3231M.cpp
 @section DS3231Mcpp_intro_section Description
 Arduino Library to access the Maxim DS3231M Real Time Clock\n\n
 See main library header file for details
*/
#include "DS3231M.h"  // Include the header definition
const uint8_t daysInMonth[] PROGMEM = {31, 28, 31, 30, 31, 30,
                                       31, 31, 30, 31, 30, 31};  ///< Numbers of days in each month

static uint16_t date2days(uint16_t y, uint8_t m, uint8_t d) {
  /*!
   @brief     returns the number of days from a given Y M D value
   @param[in] y Years
   @param[in] m Months
   @param[in] d Days
   @return    Number of days from a given Y/M/D value
  */
  if (y >= 2000) {
    y -= 2000;        // Remove year offset
  }                   // if-then year is past 2000
  uint16_t days = d;  // Store numbers of days
  for (uint8_t i = 1; i < m; ++i) {
    days += pgm_read_byte(daysInMonth + i - 1);  // Add number of days for each month
  }                                              // for-next each month
  if (m > 2 && y % 4 == 0) {
    ++days;                                 // Deal with leap years
  }                                         // if-then leap year
  return days + 365 * y + (y + 3) / 4 - 1;  // Return computed value
}  // of method date2days
static long time2long(uint16_t days, uint8_t h, uint8_t m, uint8_t s) {
  /*!
   @brief     returns the number of seconds from a given D H M S value
   @param[in] d Days
   @param[in] h Hours
   @param[in] m Minutes
   @param[in] s Seconds
   @return    Number of seconds from a given Day/Hour/Minute/Second value
  */
  return ((days * 24L + h) * 60 + m) * 60 + s;
}  // of method time2long()
static uint8_t conv2d(const char* p) {
  /*!
   @brief     converts character string representation to decimal
   @param[in] p Input character array with length of 2 bytes
   @return    Integer representation of value in array
  */
  uint8_t v = 0;
  if ('0' <= *p && *p <= '9') {
    v = *p - '0';              // set to integer value
  }                            // if-then character is in range
  return 10 * v + *++p - '0';  // shift MSB character over left and add 2nd character
}  // of method conv2d
DateTime::DateTime(uint32_t t) {
  /*!
   @brief   Class Constructor (Overloaded)
   @details Class Constructor for DateTime instantiates the class. This is an overloaded class
            constructor so there are multiple definitions. This implementation ignores time
            zones and DST changes. It also ignores leap seconds, see
            http://en.wikipedia.org/wiki/Leap_second for details
   @param[in] t Input time in seconds
  */
  t -= SECONDS_FROM_1970_TO_2000;  // bring to 2000 timestamp from 1970
  ss = t % 60;
  t /= 60;
  mm = t % 60;
  t /= 60;
  hh            = t % 24;
  uint16_t days = t / 24;
  uint8_t  leap;
  for (yOff = 0;; ++yOff) {
    leap = yOff % 4 == 0;
    if (days < (uint16_t)365 + leap) {
      break;
    }  // if-then not a leap
    days -= 365 + leap;
  }  // of for-next each year
  for (m = 1;; ++m) {
    uint8_t daysPerMonth = pgm_read_byte(daysInMonth + m - 1);
    if (leap && m == 2) {
      ++daysPerMonth;
    }  // if-then
    if (days < daysPerMonth) {
      break;
    }  // if-then not a leap
    days -= daysPerMonth;
  }  // of for-next each month
  d = days + 1;
}  // of method DateTime()
DateTime::DateTime(uint16_t year, uint8_t month, uint8_t day, uint8_t hour, uint8_t min,
                   uint8_t sec) {
  /*!
   @brief     Class Constructor (Overloaded)
   @details   Class Constructor for DateTime instantiates the class. This is an overloaded class
              constructor so there are multiple definitions. This implementation ignores time zones
              and DST changes. It also ignores leapseconds, see
              http://en.wikipedia.org/wiki/Leap_second for details
   @param[in] year Year
   @param[in] month Month
   @param[in] day Day
   @param[in] hour Hour
   @param[in] min Minute
   @param[in] sec Second

  */
  if (year >= 2000) {
    year -= 2000;
  }  // if-then year past 2000
  yOff = year;
  m    = month;
  d    = day;
  hh   = hour;
  mm   = min;
  ss   = sec;
}  // of method DateTime()
/*!
 @brief     Class Constructor (Overloaded)
 @details   Class Constructor for DateTime instantiates the class. This is an overloaded class
            constructor so there are multiple definitions. This implementation ignores time zones
            and DST changes. It also ignores leapseconds, see
            http://en.wikipedia.org/wiki/Leap_second for details
 @param[in] copy DateTime class to instantiate class with
*/
DateTime::DateTime(const DateTime& copy)
    : yOff(copy.yOff), m(copy.m), d(copy.d), hh(copy.hh), mm(copy.mm), ss(copy.ss) {}
DateTime::DateTime(const char* date, const char* time) {
  /*!
   @brief     Class Constructor (Overloaded)
   @details   Class Constructor for DateTime instantiates the class. This is an overloaded class
              constructor so there are multiple definitions. This implementation ignores time zones
              and DST changes. It also ignores leapseconds, see
              http://en.wikipedia.org/wiki/Leap_second for details. This version uses a string
              representation of the date and time to instantiate. The date and time arrive in string
              format as follows: date = "Dec 26 2009", time = "12:34:56"
   @param[in] date Character buffer with year value in MMM DD YYYY format
   @param[in] time Character buffer with time value in HH:MM:SS format
  */
  yOff = conv2d(date + 9);  // Compute the year offset
  switch (date[0]) {
    case 'J':
      m = (date[1] == 'a') ? 1 : ((date[2] == 'n') ? 6 : 7);
      break;  // Jan Jun Jul
    case 'F':
      m = 2;
      break;  // Feb
    case 'A':
      m = date[2] == 'r' ? 4 : 8;
      break;  // Apr Aug
    case 'M':
      m = date[2] == 'r' ? 3 : 5;
      break;  // May
    case 'S':
      m = 9;
      break;  // Sep
    case 'O':
      m = 10;
      break;  // Oct
    case 'N':
      m = 11;
      break;  // Nov
    case 'D':
      m = 12;
      break;  // Dec
  }           // of switch for the month
  d  = conv2d(date + 4);
  hh = conv2d(time);
  mm = conv2d(time + 3);
  ss = conv2d(time + 6);
}  // of method DateTime()
DateTime::DateTime(const __FlashStringHelper* date, const __FlashStringHelper* time) {
  /*!
   brief     Class Constructor (Overloaded)
   details   Class Constructor for DateTime instantiates the class. This is an overloaded class
             constructor so there are multiple definitions. This implementation ignores time zones
             and DST changes. It also ignores leapseconds, see
             http://en.wikipedia.org/wiki/Leap_second for details. This version uses the compiled
             in Date and Time
  @param[in] date Character buffer with year value in MMM DD YYYY format
  @param[in] time Character buffer with time value in HH:MM:SS format
  */
  char ybuff[11], tbuff[9];
  memcpy_P(ybuff, date, 11);  // copy flash string to memory
  memcpy_P(tbuff, time, 9);   // copy flash string to memory
  DateTime(ybuff, tbuff);     // Use the string version to instantiate
}  // of method DateTime()
uint8_t DateTime::dayOfTheWeek() const {
  /*!
   @brief   Return the day-of-week
   @details Monday = 1, Sunday = 7
   @return  DOW with Monday-Sunday 1-7
  */
  uint16_t day = date2days(yOff, m, d);  // compute the number of days
  return (day + 6) % 7;                  // Jan 1, 2000 is a Saturday, i.e. 6
}  // of method dayOfTheWeek()
uint32_t DateTime::unixtime(void) const {
  /*!
   @brief   return the UNIX time, which is seconds since 1970-01-01 00:00:00
   @return  UNIX Time (seconds since 1970-01-01 00:00:00)
  */
  uint32_t t;                                   // Declare return variable
  uint16_t days = date2days(yOff, m, d);        // Compute days
  t             = time2long(days, hh, mm, ss);  // Compute seconds
  t += SECONDS_FROM_1970_TO_2000;               // Add seconds from 1970 to 2000
  return t;
}  // of method unixtime()
long DateTime::secondstime(void) const {
  /*!
   @brief   return the time in seconds since 2000-01-01 00:00:00
   @return  Seconds since 2000-01-01 00:00:00
  */
  long     t;
  uint16_t days = date2days(yOff, m, d);
  t             = time2long(days, hh, mm, ss);
  return t;
}  // of method secondstime()
DateTime DateTime::operator+(const TimeSpan& span) {
  /*!
   @brief     Overloaded addition function definition
   @param[in] span TimeSpan to add
   @return    new DateTime value
  */
  return DateTime(unixtime() + span.totalseconds());
}  // of overloaded + function
DateTime DateTime::operator-(const TimeSpan& span) {
  /*!
   @brief     Overloaded subtraction function definition
   @param[in] span TimeSpan to subtract
   @return    new DateTime value
  */
  return DateTime(unixtime() - span.totalseconds());
}  // of overloaded - function
TimeSpan DateTime::operator-(const DateTime& right) {
  /*!
   @brief     Overloaded subtraction function definition
   @param[in] right DateTime to subtract
   @return    new DateTime value
  */
  return TimeSpan(unixtime() - right.unixtime());
}  // of overloaded - function
/*!
 @brief     TimeSpan class constructor (Overloaded)
 @param[in] seconds Number of seconds
*/
TimeSpan::TimeSpan(int32_t seconds) : _seconds(seconds) {}
/*!
 @brief     TimeSpan class constructor (Overloaded)
 @param[in] days Number of Days
 @param[in] hours Number of Hours
 @param[in] minutes Number of Minutes
 @param[in] seconds Number of Seconds
*/
TimeSpan::TimeSpan(int16_t days, int8_t hours, int8_t minutes, int8_t seconds)
    : _seconds((int32_t)days * 86400L + (int32_t)hours * 3600 + (int32_t)minutes * 60 + seconds) {}
/*!
 @brief     TimeSpan class constructor (Overloaded)
 @param[in] copy TimeSpan to use to instantiate class
*/
TimeSpan::TimeSpan(const TimeSpan& copy) : _seconds(copy._seconds) {}
TimeSpan TimeSpan::operator+(const TimeSpan& right) {
  /*!
   @brief     TimeSpan class addition (Overloaded)
   @param[in] right TimeSpan to use to add to value
   @return    Sum of two TimeSpans
  */
  return TimeSpan(_seconds + right._seconds);
}  // of overloaded add
TimeSpan TimeSpan::operator-(const TimeSpan& right) {
  /*!
   @brief     TimeSpan class subtraction (Overloaded)
   @param[in] right TimeSpan to use to subtract from value
   @return    Difference value
  */
  return TimeSpan(_seconds - right._seconds);
}  // of overloaded subtract
DS3231M_Class::DS3231M_Class() {}   ///< Unused class constructor
DS3231M_Class::~DS3231M_Class() {}  ///< Unused class destructor
bool DS3231M_Class::begin(const uint32_t i2cSpeed) {
  /*!
   @brief     starts I2C communications with the device
   @param[in] i2cSpeed I2C Speed to use for communications
   @return    Boolean status of the initialization
  */
  Wire.begin();                                // Start I2C as master device
  Wire.setClock(i2cSpeed);                     // Set I2C clock speed
  Wire.beginTransmission(DS3231M_ADDRESS);     // Address the DS3231M
  uint8_t errorCode = Wire.endTransmission();  // See if there's a device present
  if (errorCode == 0)                          // If we have a DS3231M
  {
    writeByte(DS3231M_RTCHOUR, readByte(DS3231M_RTCHOUR) &
                                   B10111111);  // Force use of 24 hour clock by turning off bit
  }                                             // of if-then device detected
  else {
    return false;
  }
  return true;
}  // of method begin()
uint8_t DS3231M_Class::readByte(const uint8_t addr) {
  /*!
   @brief     reads 1 byte from the specified I2C memory address
   @param[in] addr Memory/Register address
   @return    byte read from specified address
  */
  Wire.beginTransmission(DS3231M_ADDRESS);        // Address the I2C device
  Wire.write(addr);                               // Send the register address to read
  _TransmissionStatus = Wire.endTransmission();   // Close transmission
  Wire.requestFrom(DS3231M_ADDRESS, (uint8_t)1);  // Request 1 byte of data
  return Wire.read();                             // read it and return it
}  // of method readByte()
void DS3231M_Class::writeByte(const uint8_t addr, const uint8_t data) {
  /*!
   @brief     writes 1 byte to the specified I2C memory address
   @param[in] addr Memory/Register address
   @param[in] data Byte to write
  */
  Wire.beginTransmission(DS3231M_ADDRESS);       // Address the I2C device
  Wire.write(addr);                              // Send the register address to write
  Wire.write(data);                              // Send the data to write
  _TransmissionStatus = Wire.endTransmission();  // Close transmission
}  // of method writeByte()
uint8_t DS3231M_Class::bcd2int(const uint8_t bcd) {
  /*!
   @brief     converts a BCD encoded value into number representation
   @details   Takes a byte of BCD (Binard coded Decimal) and converts to the binary integer
              representation
   @param[in] bcd Binary-Coded-Decimal source byte
   @return    Unsigned integer value
  */
  return ((bcd / 16 * 10) + (bcd % 16));
}  // of method bcd2int
uint8_t DS3231M_Class::int2bcd(const uint8_t dec) {
  /*!
   @brief     converts a byte to BCD
   @details   Takes an unsigned byte and converts to BCD (Binard coded Decimal)
   @param[in] dec Unsigned integer value
   @return    Binary-Coded-Decimal value
  */
  return ((dec / 10 * 16) + (dec % 10));
}  // of method int2bcd
void DS3231M_Class::adjust() {
  /*!
   @brief     sets the current date/time (Overloaded)
   @details   This is an overloaded function, if called with no parameters then the RTC is set to
              the date/time when the program was compiled and uploaded. Otherwise the values are
              set, but the oscillator is stopped during the process and needs to be restarted upon
              completion
  */
  adjust(DateTime(F(__DATE__), F(__TIME__)));  // Set to compile time
}  // of method adjust
void DS3231M_Class::adjust(const DateTime& dt) {
  /*!
   @brief     sets the current date/time (Overloaded)
   @details   This is an overloaded function, if called with no parameters then the RTC is set to
              the date/time when the program was compiled and uploaded. Otherwise the values are
              set, but the oscillator is stopped during the process and needs to be restarted upon
              completion
   @param[in] dt DateTime value to set the clock to
  */
  writeByte(DS3231M_RTCSEC, int2bcd(dt.second() % 60));   // Write seconds, keep device off
  writeByte(DS3231M_RTCMIN, int2bcd(dt.minute() % 60));   // Write the minutes value
  writeByte(DS3231M_RTCHOUR, int2bcd(dt.hour() % 24));    // Also resets the 24Hour clock on
  writeByte(DS3231M_RTCWKDAY, dt.dayOfTheWeek());         // Update the weekday
  writeByte(DS3231M_RTCDATE, int2bcd(dt.day()));          // Write the day of month
  writeByte(DS3231M_RTCMTH, int2bcd(dt.month()));         // Month, ignore century bit
  writeByte(DS3231M_RTCYEAR, int2bcd(dt.year() - 2000));  // Write the year
  writeByte(DS3231M_STATUS, readByte(DS3231M_STATUS) & B01111111);    // Unset OSC flag bit if set
  writeByte(DS3231M_CONTROL, readByte(DS3231M_CONTROL) & B01111111);  // Unset EOSC flag bit if set
  _SetUnixTime = now().unixtime();                                    // Store time of last change
}  // of method adjust
DateTime DS3231M_Class::now() {
  /*!
   @brief     returns the current date/time
   @return    Current Date/Time
  */
  Wire.beginTransmission(DS3231M_ADDRESS);        // Address the I2C device
  Wire.write(DS3231M_RTCSEC);                     // Start at specified register
  _TransmissionStatus = Wire.endTransmission();   // Close transmission
  Wire.requestFrom(DS3231M_ADDRESS, (uint8_t)7);  // Request 7 bytes of data
  if (Wire.available() == 7)                      // Wait until the data is ready
  {
    _ss = bcd2int(Wire.read() & 0x7F);         // Mask high bit in seconds
    _mm = bcd2int(Wire.read() & 0x7F);         // Mask high bit in minutes
    _hh = bcd2int(Wire.read() & 0x3F);         // Mask 2 high bits in hours and clamp to 0-23
    Wire.read();                               // Read and ignore Day-Of-Week register
    _d = bcd2int(Wire.read() & 0x3F);          // Mask 2 high bits for day of month
    _m = bcd2int(Wire.read() & 0x1F);          // Mask 3 high bits for Month
    _y = bcd2int(Wire.read()) + 2000;          // Add 2000 to internal year
  }                                            // of if-then there is data to be read
  return DateTime(_y, _m, _d, _hh, _mm, _ss);  // Return class value
}  // of method now()
int32_t DS3231M_Class::temperature() {
  /*!
   @brief     returns the internal temperature in tenths of degrees Celsius
   @details   Using 1/10 of degrees allows use of integer rather than floating point
   @return    temperature in 1/10 degrees Celsius
  */
  int32_t temp = readByte(DS3231M_TEMPERATURE) << 8;  // read MSB
  temp |= readByte(DS3231M_TEMPERATURE + 1);          // get LSB
  temp = temp >> 6;                                   // Shift over 6 bits
  if (temp & 0x200) {
    temp |= 0xFC00;       // keep negative by setting bits
  }                       // if-then a negative number
  temp = temp * 100 / 4;  // value is in .25C increments
  return temp;            // Return computed temperature
}  // of method temperature()
bool DS3231M_Class::isStopped() {
  /*!
   @brief  Check to see if the oscillator is running
   @return return false if the oscillator is running, otherwise return true
  */
  return (readByte(DS3231M_STATUS) >> 7);  // shift OSF register bit to LSB for true/false
}  // of method isAlarm()
void DS3231M_Class::setAlarm(const uint8_t alarmType, const DateTime dt, const bool state) {
  /*!
   @brief     will set one of the 2 alarms
   @details   As the two alarms aren't identical in what will trigger them, this call chooses which
              alarm to set depending upon the "alarmType" parameter value.
   @param[in] alarmType Alarm Type, see the alarm type enumerated values
   @param[in] dt Date/Time to set the alarm to
   @param[in] state Alarm Enable state
  */
  if (alarmType >= UnknownAlarm) {
    return;                     // Don't do anything if out-of-range
  }                             // if-then out of range alarm type
  if (alarmType < everyMinute)  // These types go to alarm 1
  {
    writeByte(DS3231M_ALM1SEC, int2bcd(dt.second()));  // Set seconds value
    writeByte(DS3231M_ALM1MIN, int2bcd(dt.minute()));  // Set minutes value
    writeByte(DS3231M_ALM1HOUR, int2bcd(dt.hour()));   // Set hours value
    if (alarmType == secondsMinutesHoursDateMatch)     // Set either day of month or day
    {
      writeByte(DS3231M_ALM1DATE, int2bcd(dt.day()));
    } else {
      writeByte(DS3231M_ALM1DATE, int2bcd(dt.dayOfTheWeek()));
    }                                              // if-then-else DateMathc
    if (alarmType < secondsMinutesHoursDateMatch)  // Set the high-bit of ALM1DATE if the alarm bit
                                                   // needs setting
      writeByte(DS3231M_ALM1DATE, readByte(DS3231M_ALM1DATE) | 0x80);
    if (alarmType <
        secondsMinutesHoursMatch)  // Set the high-bit of ALM1HOUR if the alarm bit needs setting
      writeByte(DS3231M_ALM1HOUR, readByte(DS3231M_ALM1HOUR) | 0x80);
    if (alarmType <
        secondsMinutesMatch)  // Set the high-bit of ALM1MIN if the alarm bit needs setting
      writeByte(DS3231M_ALM1MIN, readByte(DS3231M_ALM1MIN) | 0x80);
    if (alarmType == everySecond)  // Set the high-bit of ALM1SEC if the alarm bit needs setting
      writeByte(DS3231M_ALM1SEC, readByte(DS3231M_ALM1SEC) | 0x80);
    if (alarmType == secondsMinutesHoursDayMatch)  // Set bit 7 if the alarm bit needs setting
      writeByte(DS3231M_ALM1DATE, readByte(DS3231M_ALM1DATE) | 0x40);
    if (state)
      writeByte(DS3231M_CONTROL, readByte(DS3231M_CONTROL) | 1);  // Set A1IE enable to on
    else
      writeByte(DS3231M_CONTROL, readByte(DS3231M_CONTROL) & 0xFE);  // Set A1IE enable to off
  } else {
    writeByte(DS3231M_ALM2MIN, int2bcd(dt.minute()));  // Set minutes value
    writeByte(DS3231M_ALM2HOUR, int2bcd(dt.hour()));   // Set hours value
    if (alarmType == minutesHoursDateMatch)            // If we do a min-hh-date match
      writeByte(DS3231M_ALM2DATE, int2bcd(dt.day()));  // Set day of month value
    else if (alarmType == minutesHoursDayMatch)        // if we have a min-hh-dow match
      writeByte(DS3231M_ALM2DATE,
                int2bcd(dt.dayOfTheWeek() | 0x80));  // Set day of week value and switch
    if (alarmType < minutesHoursDateMatch)
      writeByte(DS3231M_ALM2DATE,
                readByte(DS3231M_ALM2DATE) | 0x80);  // the alarm bit needs setting
    if (alarmType < minutesHoursMatch)
      writeByte(DS3231M_ALM2HOUR,
                readByte(DS3231M_ALM2HOUR) | 0x80);  // the alarm bit needs setting
    if (alarmType == everyMinute)
      writeByte(DS3231M_ALM2MIN, readByte(DS3231M_ALM2MIN) | 0x80);  // the alarm bit needs setting
    if (state)
      writeByte(DS3231M_CONTROL, readByte(DS3231M_CONTROL) | 2);  // Set A2IE enable to on
    else
      writeByte(DS3231M_CONTROL, readByte(DS3231M_CONTROL) & 0xFD);  // Set A2IE enable to off
  }              // of if-then-else use alarm 1 or 2
  clearAlarm();  // Clear the alarm state
  return;
}  // of method setAlarm
bool DS3231M_Class::isAlarm() {
  /*!
   @brief  return whether either of the two alarms has been triggered
   @return true if either of the 2 alarms is triggered, otherwise false
  */
  return (readByte(DS3231M_STATUS) & 3);  // Alarm if either of 2 LSBits set
}  // of method isAlarm()
void DS3231M_Class::clearAlarm() {
  /*!
   @brief  Clear a set alarm by re-writing the same contents back to the register
  */
  writeByte(DS3231M_STATUS, readByte(DS3231M_STATUS) & 0xFC);  // Alarm if either of 2 LSBits set
}  // of method clearAlarm()
void DS3231M_Class::kHz32(const bool state) {
  /*!
   @brief     Turns the 32.768kHz output on the 32Khz pin on or off
   @param[in] state "True" to turn the output on, otherwise turn off
  */
  writeByte(DS3231M_STATUS,
            (readByte(DS3231M_STATUS) & 0xF7) | (state << 3));  // Set bit 3 to state
}  // of method kHz32()
int8_t DS3231M_Class::getAgingOffset() {
  /*!
   @brief  return the aging offset value from the DS3231M
   @return signed integer value for the aging offset
  */
  return (readByte(DS3231M_AGING));
}  // of method getAgingOffset()
int8_t DS3231M_Class::setAgingOffset(const int8_t agingOffset) {
  /*!
   @brief     set the aging offset as a signed integer.
   @details   Each value is 0.12ppm and positive values slows the time base
   @param[in] agingOffset New aging offset
   @return    New value read from register after setting
  */
  writeByte(DS3231M_AGING, agingOffset);
  return (readByte(DS3231M_AGING));
}  // of method setAgingOffset()
uint8_t DS3231M_Class::weekdayRead() {
  /*!
   @brief   Reads the weekday number from the RTC
   @details This number is user-settable and is incremented when the day shifts. It is set as part
            of the adjust() method where Monday is weekday 1
   @return  Current weekday, Monday=1 and Sunday=7
  */
  uint8_t dow = readByte(DS3231M_RTCWKDAY) & B00000111;  // no need to convert, values 1-7
  return dow;
}  // of method weekdayRead()
uint8_t DS3231M_Class::weekdayWrite(const uint8_t dow) {
  /*!
   @brief     Writes the weekday (1-7) to the register and returns the new setting
   @details   Values are 1-7
   @param[in] dow Day of Week to write
   @return    returns the dow input value unless out-of-range
  */
  uint8_t retval = 0;      // assume we have an error
  if (dow > 0 && dow < 8)  // If parameter is in range, then set the register
  {
    writeByte(DS3231M_RTCWKDAY, dow);
    retval = dow;
  }  // of if-then we have a good DOW
  return retval;
}  // of method weekdayWrite()
void DS3231M_Class::pinAlarm() {
  /*!
   @brief     Sets the control register flag to make the INT/SQW Pin get pulled up on an alarm
  */
  writeByte(DS3231M_CONTROL, readByte(DS3231M_CONTROL) | 0x4);  // Set bit 3 to on
}  // of method pinAlarm()
void DS3231M_Class::pinSquareWave() {
  /*!
   @brief     Sets the control register flag to make the INT/SQW Pin produce a 1Hz signal
  */
  writeByte(DS3231M_CONTROL, readByte(DS3231M_CONTROL) & ~0x4);
}  // of method pinSquareWave()
