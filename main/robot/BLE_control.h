#ifndef BLECONTROL_H
#define BLECONTROL_H

#include <Arduino.h>
#include <ArduinoBLE.h>

class BLEControl {
public:
  BLEControl();
  void begin();
  void checkForSignal();
  float getFloatValue();

private:
  BLEDevice peripheral;
  BLECharacteristic characteristic;
  float floatValue;
};

#endif