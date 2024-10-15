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
  * (-) side velostat to side 1 of resistor (1.6 KΩ, brow/blue/red)) 
  * side 1 of the resistor [to] A0
*/

#include <FastLED.h>

#define ledPin 7
#define ledCount 120
#define sensorPin A0

const int lower_force_threshold = 200;
const long ledInterval = 0;
const long sensorInterval = 10;
int brightness = 100; // Set brightness level (0-255) - check for amps (adapter has 1A max)

unsigned long previousMillis = 0;

int sensorReading = 0;

CRGB leds[ledCount];
int currentLitLEDs = 0;
int targetLitLEDs = 0;

void setup() {
  Serial.begin(9600);
  FastLED.addLeds<NEOPIXEL, ledPin>(leds, ledCount);
  FastLED.setBrightness(brightness); 
  FastLED.clear();
  FastLED.show();
}

void loop() {
  unsigned long currentMillis = millis();

  if (currentMillis - previousMillis >= sensorInterval) {
    previousMillis = currentMillis;
    sensorReading = analogRead(sensorPin);
    Serial.print(sensorReading);
    targetLitLEDs = map(sensorReading, lower_force_threshold, 1023, 0, ledCount / 2);
    if (targetLitLEDs < 0) {
      targetLitLEDs = 0;
    }
    Serial.print(",");
    Serial.print(targetLitLEDs);
    int leftLeds = ledCount / 2 + targetLitLEDs;
    Serial.print(",");
    Serial.print(leftLeds);
    int rightLeds = ledCount / 2 - targetLitLEDs;
    Serial.print(",");
    Serial.println(rightLeds);
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