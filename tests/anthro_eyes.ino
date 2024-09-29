// This code tests facial expressions on a 8x8 matrix
// expressions are stored as PROGMEM bitmaps
// https://adestefawp.wordpress.com/projects/led-eyes/
// https://xantorohara.github.io/led-matrix-editor/

#include <Adafruit_NeoMatrix.h>

const uint8_t PROGMEM eye_open[] = {
    0b11111111,
    0b11111111,
    0b11100111,
    0b11100111,
    0b11100111,
    0b11100111,
    0b11111111,
    0b11111111};

const uint8_t PROGMEM eye_closed[] = {
    0b11111111,
    0b11111111,
    0b11111111,
    0b11111111,
    0b11111111,
    0b11111111,
    0b11111111,
    0b11111111};

const uint8_t PROGMEM eye_halfopen[] = {
    0b11111111,
    0b11111111,
    0b11111111,
    0b11111111,
    0b11100111,
    0b11100111,
    0b11111111,
    0b11111111};

#define PIXEL_PIN 11
#define MATRIX_NUMBER_OF_COLUMNS 8
#define MATRIX_NUMBER_OF_ROWS 8

// configure the neopixel matrix
Adafruit_NeoMatrix matrix = Adafruit_NeoMatrix(8, 8, PIXEL_PIN, NEO_MATRIX_BOTTOM + NEO_MATRIX_LEFT + NEO_MATRIX_ROWS + NEO_MATRIX_ZIGZAG, NEO_GRB + NEO_KHZ800);

// set main color
uint32_t blue = matrix.Color(255, 0, 0);

void setup()
{
    matrix.begin();
    matrix.setBrightness(255); // 0 to 255
    matrix.show();             // Initialize all pixels to 'off'
}

void loop()
{
    matrix.fillScreen(0);
    matrix.drawBitmap(0, 0, eye_open, 8, 8, blue);
    matrix.show();
    delay(random(200, 3000));

    matrix.fillScreen(0);
    matrix.drawBitmap(0, 0, eye_halfopen, 8, 8, blue);
    matrix.show();
    delay(50);

    matrix.fillScreen(0);
    matrix.drawBitmap(0, 0, eye_closed, 8, 8, blue);
    matrix.show();
    delay(50);

    matrix.fillScreen(0);
    matrix.drawBitmap(0, 0, eye_halfopen, 8, 8, blue);
    matrix.show();
    delay(50);
}
