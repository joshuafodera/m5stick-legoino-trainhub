/*
*******************************************************************************
* Copyright (c) 2022 by M5Stack
*                  Equipped with M5Core sample source code
*                          配套  M5Core 示例源代码
* Visit for more information: https://docs.m5stack.com/en/unit/fader
* 获取更多资料请访问: https://docs.m5stack.com/zh_CN/unit/fader
*
* Describe: UNIT FADER. 滑动电位器/推子
* Date: 2021/8/20
*******************************************************************************
  Connect UNIT FADER to port B and push the FADER slider to adjust the input
value and light brightness 将UNIT FADER连接到B端口,
推动FADER滑杆即可实现调整输入数值大小与灯光亮度
*/

#include "FastLED.h"
#include <M5StickCPlus.h>
#include <Wire.h>

#define DEBUG

#ifdef DEBUG
#define DPRINT(...) Serial.print(__VA_ARGS__)
#define DPRINTLN(...) Serial.println(__VA_ARGS__)
#else
#define DPRINT(...)   // now defines a blank line
#define DPRINTLN(...) // now defines a blank line
#endif

// How many leds in your strip?
#define NUM_LEDS 14

#define FADER_INPUT_PINS 36
#define KEY_PIN 33
#define LED_PIN 32

uint8_t ledColor = 0;
CRGB key_leds[1];

void LED(void *parameter);
void changeLedColor();

// Define the array of leds
CRGB leds[NUM_LEDS];

uint8_t beginHue = 0;
uint8_t deltaHue = 30;
uint8_t brightness = 40;
uint16_t rawADC = 0;

void setupFader(void)
{
  pinMode(FADER_INPUT_PINS, INPUT);
  M5.Lcd.drawString("FADER UNIT TEST", 160, 60, 4);
  FastLED.addLeds<NEOPIXEL, LED_PIN>(leds, NUM_LEDS);
  delay(1000);
  fill_rainbow(leds, NUM_LEDS, beginHue, deltaHue);
}

void setupKey()
{
  M5.Lcd.print("\n UNIT-KEY Example\n\n    Key State:");
  /* Init key pin */
  pinMode(KEY_PIN, INPUT_PULLUP);
  pinMode(LED_PIN, OUTPUT);
  /* Init RGB led */
  FastLED.addLeds<SK6812, LED_PIN, GRB>(key_leds, 1);

  xTaskCreate(
      LED, "led", 1000, NULL, 0,
      NULL); // Create a thread for breathing LED. 创建一个线程用于LED呼吸灯.
}

void setup()
{
  DPRINTLN("setup");

  M5.begin();
  Wire.begin();
  //  gpio_pulldown_dis(GPIO_NUM_25);
  //  gpio_pullup_dis(GPIO_NUM_25);

  M5.Lcd.fillScreen(BLACK);
  M5.Lcd.setCursor(0, 10);
  M5.Lcd.setTextColor(WHITE);
  M5.Lcd.setTextSize(1);
  M5.Lcd.printf("  Test!");
  M5.Lcd.println(" ");

  M5.Lcd.setTextDatum(MC_DATUM);

  setupKey();

  Serial.begin(115200);
  while (!Serial)
    ;
}

void loopKey()
{
  Serial.print("digitalRead(KEY_PIN) ");
  Serial.println(digitalRead(KEY_PIN));
  /* If Key was pressed */
  if (!digitalRead(KEY_PIN))
  {
    M5.Lcd.setCursor(75, 130);
    M5.Lcd.print((" Pressed "));
    changeLedColor(); // Change LED color.  更换LED呼吸灯颜色.

    // FastLED.setBrightness(255);
    // FastLED.show();
    /* Hold until the key released */
    // delay(100);
    while (!digitalRead(
        KEY_PIN)) // Hold until the key released.  在松开按键前保持状态.
      ;
    // while (!digitalRead(KEY_PIN))
    ;
  }
  else
  {
    M5.Lcd.setCursor(75, 130);
    M5.Lcd.println(("Released"));
    // FastLED.setBrightness(0);
    // FastLED.show();
  }
  delay(100);
}

void loopFader()
{
  DPRINTLN("loop");

  rawADC = analogRead(FADER_INPUT_PINS);     // Read ADC value 读取ADC数值
  brightness = map(rawADC, 0, 4000, 0, 255); // The mapping ADC value is the brightness value
  //                                    // range  映射ADC值为亮度值范围
  FastLED.setBrightness(brightness); // Adjust the brightness of the FADER
  //                                    // LED 调整FADER LED灯亮度
  FastLED.show();
  Serial.printf("%d\r\n", rawADC); // DPRINTLN
  M5.Lcd.fillRect(0, 120, 320, 100, BLACK);
  M5.Lcd.drawString("value: " + String(rawADC), 160, 160, 4);
  delay(1000);
  // byte error, address;
  // int nDevices = 0;

  // delay(5000);

  // Serial.println("Scanning for I2C devices ...");
  // for (address = 0x01; address < 0x7f; address++)
  // {
  //   Wire.beginTransmission(address);
  //   error = Wire.endTransmission();
  //   if (error == 0)
  //   {
  //     Serial.printf("I2C device found at address 0x%02X\n", address);
  //     nDevices++;
  //   }
  //   else if (error != 2)
  //   {
  //     Serial.printf("Error %d at address 0x%02X\n", error, address);
  //   }
  // }
  // if (nDevices == 0)
  // {
  //   Serial.println("No I2C devices found");
  // }
}

void loop()
{
  Serial.println("loop");
  loopKey();
}

void LED(void *parameter)
{
  key_leds[0] = CRGB::Red;
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
    key_leds[0] = CRGB::Red;
    break;
  case 1:
    key_leds[0] = CRGB::Green;
    break;
  case 2:
    key_leds[0] = CRGB::Blue;
    break;
  default:
    break;
  }
}