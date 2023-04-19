/*
*******************************************************************************
https://docs.m5stack.com/en/unit/fader
https://docs.m5stack.com/en/unit/key
https://docs.m5stack.com/en/unit/pahub

https://github.com/m5stack/M5StickC-Plus
https://github.com/corneliusmunz/legoino
*/

#include <Lpf2Hub.h>
#include <FastLED.h>
#include <Wire.h>
#include <M5StickCPlus.h>
#include ".\unit.h"
#include ".\fader.h"
#include ".\key.h"

#undef RED   // undefine the problematic #define
#undef GREEN // undefine the problematic #define

#define DEVICE_NAME "Christmas" //"Technic Hub"

// create an instance of LED class
String state = "KEY";
IUnit *unit;

void setupLCD(String text)
{
  M5.Lcd.setCursor(0, 25, 2);
  M5.Lcd.print(text);
}

void setup()
{
  M5.begin();
  if (state == "KEY")
    unit = new Key();
  else if (state == "FADER")
    unit = new Fader();

  unit->setup();
  setupLCD(unit->getTitle());
}

void loop()
{
  unit->loop();
  delay(100);
}
