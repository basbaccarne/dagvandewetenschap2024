#ifndef BEHAVIOR_H
#define BEHAVIOR_H

#include <Arduino.h>
#include <Adafruit_GFX.h>
#include <Adafruit_LEDBackpack.h>
#include <Servo.h>
#include <avr/pgmspace.h>

// Emotions bitmaps
extern const uint8_t eye_open_bmp[];
extern const uint8_t eye_closed_bmp[];
extern const uint8_t eye_halfopen_bmp[];
extern const uint8_t eye_upperleft_bmp[];
extern const uint8_t eye_upperleft_halfopen_bmp[];
extern const uint8_t eye_upperright_bmp[];
extern const uint8_t eye_upperright_halfopen_bmp[];

// LED control object
extern Adafruit_8x8matrix matrix;

// Servo objects
extern Servo servoV;
extern Servo servoH;

// Initial servo values
extern int posV;
extern int posH;

// Timers
extern unsigned long previousMillis_eyes;
extern unsigned long interval_eyes;
extern int state_eyes;
extern unsigned long previousMillis_head;
extern unsigned long interval_head;

void setupEyeHeadMovement();
void updateBehavior();
void updateEyeState();
void randomHeadMovement();

#endif