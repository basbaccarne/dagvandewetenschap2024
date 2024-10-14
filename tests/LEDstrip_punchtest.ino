// This script drives a LED strip up and down depending on the force detected by the velostat

// LED strip: WS2812 RGB LED strip
// Requires 5V & 0.2W per LED (works on Nano with 5V bridge enabled - only when powered on USB)
// N = 120 leds on an original strip

// We ditched the Neopixel library because it was too slow
// The FastLED library works much better

/*
Wiring:
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

// adapt to set thesholds & timers
const int lower_force_threshold = 200;
const long ledInterval = 0;
const long sensorInterval = 10;

unsigned long previousMillis = 0;


int sensorReading = 0;

CRGB leds[ledCount];
int currentLitLEDs = 0;
int targetLitLEDs = 0;

void setup() {
  Serial.begin(9600);
  FastLED.addLeds<NEOPIXEL, ledPin>(leds, ledCount);
  FastLED.clear();
  FastLED.show();
}

void loop() {
  unsigned long currentMillis = millis();

  if (currentMillis - previousMillis >= sensorInterval) {
    previousMillis = currentMillis;
    sensorReading = analogRead(sensorPin);
    Serial.print(sensorReading);
    targetLitLEDs = map(sensorReading, lower_force_threshold, 1023, 0, ledCount);
    if (targetLitLEDs < 0) {
      targetLitLEDs = 0;
    }
    Serial.print(",");
    Serial.println(targetLitLEDs);
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
  // Turn on LEDs below the current value and turn off others
  for (int i = 0; i < ledCount; i++) {
    if (i < numLitLEDs) {
      leds[i] = CRGB::Red;  // Set to red (or any other color)
    } else {
      leds[i] = CRGB::Black;  // Turn off the LED
    }
  }

  // Update the LED strip with the new values
  FastLED.show();
}