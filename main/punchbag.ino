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

/*
  Arduino LSM6DS3 - Simple Accelerometer

  This example reads the acceleration values from the LSM6DS3
  sensor and continuously prints them to the Serial Monitor
  or Serial Plotter.

  The circuit:
  - Arduino Uno WiFi Rev 2 or Arduino Nano 33 IoT

  created 10 Jul 2019
  by Riccardo Rizzo

  This example code is in the public domain.
*/

#include <Arduino_LSM6DS3.h>
#include <FastLED.h>
#include <ArduinoBLE.h>


// pins
#define ledPin 7

// define the BLE service
BLEService punchService("95ff7bf8-aa6f-4671-82d9-22a8931c5387");
BLEFloatCharacteristic punch("95ff7bf8-aa6f-4671-82d9-22a8931c5387", BLERead | BLENotify);

// Variables to save acceleration and gyro values
float ax, ay, az;

// Variables to store initial calibration values for acceleromter
float ax_start, ay_start, az_start;

float punching_bag_mass = 25; // Mass in kg
float gravity_constant = 9.81; // m/s²
float range_contant = 4; // Range setting of the ACC
float gravity_range = gravity_constant * range_contant; // Full (positive) range of ACC in m/s²
float register_range = pow(2, 15); // Full (positive) register range of the ACC register (16bit)
float force = 0;
const int lower_force_threshold = 100; // force threshold to light up the first LED (check serial to set right value) aka punch trehsold
const int upper_force_threshold = 400;
const long ledInterval = 50; // Set the interval between lighting up the LEDs

// LED strip variables
#define ledCount 120
CRGB leds[ledCount]; // Define the array of leds
int brightness = 100; // Set brightness level (0-255) - check for amps required
int currentLitLEDs = 0; // The number of LEDs currently lit up
int targetLitLEDs = 0; // The number of LEDs that should be lit up


void setup() {
  Serial.begin(9600);
  while (!Serial);

  if (!IMU.begin()) {
    Serial.println("Failed to initialize IMU!");

    while (1);
  }

  Serial.print("Accelerometer sample rate = ");
  Serial.print(IMU.accelerationSampleRate());
  Serial.println(" Hz");
  Serial.println();
  Serial.println("Acceleration in g's");
  Serial.println("X\tY\tZ");

  if (IMU.accelerationAvailable()) {
    IMU.readAcceleration(ax_start, ay_start, az_start);
  }

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

}

int punches = 0;

void loop() {
  float ax, ay, az;

      // Listen for BLE central
      BLEDevice central = BLE.central();
      if (central) {
        Serial.print("Connected to central: ");
        Serial.println(central.address());
        if (!central.connected()) {
          Serial.println("Disconnected from central");
        }
        while (central.connected()){
          if (IMU.accelerationAvailable()) {
            IMU.readAcceleration(ax, ay, az);

            float prev_force = force;
            float force = punching_bag_mass * sqrt(pow((ax-ax_start)*gravity_constant, 2) + pow((ay-ay_start)*gravity_constant, 2) + pow((az-az_start)*gravity_constant, 2));
            // Filter using exponential moving average
            force = 0.5*force + 0.5*prev_force;

            updateLEDS(force);

            // If force is above threshold calculate average force over punch duration
            if (force > lower_force_threshold){

              float totalforce = 0;
              float readValues = 0;
              // Keep reading values until force is below threshold
              while (force > lower_force_threshold){
                totalforce += force;
                readValues++;
                IMU.readAcceleration(ax, ay, az);
                prev_force = force;
                force = punching_bag_mass * sqrt(pow((ax-ax_start)*gravity_constant, 2) + pow((ay-ay_start)*gravity_constant, 2) + pow((az-az_start)*gravity_constant, 2));
              }

              force = totalforce/readValues;
              Serial.println("punch with force: " + String(force));
              punch.writeValue(force);
              
              punches++;
              Serial.println("punches: " + String(punches));
              delay(5);
            } 
          }
        }

      }
  
  // Poll for BLE events (this processes any pending BLE events)
  BLE.poll();
}

void updateLEDS(float force){
  // map the sensor value to the number of LEDs to light up
    targetLitLEDs = map(force, 20, 300, 0, ledCount / 2);
    if (targetLitLEDs < 0) {
      targetLitLEDs = 0;
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
