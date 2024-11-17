/*
This is the master script for the punch bag.
In entails the following components:
- Arduino nano 33 IoT: the microcontroller
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
// after these 4 digits, the file name can be anything: e.g. 0001.mp3,
0001_hello.mp3, 0001_hello_world.mp3, etc.
// the folder name needs to be "MP3", placed under the SD card "root" directory

** Wiring **
- Power:
  * External power to VIN Arduino
  * External power to LED strip
  * External power to (+) side Velostat
  * Exterbal power to DF player VCC   

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

// library
#include <Arduino_LSM6DS3.h>
#include <ArduinoBLE.h>
#include <FastLED.h>
#include "DFRobotDFPlayerMini.h"

// parameters
int brightness = 100;  // Set brightness level of leds (0-255) - check for amps required
//  threshold to detect something a s a punch
// (check serial to set right value)
const int lower_punch_threshold = 200;
const int upper_force_threshold = 400;  // for mapping the leds
const int acceleroSamplingFrequency = 10;
const long ledInterval = 10;  // Set the decrease rate of the leds

// new FSR variables
int punchThreshold = 50;
const int sensorPin = A0;
int sensorReading = 0;
int peakReading = 0;
bool punching = false;
bool sending = false;

// BLE information
BLEService punchService("95ff7bf8-aa6f-4671-82d9-22a8931c5387");
BLEFloatCharacteristic punch("95ff7bf8-aa6f-4671-82d9-22a8931c5387", BLERead | BLENotify);

// pins
#define ledPin 7
#define DFSerial Serial1

// timer
unsigned long currentMillis;

// accelerometer variables
int previousMillisAccelero;
float ax, ay, az;
float ax_start, ay_start, az_start;
float punching_bag_mass = 25;
float gravity_constant = 9.81;
float range_contant = 4;                                 // Range setting of the ACC
float gravity_range = gravity_constant * range_contant;  // Full (positive) range of ACC in m/s²
float register_range = pow(2, 15);                       // Full (positive) register range of the ACC register (16bit)

float punchForce = 0;
int punches = 0;

// led variables
unsigned long previousMillisLED;

#define ledCount 120
CRGB leds[ledCount];
int currentLitLEDs = 0;
int targetLitLEDs = 0;

// init DFplayer
DFRobotDFPlayerMini myDFPlayer;

void setup() {
  Serial.begin(9600);
  DFSerial.begin(9600);

  pinMode(A0, INPUT);

  // initialize accelerometer
  if (!IMU.begin()) {
    Serial.println("Failed to initialize IMU!");
    while (1)
      ;
  }
  // inial read of accelerometer values
  if (IMU.accelerationAvailable()) {
    IMU.readAcceleration(ax_start, ay_start, az_start);
  }

  // initialize BLE: part 1
  BLE.begin();

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
  Serial.println("BLE sending device is now advertising ...");

  // initialize led strip
  FastLED.addLeds<NEOPIXEL, ledPin>(leds, ledCount);
  FastLED.setBrightness(brightness);
  FastLED.clear();
  FastLED.show();

  // initialize Audio module
  myDFPlayer.begin(DFSerial);
  myDFPlayer.volume(20);
}

void loop() {
  // Check BLE connection
  BLEDevice central = BLE.central();

  // Send data if there is a connection
  if (central) {
    if (central.connected()) {
      // measure force & send over BLE
      // getForce();
      FSR_reading();
    }
  }

  // Let us know if there is no connection
  if (!central) {
    Serial.println("Disconnected from central");
  }

  // Poll for BLE events (this processes any pending BLE events)
  BLE.poll();
  central = BLE.central();

  // delay for stability
  delay(10);
}

void getForce() {
  // function to read the accelorometer, calculate the punch force and detect a punch

  // sampling frequency
  currentMillis = millis();
  if (currentMillis - previousMillisAccelero >= acceleroSamplingFrequency) {
    previousMillisAccelero = currentMillis;

    // read the acceleration values
    if (IMU.accelerationAvailable()) {
      IMU.readAcceleration(ax, ay, az);

      // calculate the force of the punch
      float prev_punch = punchForce;
      float punchForce =
        punching_bag_mass * sqrt(pow((ax - ax_start) * gravity_constant, 2) + pow((ay - ay_start) * gravity_constant, 2) + pow((az - az_start) * gravity_constant, 2));

      // filter using exponential moving average
      punchForce = 0.5 * punchForce + 0.5 * punchForce;

      // general print for debugging
      // Serial.println(punchForce);
      updateLEDS(punchForce);

      // if punchForce is above threshold, calculate average punch force over punch duration
      if (punchForce > lower_punch_threshold) {
        float totalpunchforce = 0;
        float readValues = 0;

        // keep reading values until punch force is below threshold
        while (punchForce > lower_punch_threshold) {
          totalpunchforce += punchForce;
          readValues++;
          IMU.readAcceleration(ax, ay, az);
          prev_punch = punchForce;
          punchForce = punching_bag_mass * sqrt(pow((ax - ax_start) * gravity_constant, 2) + pow((ay - ay_start) * gravity_constant, 2) + pow((az - az_start) * gravity_constant, 2));
        }

        // calculate the average totalpunchforce force and send it over BLE
        punchForce = totalpunchforce / readValues;
        Serial.println("punch with totalpunchforce: " + String(punchForce));

        // count the number of punches (to be omitted in final version)
        punches++;
        Serial.println("punches: " + String(punches));

        // send data over BLE
        punch.writeValue(punchForce);
        Serial.print("Sent: ");
        Serial.println(punchForce);

        // play sound effect
        playSound();
      }
    }
  }
}

void updateLEDS(float punchForce) {
  // map the sensor value to the number of LEDs to light up
  targetLitLEDs = map(punchForce, lower_punch_threshold, upper_force_threshold, 0, ledCount / 2);
  if (targetLitLEDs < 0) {
    targetLitLEDs = 0;
  }
  // If the punchForce value is increasing, go up fast
  if (targetLitLEDs > currentLitLEDs) {
    currentLitLEDs = targetLitLEDs;
    updateLEDStrip(currentLitLEDs);
  }

  // If the punchForce value is decreasing, go down slowly
  else if (targetLitLEDs < currentLitLEDs) {
    currentMillis = millis();
    if (currentMillis - previousMillisLED >= ledInterval) {
      previousMillisLED = currentMillis;
      currentLitLEDs--;  // Go down one LED at a time (slowly)
      updateLEDStrip(currentLitLEDs);
    }
  }
}

void updateLEDStrip(int numLitLEDs) {
  for (int i = 0; i < ledCount; i++) {
    if (i < ledCount / 2 - numLitLEDs) {
      leds[i] = CRGB::Black;
    } else if (i < ledCount / 2 + numLitLEDs) {
      leds[i] = CRGB::Red;
    } else {
      leds[i] = CRGB::Black;
    }
  }

  FastLED.show();
}

void playSound() {
  int fileNumber = random(1, 18);
  myDFPlayer.playMp3Folder(fileNumber);
  Serial.print("Audio: ");
  Serial.println(fileNumber);
}

void FSR_reading() {
  sensorReading = analogRead(sensorPin);

  while (sensorReading > punchThreshold) {
    punching = true;
    sensorReading = analogRead(sensorPin);
    updateLEDS(punchForce);
    if (punchForce < sensorReading) {
      punchForce = sensorReading;
    }
    delay(10);
  }

  if (punching == true) {
    Serial.println(punchForce);
    updateLEDS(punchForce);
    // send data over BLE
    punch.writeValue(punchForce);
    Serial.print("Sent: ");
    Serial.println(punchForce);

    // play sound effect
    playSound();
    punchForce = 0;
    punching = false;
  }

  updateLEDS(punchForce);
  delay(10);
}
