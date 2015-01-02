#include <SPI.h>
#include <Wire.h>
#include "Adafruit_BLE_UART.h"
#include "Adafruit_LEDBackpack.h"
#include "Adafruit_GFX.h"

#define ADAFRUITBLE_REQ 10
#define ADAFRUITBLE_RDY 2
#define ADAFRUITBLE_RST 9
Adafruit_BLE_UART uart = Adafruit_BLE_UART(ADAFRUITBLE_REQ, ADAFRUITBLE_RDY, ADAFRUITBLE_RST);

Adafruit_BicolorMatrix matrix = Adafruit_BicolorMatrix();

void draw(String command)
{
  if (command == "clear") {
    matrix.clear();
  }

  if (command == "countdown") {
    drawCountdown();
  }

  if (command == "red") {
    matrix.clear();
    matrix.fillRect(0,0, 8,8, LED_RED);
  }

  if (command == "green") {
    matrix.clear();
    matrix.fillRect(0,0, 8,8, LED_GREEN);
  }

  if (command == "yellow") {
    matrix.clear();
    matrix.fillRect(0,0, 8,8, LED_YELLOW);
  }

  matrix.writeDisplay();  // write the changes we just made to the display
}

void drawCountdown()
{
  matrix.setTextWrap(false);  // we dont want text to wrap so it scrolls nicely
  matrix.setTextSize(1);
  matrix.setTextColor(LED_GREEN);
  matrix.setRotation(1);

  matrix.clear();
  matrix.setCursor(2,1);
  matrix.print("3");
  matrix.writeDisplay();
  delay(500);

  matrix.clear();
  matrix.setCursor(2,1);
  matrix.print("2");
  matrix.writeDisplay();
  delay(500);

  matrix.clear();
  matrix.setCursor(2,1);
  matrix.print("1");
  matrix.writeDisplay();
  delay(500);
}

void rxCallback(uint8_t *buffer, uint8_t len)
{
  String command = "";
  for(int i=0; i<len; i++) {
   command = String(command + (char)buffer[i]);
  }

  draw(command);

  debugMsg(buffer, len);
  uart.write(buffer, len); // Echo the same data back via BLE
}

void setup(void)
{
  Serial.begin(9600);
  while(!Serial); // Leonardo/Micro should wait for serial init

  matrix.begin(0x70);  // pass in the address

  draw("countdown");

  draw("red");
  delay(250);
  draw("yellow");
  delay(250);
  draw("green");
  delay(250);
  draw("clear");

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
