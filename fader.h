/*
https://docs.m5stack.com/en/unit/fader
*/

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
#define INPUT_MAX 4095
#define INPUT_MIN 0

class Fader : public IUnit
{
private:
  int value = 0;
  int rawADC = 0;
  uint8_t beginHue = 0;
  uint8_t deltaHue = 30;
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
    this->rawADC = analogRead(INPUT_PIN);
    // Mapping ADC value to the brightness range
    this->value = getCenterBrightness();

    // Serial.print("analogRead(KEY_PIN)");
    // Serial.println(this->rawADC);

    FastLED.setBrightness(this->value); // Adjust the brightness of the FADER LED
    FastLED.show();
  }

  virtual int getRaw()
  {
    return this->rawADC;
  }

  virtual int getValue()
  {
    return this->value;
  }

  virtual String getTitle()
  {
    return ("  FADER UNIT TEST\n   Example\n\n  Fader State:");
  }

  int getCenterBrightness()
  {
    int brightness = 0;
    int mid = INPUT_MAX / 2;
    float bufferPercent = 0.12;
    int buffer = static_cast<int>(INPUT_MAX * bufferPercent);
    int val = this->rawADC > mid ? this->rawADC - mid : mid - this->rawADC;

    if (val > buffer)
    {
      brightness = map(val, INPUT_MIN, mid, MIN_BRIGHTNESS, MAX_BRIGHTNESS);
    }

    return brightness;
  }

  int getTotalBrightness()
  {
    return map(this->rawADC, INPUT_MIN, INPUT_MAX, MIN_BRIGHTNESS, MAX_BRIGHTNESS);
  }
};

#endif