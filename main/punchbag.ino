/*
This is the master script for the punch bag.
In entails the following components:
- Arduino nano 33 IoT: the microcontroller
- Velostat: a force sensor that detects the force of the punch
- LED strip: light up from the center (led 60 of 120) and spreads outward depending on the force of the punch
- BLE connection to send the values to the robot

** LED **
- LED strip: WS2812 RGB LED strip
-  Requires 5V & 0.2W per LED = 4.8 A for 120 leds
- N = 120 leds on an original strip
- We ditched the Neopixel library because it was too slow
- The FastLED library works much better

** Wiring **
- Power:
  * External power to VIN Arduino
  * External power to LED strip
  * External power to (+) side Velostat

- Grounds
  * External GND to GND Arduino
  * External GND to GND LED strip
  * External GND to side 2 of the resistor 

- Signals
  * DIN of the LED strip to pin 7 of the Arduino
  * (-) side velostat to side 1 of resistor (150Ω, brown/green/brown)) 
  * side 1 of the resistor [to] A0
*/

// libraries
#include <FastLED.h>
#include <ArduinoBLE.h>

// BLE service
BLEService punchService("95ff7bf8-aa6f-4671-82d9-22a8931c5387");
BLEFloatCharacteristic punch("95ff7bf8-aa6f-4671-82d9-22a8931c5387", BLERead | BLENotify);

// pins
#define ledPin 7
#define ledCount 120
#define sensorPin A0

// configuration
const int lower_force_threshold = 200;
const long ledInterval = 0;
const long sensorInterval = 10;
int brightness = 100; // Set brightness level (0-255) - check for amps (adapter has 1A max)

// timers
unsigned long previousMillis = 0;

// Sensor readings
int sensorReading = 0;

// LED strip variables
CRGB leds[ledCount];
int currentLitLEDs = 0;
int targetLitLEDs = 0;

void setup() {
  // initialize Serial communication
  Serial.begin(9600);

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