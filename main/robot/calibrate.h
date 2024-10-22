#ifndef CALIBRATEL_H
#define CALIBRATEL_H

#include <Arduino.h>

class calibrate {
public:
    calibrate();
    void gather(float value);
    float getBaseline();
private:
    float baseline;
    int counter;
    float sum;
};

#endif