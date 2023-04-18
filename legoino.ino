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
// #define NUM_LEDS 1
#define NUM_LEDS 14
#define MIN_BRIGHTNESS 0
#define MAX_BRIGHTNESS 25

#define KEY_PIN 33 // Port B input pin
#define LED_PIN 32 // Port B output pin

// Define the array of leds
CRGB leds[NUM_LEDS];

void LED(void *parameter);
void changeLedColor();

uint8_t beginHue = 0;
uint8_t deltaHue = 30;
uint8_t brightness = 100;
uint16_t rawADC = 0;

void setupLCD(String text)
{
  M5.Lcd.setCursor(0, 25, 2);
  M5.Lcd.print(text);
}

void setupFader()
{
  setupLCD(("  FADER UNIT TEST\n   Example\n\n  Fader State:"));
  pinMode(KEY_PIN, INPUT);                            // Init Key pin.
  FastLED.addLeds<NEOPIXEL, LED_PIN>(leds, NUM_LEDS); // Init LEDs
  delay(1000);
  fill_rainbow(leds, NUM_LEDS, beginHue, deltaHue);
}

void setupKey()
{
  setupLCD(("  UNIT-KEY\n   Example\n\n  Key State:"));
  pinMode(KEY_PIN, INPUT_PULLUP);                 // Init Key pin.
  FastLED.addLeds<SK6812, LED_PIN, GRB>(leds, 1); // Init LEDs
  delay(1000);
  // Create a thread for breathing LED.
  xTaskCreate(LED, "led", 1000, NULL, 0, NULL);
}

String state = "FADER";

void setup()
{
  M5.begin();
  if (state == "KEY")
    setupKey();
  else if (state == "FADER")
    setupFader();
}

void loop()
{
  if (state == "FADER")
  {
    rawADC = analogRead(KEY_PIN);
    Serial.print("analogRead(KEY_PIN)");
    Serial.println(rawADC);
    // Mapping ADC value to the brightness range
    brightness = map(rawADC, 0, 4095, MIN_BRIGHTNESS, MAX_BRIGHTNESS);
    FastLED.setBrightness(brightness); // Adjust the brightness of the FADER LED
    FastLED.show();
    Serial.printf("%d\r\n", rawADC);
    M5.Lcd.setCursor(0, 100, 2);
    M5.Lcd.print(("value: " + String(rawADC) + "  "));
    M5.Lcd.setCursor(0, 120, 2);
    M5.Lcd.print(("brightness: " + String(brightness) + "  "));
  }
  else
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
  }

  delay(100);
}

void LED(void *parameter)
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
