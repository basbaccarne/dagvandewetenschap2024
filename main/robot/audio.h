#ifndef AUDIO_H
#define AUDIO_H

#include <Arduino.h>
#include "DFRobotDFPlayerMini.h"

// On Arduino Nano 33IOT the TX & RX pins are added to the Serial1 object
// Name Serial1 as DFSerial to make it clear that it is used for the DFPlayer

#define DFSerial Serial1
extern DFRobotDFPlayerMini myDFPlayer;

// Variables for the audio status
extern int audioStatus_pin;
extern int audioStatus;

// setup and play sound functions
void setupDFPlayer();
void playSound(int fileNumber);

#endif