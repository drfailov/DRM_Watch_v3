#include <U8g2lib.h>

//U8G2_LS027B7DH01_M0_400X240_x_4W_HW_SPI 
//U8G2_LS027B7DH01_400X240_1_4W_HW_SPI u8g2(U8G2_R0, /* cs=*/ 10, /* dc=*/ U8X8_PIN_NONE, /* reset=*/ 8);	// there is no DC line for this display 
//U8G2_LS027B7DH01_400X240_F_4W_HW_SPI u8g2(U8G2_R0, /* cs=*/ 10, /* dc=*/ U8X8_PIN_NONE, /* reset=*/ 8);	// there is no DC line for this display, SPI Mode 2
//U8G2_LS013B7DH03_128X128_F_4W_HW_SPI u8g2(U8G2_R0, /* cs=*/ 10, /* dc=*/ U8X8_PIN_NONE, /* reset=*/ 8);	// there is no DC line for this display
//U8G2_LS027B7DH01_400X240_F_4W_HW_SPI u8g2(U8G2_R0, /* cs=*/ 10, /* dc=*/ U8X8_PIN_NONE, /* reset=*/ 8);	// there is no DC line for this display, SPI Mode 2
//U8G2_LS027B7DH01_400X240_1_4W_HW_SPI  u8g2(U8G2_R0, /* cs=*/ 10, /* dc=*/ U8X8_PIN_NONE, /* reset=*/ 8);
//U8G2_LS027B7DH01_M0_400X240_F_4W_HW_SPI u8g2(U8G2_R0, /* cs=*/ 10, /* dc=*/ U8X8_PIN_NONE, /* reset=*/ 8);	// there is no DC line for this display, SPI Mode 0
U8G2_LS013B7DH05_144X168_F_4W_HW_SPI u8g2(U8G2_R0, /* cs=*/ 10, /* dc=*/ U8X8_PIN_NONE, /* reset=*/ 8);	// there is no DC line for this display


void setup() {
  // put your setup code here, to run once:
  pinMode(15, OUTPUT);
  //pinMode(10, OUTPUT);
  //digitalWrite(10, HIGH);
  Serial.begin(115200);
}

void loop() {
  // put your main code here, to run repeatedly:
  Serial.print("Boot=");
  Serial.println(digitalRead(0));
  digitalWrite(15, HIGH);
  delay(100);
  digitalWrite(15, LOW);
  delay(500);
}
