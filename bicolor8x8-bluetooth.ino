#include <SPI.h>
#include "Adafruit_BLE_UART.h"

#define ADAFRUITBLE_REQ 10
#define ADAFRUITBLE_RDY 2
#define ADAFRUITBLE_RST 9

Adafruit_BLE_UART uart = Adafruit_BLE_UART(ADAFRUITBLE_REQ, ADAFRUITBLE_RDY, ADAFRUITBLE_RST);

void rxCallback(uint8_t *buffer, uint8_t len)
{
  String result = "";
  for(int i=0; i<len; i++) {
   result = String(result + (char)buffer[i]);
  }

  if (result == "red") {
    Serial.print("HI THERE");
  }

  debugMsg(buffer, len);
  uart.write(buffer, len); // Echo the same data back via BLE
}

void setup(void)
{
  Serial.begin(9600);
  while(!Serial); // Leonardo/Micro should wait for serial init

  uart.setRXcallback(rxCallback);
  uart.setDeviceName("BICOLOR"); /* 7 characters max! */
  uart.begin();
}

void loop()
{
  uart.pollACI();
}

void debugMsg(uint8_t *buffer, uint8_t len)
{
  Serial.print(F("Received "));
  Serial.print(len);
  Serial.print(F(" bytes: "));
  for(int i=0; i<len; i++) {
   Serial.print((char)buffer[i]);
  }
  Serial.print(F(" ["));

  for(int i=0; i<len; i++)
  {
    Serial.print(" 0x"); Serial.print((char)buffer[i], HEX);
  }
  Serial.println(F(" ]")); 
}

