#ifndef CALIBRATE_H
#define CALIBRATE_H

#include <Arduino.h>

class calibrate {
    public:
    calibrate();
    void gather();
    float getBaseline();

    private:
    float baseline;
    int counter;
    int sum;

};

#endif