/*
This is the master script for the punch bag.
In entails the following components:
- Arduino nano 33 IoT: the microcontroller
- Velostat: a force sensor that detects the force of the punch
- LED strip: light up from the center (led 60 of 120) and spreads outward depending on the force of the punch
- A DF player mini with a speaker: plays a sound when the punch is hard enough
- BLE connection to send the values to the robot

** LED **
- LED strip: WS2812 RGB LED strip
- Requires 5V & 0.2W per LED = 4.8 A for 120 leds
- N = 120 leds on an original strip
- We ditched the Neopixel library because it was too slow
- The FastLED library works much better

*** DF player mini ***
- file structure:
// files need to start with 4 digits: e.g. 0001.mp3, 0002.mp3, 0003.mp3, etc.
// after these 4 digits, the file name can be anything: e.g. 0001.mp3, 0001_hello.mp3, 0001_hello_world.mp3, etc.
// the folder name needs to be "MP3", placed under the SD card "root" directory

** Wiring **
- Power:
  * External power to VIN Arduino
  * External power to LED strip
  * External power to (+) side Velostat
  * Externbal power to DF player VCC

- Grounds:
  * External GND to GND Arduino
  * External GND to GND LED strip
  * External GND to side 2 of the resistor 
  * External GND to DF player GND

- Signals:
  * Arduino pin 7 to DIN of the LED strip

  * Arduino RX to DF player TX
  * Arduino TX (via 1k resistor) to DF player RX
  * Speaker - to DF player SPK1
  * Speaker + to DF player SPK2
  
  * Arduino A0 to side 1 of the resistor 
  * (-) side velostat to side 1 of resistor (150Ω, brown/green/brown)) 
*/

// libraries
#include <FastLED.h>
#include <ArduinoBLE.h>
#include "DFRobotDFPlayerMini.h"

// define the BLE service
BLEService punchService("95ff7bf8-aa6f-4671-82d9-22a8931c5387");
BLEFloatCharacteristic punch("95ff7bf8-aa6f-4671-82d9-22a8931c5387", BLERead | BLENotify);

// pins
#define ledPin 7
#define sensorPin A0
#define DFSerial Serial1 // On Arduino Nano 33IOT the TX & RX pins are added to the Serial1 object

// configuration variables
const int lower_force_threshold = 200; // force threshold to light up the first LED (check serial to set right value) aka punch trehsold
const long ledInterval = 0; // Set the interval between lighting up the LEDs
const long sensorInterval = 10; // Set the interval between reading the sensor (timer instead of delay)
const int volume = 30; //Set volume value. From 0 to 30
const int sound_duration = 300; // allow the sound to play > could be optimized with a switch case

// timers (to avoid delay)
unsigned long previousMillis = 0;
unsigned long previousMillis_soundFX = 0;

// sensor readings
int sensorReading = 0;

// LED strip variables
#define ledCount 120
CRGB leds[ledCount]; // Define the array of leds
int brightness = 100; // Set brightness level (0-255) - check for amps required
int currentLitLEDs = 0; // The number of LEDs currently lit up
int targetLitLEDs = 0; // The number of LEDs that should be lit up

// DF player
DFRobotDFPlayerMini myDFPlayer; // Create the DF player object

void setup() {

  // initialize Serial communication
  Serial.begin(9600);
  DFSerial.begin(9600); // inititialize RX/TX communication for the DF player

  // initialize led strip
  FastLED.addLeds<NEOPIXEL, ledPin>(leds, ledCount);
  FastLED.setBrightness(brightness); 
  FastLED.clear();
  FastLED.show();

  // initialize BLE: part 1
  if (!BLE.begin())
    {
        Serial.println("BLE failed to Initiate");
        delay(500);
        while (1);
    }
  
  // initialize BLE: part 2 
  // Set advertised local name and services UUID
  BLE.setLocalName("BLE punchbag 1 (peripheral)");
  BLE.setAdvertisedService(punchService);
  punchService.addCharacteristic(punch);
  BLE.addService(punchService);

  // initialize BLE: part 3
  // Set default values for characteristic
  punch.writeValue(0.0);

  // initialize BLE: part 4
  // Start advertising
  BLE.advertise();
  Serial.println("BLE sending device 1 is now advertising ...");

  // initialize DF player
  myDFPlayer.begin(DFSerial);
  myDFPlayer.volume(volume); 
}

void loop() {
  // get actual millis
  unsigned long currentMillis = millis();

  // read sensor value & map it to the number of LEDs to light up
  if (currentMillis - previousMillis >= sensorInterval) {
    previousMillis = currentMillis;
    // read the sensor value
    sensorReading = analogRead(sensorPin);
    Serial.print(sensorReading);
    // map the sensor value to the number of LEDs to light up
    targetLitLEDs = map(sensorReading, lower_force_threshold, 1023, 0, ledCount / 2);
    if (targetLitLEDs < 0) {
      targetLitLEDs = 0;
    }
    // debugging prints
    Serial.print(",");
    Serial.print(targetLitLEDs);
    int leftLeds = ledCount / 2 + targetLitLEDs;
    Serial.print(",");
    Serial.print(leftLeds);
    int rightLeds = ledCount / 2 - targetLitLEDs;
    Serial.print(",");
    Serial.println(rightLeds);

    // send the sensor value over BLE
    punch.writeValue(sensorReading);
  }

  // If the force value is increasing, go up fast
  if (targetLitLEDs > currentLitLEDs) {
    currentLitLEDs = targetLitLEDs;
    updateLEDStrip(currentLitLEDs);
  }

  // If the force value is decreasing, go down slowly
  else if (targetLitLEDs < currentLitLEDs) {
    currentLitLEDs--;  // Go down one LED at a time (slowly)
    updateLEDStrip(currentLitLEDs);
    delay(ledInterval);  // Slow down the decrease
  }

  // DF player: play the second audio file if the sensor value is above punch threshold (lower_force_threshold)
  // delay 100 ms to allow the sound to play
  if (sensorReading > lower_force_threshold && currentMillis - previousMillis_soundFX >= sound_duration ) {
    previousMillis_soundFX = currentMillis;
    myDFPlayer.play(2);
    Serial.println("bliep!");
  }

  // BLE error handling
  BLEDevice central = BLE.central();
  if (central) {
    Serial.print("Connected to central: ");
    Serial.println(central.address());
    if (!central.connected()) {
      Serial.println("Disconnected from central");
    }
  }
  // Poll for BLE events (this processes any pending BLE events)
  BLE.poll();
}

void updateLEDStrip(int numLitLEDs) {
  for (int i = 0; i < ledCount; i++) {
    if (i < ledCount / 2 - numLitLEDs){
      leds[i] = CRGB::Black;
    }
    else if (i < ledCount / 2 + numLitLEDs){
      leds[i] = CRGB::Red; 
    }
    else {
      leds[i] = CRGB::Black;
    }
  }

  FastLED.show();
}