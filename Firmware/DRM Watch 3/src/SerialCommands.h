#include <Arduino.h>
#include <Lcd.h>

const byte numChars = 32;
char receivedChars[numChars];
boolean newData = false;

void processCommand()
{
  if (newData == true)
  {
    // if (strstr(receivedChars, "max") == receivedChars) // starts with example
    // {
    //   voltage_max = atof(receivedChars + 3);
    //   EEPROM.put(18, voltage_max);
    //   Serial.print(F("voltage_max = "));
    //   Serial.println(voltage_max);
    // }
    if (strcmp(receivedChars, "screenshot") == 0)
    {
       lcd()->writeBufferXBM2(Serial);
       Serial.flush();
    }
    else
    {
      Serial.print(F("Received: "));
      Serial.println(receivedChars);
      drawMessageAnimated(String(receivedChars));
      Serial.println(F("-- Help begin: --"));
      Serial.println(F("<screenshot> - Send XBM screenshot"));
      Serial.println(F("-- Help end. --"));
    }
    newData = false;
  }
}

void recvWithStartEndMarkers()
{
  static boolean recvInProgress = false;
  static byte ndx = 0;
  char startMarker = '<';
  char endMarker = '>';
  char rc;
  Serial.setTimeout(500);
  while (Serial.available() > 0 && newData == false)
  {
    rc = Serial.read();

    if (recvInProgress == true)
    {
      if (rc != endMarker)
      {
        receivedChars[ndx] = rc;
        ndx++;
        if (ndx >= numChars)
        {
          ndx = numChars - 1;
        }
      }
      else
      {
        receivedChars[ndx] = '\0'; // terminate the string
        recvInProgress = false;
        ndx = 0;
        newData = true;
      }
    }

    else if (rc == startMarker)
    {
      recvInProgress = true;
    }
  }
}

void serialLoop(){
  recvWithStartEndMarkers();
  processCommand();
}