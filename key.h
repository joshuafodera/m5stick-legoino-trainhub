/*
https://docs.m5stack.com/en/unit/key
*/
#include <FastLED.h>
#include <Wire.h>
#include <M5StickCPlus.h>
#include ".\unit.h"

#ifndef KEY_H
#define KEY_H

#define NUM_LEDS 1
#define MIN_BRIGHTNESS 0
#define MAX_BRIGHTNESS 25
#define INPUT_PIN 33  // Port B input pin
#define OUTPUT_PIN 32 // Port B output pin

class Key : public IUnit
{
private:
  int value = 0;
  uint8_t ledColor = 0;
  CRGB leds[NUM_LEDS];

public:
  virtual void setup()
  {
    pinMode(INPUT_PIN, INPUT_PULLUP);                  // Init Key pin.
    FastLED.addLeds<SK6812, OUTPUT_PIN, GRB>(leds, 1); // Init LEDs
    delay(1000);
    // Create a thread for breathing LED.
    xTaskCreate(Key::breathLEDWrapper, "led", 1000, this, 0, NULL);
  }

  virtual void loop()
  {
    this->value = digitalRead(INPUT_PIN);
    Serial.print("digitalRead(KEY_PIN)");
    Serial.println(this->value);
    if (!this->value) // If Key was pressed.
    {
      M5.Lcd.setCursor(0, 100, 2);
      M5.Lcd.print(("  Pressed "));
      changeLedColor();
      // while (!digitalRead(INPUT_PIN)); // Hold until the key released.
    }
    else
    {
      M5.Lcd.setCursor(0, 100, 2);
      M5.Lcd.println(("  Released"));
    }
  }

  virtual int getValue()
  {
    return this->value;
  }

  virtual int getRaw()
  {
    return this->value;
  }

  virtual String getTitle()
  {
    return ("  UNIT-KEY\n   Example\n\n  Key State:");
  }

  static void breathLEDWrapper(void *parameter)
  {
    Key *key = static_cast<Key *>(parameter);
    key->breathLED();
  }

  void breathLED()
  {
    const int delayMs = 1275 / MAX_BRIGHTNESS;
    leds[0] = CRGB::Red;
    for (;;)
    {
      for (int i = 0; i < MAX_BRIGHTNESS;
           i++)
      { // Set LED brightness from 0 to 255.  设置LED亮度从0到255.
        FastLED.setBrightness(i);
        FastLED.show();
        delay(delayMs);
      }
      for (int i = MAX_BRIGHTNESS; i > 0;
           i--)
      { // Set LED brightness from 255 to 0.  设置LED亮度从255到0.
        FastLED.setBrightness(i);
        FastLED.show();
        delay(delayMs);
      }
    }
    vTaskDelete(NULL);
  }

  void changeLedColor()
  {
    Serial.println("changeLedColor");
    ledColor++;
    if (ledColor > 2)
      ledColor = 0;
    switch (
        ledColor)
    { // Change LED colors between R,G,B.  在红绿蓝中切换LED颜色.
    case 0:
      leds[0] = CRGB::Red;
      break;
    case 1:
      leds[0] = CRGB::Green;
      break;
    case 2:
      leds[0] = CRGB::Blue;
      break;
    default:
      break;
    }
  }
};

#endif