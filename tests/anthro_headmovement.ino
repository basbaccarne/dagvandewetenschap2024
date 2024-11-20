/* this script tests some natural head movement

It moves two servo's across an array of postions (X and Y axes)

Without delays.
Using incremental position updates for controlled and smooth movement.

For better smoothing: https://www.youtube.com/watch?v=jsXolwJskKM 

wiring & remarks
- the M5stack 360 servo are controlled differently
- Most servo's do not work at 3.3V
- When working with Arduino Nano, prefer an external power source
- Draws peaks around 0,2 A when moving. Multiple motors peak higher
- Don't forget to connect grounds

wiring:
External 5V Power Supply
+5V  ---------------------->  Servo Power (Red)
GND  ---------------------->  Servo Ground (Black/Brown)
                           |
                           +--> Arduino GND

Arduino
Digital Pin 12  ------------>  Servo Control (Yellow/Orange/White)
Digital Pin 11  ------------>  Servo Control (Yellow/Orange/White)
*/

// libraries
#include <Servo.h>

// general settings
int ServoInterval = 30;              // how fast should the robot head move?
int lowerRestIntervalLimit = 600;    // what is the minimum time between two movements?
int higherRestIntervalLimit = 2000;  // what is the maximum time between two movements?
int lowerPostion = 40;               // What is the lower limit of the X and Y postions?
int higherPostion = 80;              // What is the upper limit of the X and Y postions?
const int positions = 10;            // How many different positions do you want?

// define servo objects
Servo myServoX;
Servo myServoY;

// position arrays (will be randomized)
int arrayX[positions];
int arrayY[positions];

// variables for servo control & timing
bool moving = false;
bool resting = false;
unsigned long startRest = 0;
int restInterval;
unsigned long ServoPreviousMillis = 0;
unsigned long currentMillis;
int i = 0;

// start positions
int posX = 60;
int prevPosX = 60;
int posY = 60;
int prevPosY = 60;

void setup() {

  // init serial & define servo pins
  Serial.begin(9600);
  myServoX.attach(11);
  myServoY.attach(12);

  // set motors to start position
  myServoX.write(posX);
  myServoY.write(posY);

  // generate random positions
  for (int j = 0; j < positions; j++) {
    arrayX[j] = random(lowerPostion, higherPostion);
    arrayY[j] = random(lowerPostion, higherPostion);
  }
}

void loop() {
  currentMillis = millis();
  if (currentMillis - ServoPreviousMillis >= ServoInterval) {
    ServoPreviousMillis = currentMillis;
    UpdateHead();
  }
}

void UpdateHead() {

  // if the head is not moving or resting
  if (!moving) {

    // get a new X and Y postion (next one in the array)
    posX = arrayX[i];
    posY = arrayY[i];
    Serial.print("moving to X: ");
    Serial.print(posX);
    Serial.print(" Y: ");
    Serial.println(posY);

    // shift the index for the next cycle (& reset at the end)
    i++;
    if (i > positions) { i = 0; }

    // change status to moving
    moving = true;
  }

  // if the head is moving, but not resting
  if (moving && !resting) {

    // slowly move up if the new position is higher
    if (posX > prevPosX) {
      prevPosX++;
      myServoX.write(prevPosX);
    }
    if (posY > prevPosY) {
      prevPosY++;
      myServoY.write(prevPosY);
    }

    // slowly down up if the new position is lower
    if (posX < prevPosX) {
      prevPosX--;
      myServoX.write(prevPosX);
    }
    if (posY < prevPosY) {
      prevPosY--;
      myServoY.write(prevPosY);
    }

    // if both movements are done
    if (posY - prevPosY == 0 && posX - prevPosX == 0) {
      // enter rest state for a random time
      startRest = millis();
      resting = true;
      restInterval = random(lowerRestIntervalLimit, higherRestIntervalLimit);
    }
  }

  if (resting) {
    // rest as for a random amount of time
    if (currentMillis - startRest >= restInterval) {
      startRest = currentMillis;
      // then enter the new state
      resting = false;
      moving = false;
    }
  }
}