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
        0b00111100,
        0b01111110,
        0b11111111,
        0b11100111,
        0b11100111,
        0b11111111,
        0b01111110,
        0b00111100},
    eye_closed_bmp[] = {
        0b00111100, 
        0b01111110, 
        0b11111111, 
        0b11111111, 
        0b11111111, 
        0b11111111, 
        0b01111110, 
        0b00111100}, 
    eye_halfopen_bmp[] = {
        0b00111100, 
        0b01111110, 
        0b11111111, 
        0b11111111, 
        0b11100111, 
        0b11111111, 
        0b01111110, 
        0b00111100},
    eye_upperleft_bmp[] = {
        0b00111100,
        0b01111110,
        0b11001111,
        0b11001111,
        0b11111111,
        0b11111111,
        0b01111110,
        0b00111100},
    eye_upperleft_halfopen_bmp[] = {
        0b00111100,
        0b01111110,
        0b11111111,
        0b11001111,
        0b11111111,
        0b11111111,
        0b01111110,
        0b00111100},
    eye_upperright_bmp[] = {
        0b00111100,
        0b01111110,
        0b11110011,
        0b11110011,
        0b11111111,
        0b11111111,
        0b01111110,
        0b00111100} ,    
    eye_upperright_halfopen_bmp[] = {
        0b00111100,
        0b01111110,
        0b11111111,
        0b11110011,
        0b11111111,
        0b11111111,
        0b01111110,
        0b00111100}     
      ;


Adafruit_8x8matrix matrix = Adafruit_8x8matrix();

void setup()
{
    Serial.begin(9600);
    matrix.begin(0x70); // I2C address
}

void loop()
{

    // central
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

    matrix.clear();
    matrix.drawBitmap(0, 0, eye_open_bmp, 8, 8, LED_ON);
    matrix.writeDisplay();
    delay(random(500, 1000));

    // left
    matrix.clear();
    matrix.drawBitmap(0, 0, eye_upperleft_bmp, 8, 8, LED_ON);
    matrix.writeDisplay();

    delay(random(2000, 4000));

    matrix.clear();
    matrix.drawBitmap(0, 0, eye_upperleft_halfopen_bmp, 8, 8, LED_ON);
    matrix.writeDisplay();

    delay(random(80, 120));

    matrix.clear();
    matrix.drawBitmap(0, 0, eye_closed_bmp, 8, 8, LED_ON);
    matrix.writeDisplay();

    delay(random(100, 130));

    matrix.clear();
    matrix.drawBitmap(0, 0, eye_upperleft_halfopen_bmp, 8, 8, LED_ON);
    matrix.writeDisplay();

    delay(random(80, 120));

    matrix.clear();
    matrix.drawBitmap(0, 0, eye_upperleft_bmp, 8, 8, LED_ON);
    matrix.writeDisplay();
    delay(random(500,1000));

    // central
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

    matrix.clear();
    matrix.drawBitmap(0, 0, eye_open_bmp, 8, 8, LED_ON);
    matrix.writeDisplay();
    delay(random(500, 1000));

    // right
    matrix.clear();
    matrix.drawBitmap(0, 0, eye_upperright_bmp, 8, 8, LED_ON);
    matrix.writeDisplay();

    delay(random(2000, 4000));

    matrix.clear();
    matrix.drawBitmap(0, 0, eye_upperright_halfopen_bmp, 8, 8, LED_ON);
    matrix.writeDisplay();

    delay(random(80, 120));

    matrix.clear();
    matrix.drawBitmap(0, 0, eye_closed_bmp, 8, 8, LED_ON);
    matrix.writeDisplay();

    delay(random(100, 130));

    matrix.clear();
    matrix.drawBitmap(0, 0, eye_upperright_halfopen_bmp, 8, 8, LED_ON);
    matrix.writeDisplay();

    delay(random(80, 120));

    matrix.clear();
    matrix.drawBitmap(0, 0, eye_upperright_bmp, 8, 8, LED_ON);
    matrix.writeDisplay();
    delay(random(500,1000));
    
}