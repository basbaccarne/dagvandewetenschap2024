/*
This is a simple script to test a servo motor. It moves the servo to 0, 90, and 180 degrees, waiting for a second between each movement.
Wiring: external 5V power source to the servo, GND of the servo to the GND of the Arduino, and the signal wire of the servo to pin 11 of the Arduino.
Draws peaks around 0,2 A when moving.
*/

#include <Servo.h>

Servo myServo;

void setup() {
  myServo.attach(11); // Attach the servo to pin 11
}

void loop() {
  myServo.write(0);  // Move to 0 degrees
  delay(1000);       // Wait for a second
  myServo.write(90); // Move to 90 degrees
  delay(1000);       // Wait for a second
  myServo.write(180); // Move to 180 degrees
  delay(1000);       // Wait for a second
}