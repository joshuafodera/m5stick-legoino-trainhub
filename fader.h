#include <FastLED.h>
#include <Wire.h>
#include <M5StickCPlus.h>
#include ".\unit.h"

#ifndef FADER_H
#define FADER_H

// Setup LEDs on port B
// #define NUM_LEDS 1
#define NUM_LEDS 14
#define MIN_BRIGHTNESS 0
#define MAX_BRIGHTNESS 25
#define INPUT_PIN 33  // Port B input pin
#define OUTPUT_PIN 32 // Port B output pin

class Fader : public IUnit
{
private:
  int value = 0;
  uint8_t beginHue = 0;
  uint8_t deltaHue = 30;
  uint8_t brightness = 100;
  CRGB leds[NUM_LEDS];

public:
  virtual void setup()
  {
    pinMode(INPUT_PIN, INPUT);                             // Init Key pin.
    FastLED.addLeds<NEOPIXEL, OUTPUT_PIN>(leds, NUM_LEDS); // Init LEDs
    delay(1000);
    fill_rainbow(leds, NUM_LEDS, beginHue, deltaHue);
  }

  virtual void loop()
  {
    this->value = analogRead(INPUT_PIN);
    Serial.print("analogRead(KEY_PIN)");
    Serial.println(this->value);

    // Mapping ADC value to the brightness range
    brightness = map(value, 0, 4095, MIN_BRIGHTNESS, MAX_BRIGHTNESS);
    FastLED.setBrightness(brightness); // Adjust the brightness of the FADER LED
    FastLED.show();
    Serial.printf("%d\r\n", value);
    M5.Lcd.setCursor(0, 100, 2);
    M5.Lcd.print(("value: " + String(value) + "    "));
    M5.Lcd.setCursor(0, 120, 2);
    M5.Lcd.print(("brightness: " + String(brightness) + "    "));
  }

  virtual int getValue()
  {
    return this->value;
  }

  virtual String getTitle()
  {
    return ("  FADER UNIT TEST\n   Example\n\n  Fader State:");
  }
};

#endif