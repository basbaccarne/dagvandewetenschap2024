// this script tests some anthropomorphic head movement

// the head can move up and down
// the head can move left and right
// using two servos

#include <Servo.h>

Servo servoX;
Servo servoY;

int pos = 0;

void setup()
{
    servoX.attach(9);
    servoY.attach(10);
}

void loop()
{
    // Go from 0° to 180°
    for (pos = 0; pos <= 180; pos += 1)
    {
        servoX.write(pos);
        servoY.write(pos);
        delay(15);
    }

    // Go from 180° to 0°
    for (pos = 180; pos >= 0; pos -= 1)
    {
        servoX.write(pos);
        servoY.write(pos);
        delay(15);
    }
}
