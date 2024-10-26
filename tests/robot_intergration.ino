/* integrated eye and head movement

wiring:
Power
- External 5V Power Supply to breadboard (5V)
- 5V to Artduino Vin
- 5V to VCC led matrici
- 5V to servo's

GND
- External power GND to breadboard
- GND to Arduino GND
- GND to GND led matrici
- GND tot GND servo's

Singals:
- D12 & D11 to servo signals
- SDA & SCL to led matrici SDA & SCL
*/

// libraries
#include <Adafruit_GFX.h>
#include <Adafruit_LEDBackpack.h>
#include <Servo.h>

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
        0b00111100};   

// led control object
Adafruit_8x8matrix matrix = Adafruit_8x8matrix();

// servo objects
Servo servoV;
Servo servoH;

// initial servo values (go from 0-120)
int posV = 60;
int posH = 60;

// timers
unsigned long previousMillis_eyes = 0;
unsigned long interval_eyes = 0;
int state_eyes = 0;
unsigned long previousMillis_head = 0;
unsigned long interval_head = 200;


void setup() {
  Serial.begin(9600);
  matrix.begin(0x70);  // I2C address of the LED matrix

  servoV.attach(12);
  servoH.attach(11);
}

void loop() {
  // general timer for the delays
  unsigned long currentMillis = millis();

  if(currentMillis - previousMillis_head >= interval_head){
      randomHeadMovement();
      previousMillis_head = currentMillis;
  }

  if (currentMillis - previousMillis_eyes >= interval_eyes)
    {
        previousMillis_eyes = currentMillis;

        switch (state_eyes)
        {
        case 0:
            matrix.clear();
            matrix.drawBitmap(0, 0, eye_open_bmp, 8, 8, LED_ON);
            matrix.writeDisplay();
            interval_eyes = random(2000, 4000);
            state_eyes = 1;
            break;
        case 1:
            matrix.clear();
            matrix.drawBitmap(0, 0, eye_halfopen_bmp, 8, 8, LED_ON);
            matrix.writeDisplay();
            interval_eyes = random(80, 120);
            state_eyes = 2;
            break;
        case 2:
            matrix.clear();
            matrix.drawBitmap(0, 0, eye_closed_bmp, 8, 8, LED_ON);
            matrix.writeDisplay();
            interval_eyes = random(100, 130);
            state_eyes = 3;
            break;
        case 3:
            matrix.clear();
            matrix.drawBitmap(0, 0, eye_halfopen_bmp, 8, 8, LED_ON);
            matrix.writeDisplay();
            interval_eyes = random(80, 120);
            state_eyes = 4;
            break;
        case 4:
            matrix.clear();
            matrix.drawBitmap(0, 0, eye_open_bmp, 8, 8, LED_ON);
            matrix.writeDisplay();
            interval_eyes = random(500, 1000);
            state_eyes = 5;
            break;
        case 5:
            matrix.clear();
            matrix.drawBitmap(0, 0, eye_upperleft_bmp, 8, 8, LED_ON);
            matrix.writeDisplay();
            interval_eyes = random(2000, 4000);
            state_eyes = 6;
            break;
        case 6:
            matrix.clear();
            matrix.drawBitmap(0, 0, eye_upperleft_halfopen_bmp, 8, 8, LED_ON);
            matrix.writeDisplay();
            interval_eyes = random(80, 120);
            state_eyes = 7;
            break;
        case 7:
            matrix.clear();
            matrix.drawBitmap(0, 0, eye_closed_bmp, 8, 8, LED_ON);
            matrix.writeDisplay();
            interval_eyes = random(100, 130);
            state_eyes = 8;
            break;
        case 8:
            matrix.clear();
            matrix.drawBitmap(0, 0, eye_upperleft_halfopen_bmp, 8, 8, LED_ON);
            matrix.writeDisplay();
            interval_eyes = random(80, 120);
            state_eyes = 9;
            break;
        case 9:
            matrix.clear();
            matrix.drawBitmap(0, 0, eye_upperleft_bmp, 8, 8, LED_ON);
            matrix.writeDisplay();
            interval_eyes = random(500, 1000);
            state_eyes = 10;
            break;
        case 10:
            matrix.clear();
            matrix.drawBitmap(0, 0, eye_open_bmp, 8, 8, LED_ON);
            matrix.writeDisplay();
            interval_eyes = random(2000, 4000);
            state_eyes = 11;
            break;
        case 11:
            matrix.clear();
            matrix.drawBitmap(0, 0, eye_halfopen_bmp, 8, 8, LED_ON);
            matrix.writeDisplay();
            interval_eyes = random(80, 120);
            state_eyes = 12;
            break;
        case 12:
            matrix.clear();
            matrix.drawBitmap(0, 0, eye_closed_bmp, 8, 8, LED_ON);
            matrix.writeDisplay();
            interval_eyes = random(100, 130);
            state_eyes = 13;
            break;
        case 13:
            matrix.clear();
            matrix.drawBitmap(0, 0, eye_halfopen_bmp, 8, 8, LED_ON);
            matrix.writeDisplay();
            interval_eyes = random(80, 120);
            state_eyes = 14;
            break;
        case 14:
            matrix.clear();
            matrix.drawBitmap(0, 0, eye_open_bmp, 8, 8, LED_ON);
            matrix.writeDisplay();
            interval_eyes = random(500, 1000);
            state_eyes = 15;
            break;
        case 15:
            matrix.clear();
            matrix.drawBitmap(0, 0, eye_upperright_bmp, 8, 8, LED_ON);
            matrix.writeDisplay();
            interval_eyes = random(2000, 4000);
            state_eyes = 16;
            break;
        case 16:
            matrix.clear();
            matrix.drawBitmap(0, 0, eye_upperright_halfopen_bmp, 8, 8, LED_ON);
            matrix.writeDisplay();
            interval_eyes = random(80, 120);
            state_eyes = 17;
            break;
        case 17:
            matrix.clear();
            matrix.drawBitmap(0, 0, eye_closed_bmp, 8, 8, LED_ON);
            matrix.writeDisplay();
            interval_eyes = random(100, 130);
            state_eyes = 18;
            break;
        case 18:
            matrix.clear();
            matrix.drawBitmap(0, 0, eye_upperright_halfopen_bmp, 8, 8, LED_ON);
            matrix.writeDisplay();
            interval_eyes = random(80, 120);
            state_eyes = 19;
            break;
        case 19:
            matrix.clear();
            matrix.drawBitmap(0, 0, eye_upperright_bmp, 8, 8, LED_ON);
            matrix.writeDisplay();
            interval_eyes = random(500, 1000);
            state_eyes = 0;
            break;
        }
    }
}

void randomHeadMovement(){
  int DoesItMove = random(0,100);
  if(DoesItMove > 50){
    posV = random(40,80);
    posH = random(40,80);
    servoV.write(posV);
    servoH.write(posH);
  }
}