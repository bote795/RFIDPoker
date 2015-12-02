
/*****************************
     RFID-powered lockbox

This sketch will move a servo when
a trusted tag is read with the 
ID-12/ID-20 RFID module

Pinout for SparkFun RFID USB Reader
Arduino ----- RFID module
5V            VCC
GND           GND
D2            TX

Pinout for SparkFun RFID Breakout Board
Arduino ----- RFID module
5V            VCC
GND           GND
D2            D0

Connect the servo's power, ground, and
signal pins to VCC, GND,
and Arduino D9

If using the breakout, you can also 
put an LED & 330 ohm resistor between 
the RFID module's READ pin and GND for 
a "card successfully read" indication

by acavis, 3/31/2015


 LCD RS pin to digital pin 12
 * LCD Enable pin to digital pin 11
 * LCD D4 pin to digital pin 5
 * LCD D5 pin to digital pin 4
 * LCD D6 pin to digital pin 3
 * LCD D7 pin to digital pin 2
 * LCD R/W pin to ground
 
Inspired by & partially adapted from
http://bildr.org/2011/02/rfid-arduino/

******************************/

#include <SoftwareSerial.h>
#include <LiquidCrystal.h>
#include <Key.h>
#include <Keypad.h>
#include <math.h>
#include "texasHoldem.h"

void setup() {
  // Starts the hardware and software serial ports
   Serial.begin(9600);
   rSerial.begin(9600);
  // set up the LCD's number of columns and rows:
  lcd.begin(20, 4);
  // Print a message to the LCD.
  // lcd.print("hello, world");
  // Serial.println("Booting up");
  // Serial.println("Welcome to Texas Holdem");
  writeToLCD("Welcome to",0);
  writeToLCD("Texas Holdem",1);
  delay(2000);
  initializeGame();

}

void loop() {
  
}
  

