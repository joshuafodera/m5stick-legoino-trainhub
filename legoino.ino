/*
*******************************************************************************
https://docs.m5stack.com/en/unit/fader
https://docs.m5stack.com/en/unit/key
https://docs.m5stack.com/en/unit/pahub

https://github.com/m5stack/M5StickC-Plus
https://github.com/corneliusmunz/legoino
https://docs.m5stack.com/en/api/stickc/system_m5stickc
*/

#include <Lpf2Hub.h>
#include <FastLED.h>
#include <Wire.h>
#include <M5StickCPlus.h>
#include ".\unit.h"
#include ".\fader.h"
#include ".\key.h"
#include ".\trainhub.h"
#include ".\lcd.h"

enum class State
{
  Init,
  Connecting,
  Connected,
  Loop,
  Disconnect,
  Complete
};

String printState(State state)
{
  switch (state)
  {
  case State::Init:
    return "Init";
  case State::Connecting:
    return "Connecting";
  case State::Connected:
    return "Connected";
  case State::Loop:
    return "Loop";
  case State::Disconnect:
    return "Disconnect";
  case State::Complete:
    return "Complete";
  }
}

State state = State::Init;
String unitType = "FADER";
IUnit *unit;
TrainHub *trainHub;
LcdScreen *lcd;

void setupLCD(String text)
{
  M5.Lcd.setCursor(0, 25, 2);
  M5.Lcd.print(text);
}

void setup()
{
  M5.begin();
  Serial.begin(115200);

  if (unitType == "KEY")
    unit = new Key();
  else if (unitType == "FADER")
    unit = new Fader();

  trainHub = new TrainHub();
  lcd = new LcdScreen();

  unit->setup();
  lcd->setup(unit->getTitle());

#ifdef DEBUG
  while (!Serial)
  {
    ;
  }
#endif

  Serial.println("Starting");
}

int count = 0;

// main loop
void loop()
{
  // every loop
  M5.update();
  unit->loop();
  trainHub->stateMachine(M5.BtnA.isPressed());

  // updates based on values
  int rawValue = unit->getRaw();
  int value = unit->getValue();
  TrainDirection trainDirection = trainHub->getTrainDirection(rawValue, 4095, 0.12);
  lcd->printLine(2, "state: " + printState(state));
  lcd->printLine(3, "rawValue: " + String(rawValue));
  lcd->printLine(4, "value: " + String(value));
  lcd->printLine(5, "TrainState: " + trainHub->getTrainState());
  lcd->printLine(6, "TrainDirection: " + printTrainDirection(trainDirection));

  trainHub->loop(M5.BtnA.isPressed(), value, trainDirection);

  delay(100);
} // End of loop
