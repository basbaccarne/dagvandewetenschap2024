// This is a test script to show rainbow colors on the LED strip

// LED strip: WS2812 RGB LED strip
// Requires 5V & 0.2W per LED (works on Nano with 5V enabled)
// N = 120 leds on an original strip

/*
Wiring:
- Connect the 5V of the LED strip to an external 5V power source
- Connect the GND of the LED strip to the GND of the Arduino
- Connect the DIN of the LED strip to pin 7 of the Arduino
- Connect the GND of the external power source to the GND of the Arduino
*/

#include <Adafruit_NeoPixel.h>

#define PIN 7      
#define LED_COUNT 120

Adafruit_NeoPixel leds = Adafruit_NeoPixel(LED_COUNT, PIN, NEO_GRB + NEO_KHZ800);

void setup()
{
  leds.begin();
  clearLEDs();
  leds.show();
}

void loop()
{
    for (int i=0; i<LED_COUNT; i++)
  {
    rainbow(i);
    delay(10);
  }
}

void clearLEDs()
{
  for (int i=0; i<LED_COUNT; i++)
  {
    leds.setPixelColor(i, 0);
  }
}

void rainbow(byte startPosition) 
{
  int rainbowScale = 192 / LED_COUNT;
  for (int i=0; i<LED_COUNT; i++)
  {
    leds.setPixelColor(i, rainbowOrder((rainbowScale * (i + startPosition)) % 192));
  }
  leds.show();
}

uint32_t rainbowOrder(byte position) 
{
  if (position < 31)
  {
    return leds.Color(0xFF, position * 8, 0);
  }
  else if (position < 63)
  {
    position -= 31;
    return leds.Color(0xFF - position * 8, 0xFF, 0);
  }
  else if (position < 95)
  {
    position -= 63;
    return leds.Color(0, 0xFF, position * 8);
  }
  else if (position < 127)
  {
    position -= 95;
    return leds.Color(0, 0xFF - position * 8, 0xFF);
  }
  else if (position < 159)
  {
    position -= 127;
    return leds.Color(position * 8, 0, 0xFF);
  }
  else
  {
    position -= 159;
    return leds.Color(0xFF, 0x00, 0xFF - position * 8);
  }
}