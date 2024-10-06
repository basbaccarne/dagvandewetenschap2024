// this script tests some anthropomorphic head movement

/* the concept
  - the head can move up and down (servoV, vertical)
  - the head can move left and right (servoH, horzintal)
  - using two servos
  - using smoothing
  - good resource: https://www.youtube.com/watch?v=jsXolwJskKM 
*/

/* wiring & remarks
- the M5stack 360 servo are controlled differently
- Most servo's do not work at 3.3V
- When working with Arduino Nano, prefer an external power source
- Don't forget to connect grounds

wiring:
External 5V Power Supply
+5V  ---------------------->  Servo Power (Red)
GND  ---------------------->  Servo Ground (Black/Brown)
                           |
                           +--> Arduino GND

Arduino
Digital Pin 12  ------------>  Servo Control (Yellow/Orange/White)

*/

#include <Servo.h>

Servo servoV;
Servo servoH;

int pos = 0;

void setup() {
  servoV.attach(12);
  servoH.attach(11);
}

void loop() {
  // movement part 1
  for (int pos = 90; pos >= 0; pos--) {
    servoV.write(pos);
    servoH.write(90 - pos);
    delay(30);

    Serial.println(pos);
  }

  // movement part 2
  for (int pos = 0; pos <= 90; pos++) {
    servoV.write(pos);
    servoH.write(90 - pos);
    delay(30);

    Serial.println(pos);
  }
}
