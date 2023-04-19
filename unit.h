#ifndef IUNIT_H
#define IUNIT_H

class IUnit
{
public:
  virtual void setup() = 0;
  virtual void loop() = 0;
  virtual int getValue() = 0;
  virtual String getTitle() = 0;
};

#endif