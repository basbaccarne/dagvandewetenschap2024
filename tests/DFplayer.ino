// sample script to test the DF player
// https://github.com/PowerBroker2/DFPlayerMini_Fast

// Wires:
// Arduino D10 to DF player TX
// Arduino D11 to DF player RX
// Arduino GND to DF player GND
// Arduino 5V to DF player VCC

// file structure:
// files need to start with 4 digits: e.g. 0001.mp3, 0002.mp3, 0003.mp3, etc.
// after these 4 digits, the file name can be anything: e.g. 0001.mp3, 0001_hello.mp3, 0001_hello_world.mp3, etc.
// the folder name needs to be "mp3", placed under the SD card "root" directory

// libraries
#include <DFPlayerMini_Fast.h>
DFPlayerMini_Fast myMP3; // Create an instance of the DFPlayerMini_Fast class named "myMP3"

#include <SoftwareSerial.h>
SoftwareSerial mySerial(10, 11); // set D10 & D11 as RX & TX

void setup()
{
    Serial.begin(9600);
    myMP3.begin(mySerial, true);

    myMP3.volume(10); // 0 to 30
    Serial.print("volume: ");
    Serial.println(10);
}

void loop()
{
    int randomSound = random(1, 12);
    Serial.print("playing file: ");
    Serial.println(randomSound);
    myMP3.play(randomSound);
    delay(1000);
}