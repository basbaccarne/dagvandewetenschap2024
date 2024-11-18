// include the .h file
#include "audio.h"

DFRobotDFPlayerMini myDFPlayer;
int audioStatus_pin = 2;
int audioStatus = 0;

void setupDFPlayer() {
  // set the audio status pin as input
  pinMode(audioStatus_pin, INPUT);

  // initialize both Serial communications
  DFSerial.begin(9600);

  // Open the SD card stream
  myDFPlayer.begin(DFSerial);
  myDFPlayer.volume(30);  // Set volume value. From 0 to 30
}

void playSound(int fileNumber) {
  // get the status of the player
  // audioStatus = digitalRead(audioStatus_pin);
    myDFPlayer.playMp3Folder(fileNumber);
    Serial.print("Audio: ");
    Serial.println(fileNumber);
}