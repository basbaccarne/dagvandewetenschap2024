// This script drives a LED strip up and down depending on the force detected by the velostat

// LED strip: WS2812 RGB LED strip
// Requires 5V & 0.2W per LED (works on Nano with 5V bridge enabled - only when powered on USB)
// N = 120 leds on an original strip

// We ditched the Neopixel library because it was too slow
// The FastLED library works much better

/*
Wiring:
- Connect the 5V of the LED strip to an external 5V power source
- Connect the GND of the LED strip to the GND of the Arduino
- Connect the DIN of the LED strip to pin 7 of the Arduino
- Connect the GND of the external power source to the GND of the Arduino

- connect (-) side of the velostat [to] side 1 of resistor (1.6 KΩ, brow/blue/red)) 
- connect side 1 of the resistor [to] A0
- connect side 2 of the resistor [to] GND
- connect (+) side of the velostat [to] 3.3V
*/

#include <FastLED.h>

#define ledPin 7
#define ledCount 120
#define sensorPin A0

int sensorReading = 0;
int mappedPressure = 0;

CRGB leds[ledCount];
int currentLitLEDs = 0;
int targetLitLEDs = 0;  

void setup() {
  pinMode(ledPin, INPUT);
  Serial.begin(9600);

  FastLED.addLeds<NEOPIXEL, ledPin>(leds, ledCount);
  FastLED.clear();
  FastLED.show(); 
}

void loop() {

  sensorReading = analogRead(sensorPin);
  Serial.print(sensorReading);
  targetLitLEDs  = map(sensorReading, 200, 1023, 0, ledCount);
  Serial.print(" | ");
  Serial.println(mappedPressure);

  // If the force value is increasing, go up fast
  if (targetLitLEDs > currentLitLEDs) {
    currentLitLEDs = targetLitLEDs;
    updateLEDStrip(currentLitLEDs);
  }

  // If the force value is decreasing, go down slowly
  else if (targetLitLEDs < currentLitLEDs) {
    currentLitLEDs--;  // Go down one LED at a time (slowly)
    updateLEDStrip(currentLitLEDs);
    delay(10);  // Slow down the decrease
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