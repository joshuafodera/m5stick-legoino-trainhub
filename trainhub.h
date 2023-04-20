/*
https://github.com/corneliusmunz/legoino
*/

#ifndef TRAINHUB_H
#define TRAINHUB_H

#include <Lpf2Hub.h>
#include <M5StickCPlus.h>

#undef RED   // undefine the problematic #define
#undef GREEN // undefine the problematic #define
#undef BLUE  // undefine the problematic #define

#define DEVICE_NAME "Christmas" //"Technic Hub"

// create a hub instance
Lpf2Hub myTrainHub;
byte port = (byte)PoweredUpHubPort::A;

enum class TrainDirection
{
  Stop,
  Forward,
  Reverse
};

enum class TrainState
{
  Null,
  Init,
  Connecting,
  Connected,
  Running,
  Disconnected,
  Error,
};

enum class TrainTaskState
{
  Start,
  Running
};

String printTrainDirection(TrainDirection direction)
{
  switch (direction)
  {
  case TrainDirection::Stop:
    return "Stop";
  case TrainDirection::Forward:
    return "Forward";
  case TrainDirection::Reverse:
    return "Reverse";
  }
}

class TrainHub
{
private:
  TrainState state;
  TrainTaskState taskState;

public:
  void setup()
  {
    state = TrainState::Null;
    taskState = TrainTaskState::Running;
  }

  void stateMachine(int isBtnAPressed)
  {
    if (taskState == TrainTaskState::Running)
    {
      taskState = TrainTaskState::Start;

      if (!myTrainHub.isConnected() && !myTrainHub.isConnecting() && isBtnAPressed)
      {
        state = TrainState::Init;
      }
      else if (myTrainHub.isConnecting())
      {
        state = TrainState::Connecting;
      }
      else if (state == TrainState::Running && !myTrainHub.isConnected())
      {
        state = TrainState::Disconnected;
      }
      else if (state == TrainState::Connected || state == TrainState::Running)
      {
        state = TrainState::Running;
      }
      else if (myTrainHub.isConnected())
      {
        state = TrainState::Connected;
      }
    }
  }

  void loop(int isBtnAPressed, int value, TrainDirection direction)
  {
    if (taskState == TrainTaskState::Start)
    {
      taskState = TrainTaskState::Running;
      switch (state)
      {
      case TrainState::Init:
        Serial.println("TrainHub: Init");
        // myTrainHub.init(); // initialize the PoweredUpHub instance
        myTrainHub.init("90:84:2b:08:0c:f6"); // Initialize hub with a specific address
        break;
      case TrainState::Connecting:
        Serial.println("TrainHub: Connecting");
        myTrainHub.connectHub();
        delay(100);
        break;
      case TrainState::Connected:
        // myTrainHub.setLedColor(Color::BLUE);
        Serial.println("TrainHub: Connected to HUB");
        Serial.print("Hub address: ");
        Serial.println(myTrainHub.getHubAddress().toString().c_str());
        Serial.print("Hub name: ");
        Serial.println(myTrainHub.getHubName().c_str());
        break;
      case TrainState::Disconnected:
        Serial.println("TrainHub: Disconnected");
        break;
      case TrainState::Error:
        Serial.println("TrainHub: Error");
        break;
      }
    }
    if (state == TrainState::Running && taskState == TrainTaskState::Running)
    {
      // -100..0..100
      if (direction == TrainDirection::Stop)
      {
        myTrainHub.stopBasicMotor(port);
      }
      else if (direction == TrainDirection::Forward)
      {
        myTrainHub.setBasicMotorSpeed(port, value);
      }
      else if (direction == TrainDirection::Reverse)
      {
        myTrainHub.setBasicMotorSpeed(port, -value);
      }
      delay(200);
    }
  }

  void loopAction(int value)
  {
    if (!myTrainHub.isConnected() && !myTrainHub.isConnecting())
    {
      myTrainHub.init(); // initialize the PoweredUpHub instance
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

      if (value == 0)
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
  }

  TrainDirection getTrainDirection(int value, int max, float bufferPercent)
  {
    int mid = max / 2;
    int buffer = static_cast<int>(max * bufferPercent);

    if (value < (mid + buffer) && value > (mid - buffer))
    {
      return TrainDirection::Stop;
    }
    else if (value < mid)
    {
      return TrainDirection::Forward;
    }

    return TrainDirection::Reverse;
  }

  String getTrainState()
  {
    switch (state)
    {
    case TrainState::Init:
      return "Init";
    case TrainState::Connecting:
      return "Connecting";
    case TrainState::Connected:
      return "Connected";
    case TrainState::Running:
      return "Running";
    case TrainState::Disconnected:
      return "Disconnected";
    case TrainState::Error:
      return "Error";
    }

    return "";
  }

  String getTrainTaskState()
  {
    switch (taskState)
    {
    case TrainTaskState::Start:
      return "Start";
    case TrainTaskState::Running:
      return "Running";
    }

    return "";
  }
};

#endif