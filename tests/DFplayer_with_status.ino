/* 
sample script to test the DF player
https://github.com/PowerBroker2/DFPlayerMini_Fast
addition to the basic scripy: check play status and only play if there is no sound playing

Wires:
// Arduino RX to DF player TX
// Arduino TX (via 1k resistor) to DF player RX
// Arduino GND to DF player GND
// Arduino 5V to DF player VCC (5V pads on nano need be soldered)
// Speaker - to DF player SPK1
// Speaker + to DF player SPK2

Voltage divider to read 5V high signal from BUSY pin to 3.3V arduino pin
// BUSY to R1 (560 ohm) side 1
// R1 (560 ohm) side 2 to arduino pin 2
// R1 (560 ohm) side 2 to R2 (1k ohm) side 1
// R2 (1k ohm) side 2 to GND


file structure:
// files need to start with 4 digits: e.g. 0001.mp3, 0002.mp3, 0003.mp3, etc.
// after these 4 digits, the file name can be anything: e.g. 0001.mp3, 0001_hello.mp3, 0001_hello_world.mp3, etc.
// the folder name needs to be "MP3", placed under the SD card "root" directory

Source: https://github.com/DFRobot/DFRobotDFPlayerMini/blob/master/examples/GetStarted/GetStarted.ino
*/

#include "DFRobotDFPlayerMini.h"

// On Arduino Nano 33IOT the TX & RX pins are added to the Serial1 object
// Name Serial1 as DFSerial to make it clear that it is used for the DFPlayer

#define DFSerial Serial1
DFRobotDFPlayerMini myDFPlayer;

// Variables for the audio status
int audioStatus_pin = 2;
int audioStatus = 0;

void setup() {
  // set the audio status pin as input
  pinMode(audioStatus_pin, INPUT);

  // initialize both Serial communications
  DFSerial.begin(9600);
  Serial.begin(115200);
  
  // Open the SD card stream
  myDFPlayer.begin(DFSerial);
  myDFPlayer.volume(30);  // Set volume value. From 0 to 30
  // myDFPlayer.play(1);  //Play the first mp3
}

void loop() {
  static unsigned long timer = millis();

  // get the status of the player
  audioStatus = digitalRead(audioStatus_pin);

  // print the status of the status pin
  if (audioStatus == HIGH) {
    Serial.println("Not playing");
  } else {
    Serial.println("Playing");
  }

  // every 3 seconds, play the second mp3 file, but only if the player is not playing
  if (millis() - timer > 3000) {
    timer = millis();
    if (audioStatus == HIGH) {
      myDFPlayer.play(9);
      Serial.println("bliep!");
    }
  }
}