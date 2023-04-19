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

// create a hub instance
Lpf2Hub myTrainHub;
byte port = (byte)PoweredUpHubPort::A;

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
  Serial.begin(115200);
  if (state == "KEY")
    unit = new Key();
  else if (state == "FADER")
    unit = new Fader();

  unit->setup();
  setupLCD(unit->getTitle());

  while (!Serial)
  {
    ;
  }
  Serial.println("Starting");
}

// main loop
void loop()
{
  unit->loop();

  if (!myTrainHub.isConnected() && !myTrainHub.isConnecting())
  {
    myTrainHub.init(); // initalize the PoweredUpHub instance
    // myTrainHub.init("90:84:2b:03:19:7f"); //example of initializing an hub with a specific address
  }

  // connect flow. Search for BLE services and try to connect if the uuid of the hub is found
  if (myTrainHub.isConnecting())
  {
    myTrainHub.connectHub();
    if (myTrainHub.isConnected())
    {
      Serial.println("Connected to HUB");
      Serial.print("Hub address: ");
      Serial.println(myTrainHub.getHubAddress().toString().c_str());
      Serial.print("Hub name: ");
      Serial.println(myTrainHub.getHubName().c_str());
    }
    else
    {
      Serial.println("Failed to connect to HUB");
    }
  }

  // if connected, you can set the name of the hub, the led color and shut it down
  if (myTrainHub.isConnected())
  {

    char hubName[] = DEVICE_NAME;
    myTrainHub.setHubName(hubName);

    if (unit->getValue() == 0)
    {
      myTrainHub.setLedColor(Color::GREEN);
      delay(1000);
      myTrainHub.setBasicMotorSpeed(port, 35);
      delay(1000);
    }
    else
    {
      myTrainHub.setLedColor(Color::RED);
      delay(1000);
      myTrainHub.setBasicMotorSpeed(port, -35);
      delay(1000);
    }

    myTrainHub.stopBasicMotor(port);
    delay(1000);
  }
  else
  {
    Serial.println("Train hub is disconnected");
  }

} // End of loop
