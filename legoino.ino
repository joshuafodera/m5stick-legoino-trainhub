/*
*******************************************************************************
* Visit for more information: https://docs.m5stack.com/en/unit/fader
* Visit for more information: https://docs.m5stack.com/en/unit/key
*/

#include <M5StickCPlus.h>
#include <FastLED.h>
#include <Wire.h>

uint8_t ledColor = 0;

// Setup LEDs on port B
#define NUM_LEDS 1
#define LED_CLASS SK6812
// #define NUM_LEDS 14
// #define LED_CLASS NEOPIXEL

#define KEY_PIN 33 // Port B input pin
#define LED_PIN 32 // Port B output pin
CRGB leds[NUM_LEDS];

void LED(void *parameter);
void changeLedColor();

void setup()
{
  M5.begin();
  M5.Lcd.setCursor(0, 25, 2);
  M5.Lcd.print(("  UNIT-KEY\n   Example\n\n  Key State:"));

  pinMode(KEY_PIN, INPUT_PULLUP); // Init Key pin.

  FastLED.addLeds<LED_CLASS, LED_PIN, GRB>(leds, NUM_LEDS); // Init FastLED.

  xTaskCreate(
      LED, "led", 1000, NULL, 0,
      NULL); // Create a thread for breathing LED. 创建一个线程用于LED呼吸灯.
}

void loop()
{
  Serial.print("digitalRead(KEY_PIN)");
  Serial.println(digitalRead(KEY_PIN));
  if (!digitalRead(KEY_PIN)) // If Key was pressed.
  {
    M5.Lcd.setCursor(0, 100, 2);
    M5.Lcd.print(("  Pressed "));
    changeLedColor();
    while (!digitalRead(KEY_PIN))
      ; // Hold until the key released.
  }
  else
  {
    M5.Lcd.setCursor(0, 100, 2);
    M5.Lcd.println(("  Released"));
  }
  delay(100);
}

void LED(void *parameter)
{
  leds[0] = CRGB::Red;
  for (;;)
  {
    for (int i = 0; i < 255;
         i++)
    { // Set LED brightness from 0 to 255.  设置LED亮度从0到255.
      FastLED.setBrightness(i);
      FastLED.show();
      delay(5);
    }
    for (int i = 255; i > 0;
         i--)
    { // Set LED brightness from 255 to 0.  设置LED亮度从255到0.
      FastLED.setBrightness(i);
      FastLED.show();
      delay(5);
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
