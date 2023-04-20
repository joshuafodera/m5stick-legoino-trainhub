/*
*******************************************************************************
https://docs.m5stack.com/en/api/stickc/system_m5stickc
*/
#include <M5StickCPlus.h>

#ifndef LCD_H
#define LCD_H

#define X_OFFSET 4
#define LINE_HEIGHT 20
const int32_t WIDTH = 135;
const int32_t HEIGHT = 240;

class LcdScreen
{
public:
  void setup(String text)
  {
    printLine(1, "Train Hub");
  }

  void loop()
  {
  }

  void printLine(int line, String text)
  {
    // M5.Lcd.fillRect(0, LINE_HEIGHT * line, WIDTH, 20, BLACK);
    M5.Lcd.setCursor(X_OFFSET, LINE_HEIGHT * line, 2);
    M5.Lcd.print(text + "        ");
  }
};

#endif