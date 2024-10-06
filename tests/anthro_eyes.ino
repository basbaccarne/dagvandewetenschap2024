/*
This code tests facial expressions on a 8x8 matrix
- The robot has two eyes, which are 2 coupled 8x8 matrices

expressions are stored as PROGMEM bitmaps
- https://adestefawp.wordpress.com/projects/led-eyes/
- https://xantorohara.github.io/led-matrix-editor/

Information on the Adafruit 8x8 matrix:
- https://www.adafruit.com/product/870
- The backpacks come with address-selection jumpers so you can connect up to four mini 8x8's on a single I2C bus
- The adafuit 8x8 matrix works with the backpack library

wiring:
+  ---------------------->  Arduino 3.3V
-  ---------------------->  Arduino GND
D  ---------------------->  Arduino SDA
C  ---------------------->  Arduino SCL
>> Chain second 8x8 matrix to the first (+ to +, - to -, D to D, C to C)
*/

#include <Adafruit_GFX.h>
#include <Adafruit_LEDBackpack.h>

// emotions
static const uint8_t PROGMEM
    eye_open_bmp[] = {
        0b11111111,
        0b11111111,
        0b11000011,
        0b11000011,
        0b11000011,
        0b11000011,
        0b11111111,
        0b11111111},
    eye_closed_bmp[] = {
        0b11111111, 
        0b11111111, 
        0b11111111, 
        0b11111111, 
        0b11111111, 
        0b11111111, 
        0b11111111, 
        0b11111111
        }, 
    eye_halfopen_bmp[] = {
        0b11111111, 
        0b11111111, 
        0b11111111, 
        0b11111111, 
        0b11000011, 
        0b11000011, 
        0b11111111, 
        0b11111111
        };

Adafruit_8x8matrix matrix = Adafruit_8x8matrix();

void setup()
{
    Serial.begin(9600);
    matrix.begin(0x70); // I2C address
}

void loop()
{

    matrix.clear();
    matrix.drawBitmap(0, 0, eye_open_bmp, 8, 8, LED_ON);
    matrix.writeDisplay();

    delay(random(2000, 4000));

    matrix.clear();
    matrix.drawBitmap(0, 0, eye_halfopen_bmp, 8, 8, LED_ON);
    matrix.writeDisplay();

    delay(random(80, 120));

    matrix.clear();
    matrix.drawBitmap(0, 0, eye_closed_bmp, 8, 8, LED_ON);
    matrix.writeDisplay();

    delay(random(100, 130));

    matrix.clear();
    matrix.drawBitmap(0, 0, eye_halfopen_bmp, 8, 8, LED_ON);
    matrix.writeDisplay();

    delay(random(80, 120));
}