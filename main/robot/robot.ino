/*
 This is the main robot script. The script entails the following modules:
 - BLE control to reveice pressure readings from the punchbag
- Speaker control to play instructions
 - Motor control to move the robot head
 - LED control to light up the robot eyes
 - Protopie Control to send instructions to the Protopie app

The program is organized across a switch case state machine. The states are:
- BOOTING: the start up and calibration state, right after the robot is turned on
- IDLE: the initial state, waiting for a signal to start
- WELCOME: the state where the robot welcomes the user and gives the first instructions
- CHALLENGE1: the first challenge state (punch as hard as you can)
- CHALLENGE1_DEBRIEF: the debrief state after the first challeng
- CHALLENGE2: the second challenge state (punch as fast as you can)
- CHALLENGE2_DEBRIEF: the debrief state after the second challenge
- CHALLENGE3: the third challenge state (punch as accurate/steady as you can)
- CHALLENGE3_DEBRIEF: the debrief state after the third challenge
- CHALLENGE4: the fourth challenge state (punch as long as you can
- CHALLENGE4_DEBRIEF: the debrief state after the fourth challenge
- CONCLUSION: the final state, where the robot gives the final instructions and the user can leave > goes back the IDLE state
*/

// Modules
#include "BLE_control.h"
BLEControl bleControl;

#include "calibrate.h"
calibrate calibrate;

// States
enum State {
  BOOTING,
  IDLE,
  WELCOME,
  CHALLENGE1,
  CHALLENGE1_DEBRIEF,
  CHALLENGE2,
  CHALLENGE2_DEBRIEF,
  CHALLENGE3,
  CHALLENGE3_DEBRIEF,
  CHALLENGE4,
  CHALLENGE4_DEBRIEF,
  CONCLUSION
};
State currentState = BOOTING;
State previousState = CONCLUSION;

// Delays and timers
unsigned long previousMillis_BLE = 0;
int BLEInterval = 200;

unsigned long calibration_time = 5000;

// State overarchin variables
float punch_baseline = 0;
float punch_threshold = 500;

void setup() {
  Serial.begin(9600);
  bleControl.begin();
}

void loop() {
  // general timer for the delays
  unsigned long currentMillis = millis();

  // the big state machine!
  switch (currentState) {
    case BOOTING:
      // status update
      if (previousState != BOOTING) {
        Serial.print("state||");
        Serial.println("BOOTING");
        Serial.println("Booting ...");
        previousState = BOOTING;
      }
      // get punch value
      if (currentMillis - previousMillis_BLE >= BLEInterval) {
        previousMillis_BLE = currentMillis;
        bleControl.checkForSignal();
        float value = bleControl.getFloatValue();
        Serial.print("punch_value||");
        Serial.println(value);

        // calibrate
        if (currentMillis < calibration_time) {
          calibrate.gather(value);
          punch_baseline = calibrate.getBaseline();
          Serial.print("baseline||");
          Serial.println(punch_baseline);
        }

        // go to the next state
        else {
          currentState = IDLE;
        }
      }

      break;

    case IDLE:
      // status update
      if (previousState != IDLE) {
        Serial.print("Punch baseline set at: ");
        Serial.println(punch_baseline);
        Serial.print("state||");
        Serial.println("IDLE");
        Serial.println("Ready for action! Waiting for the first punch ...");
        previousState = IDLE;
      }

      // get punch value
      if (currentMillis - previousMillis_BLE >= BLEInterval) {
        previousMillis_BLE = currentMillis;
        bleControl.checkForSignal();
        float value = bleControl.getFloatValue();
        Serial.print("punch_value||");
        Serial.println(value);

        if (value > punch_threshold) {
          currentState = WELCOME;
        }
      }
      break;

    case WELCOME:
      // status update
      if (previousState != WELCOME) {
        Serial.print("state||");
        Serial.println("WELCOME");
        Serial.println("Welcome! ...");
        previousState = WELCOME;
      }

      // Welcome message
      // play welcome message
      // ...

      break;

      
  }

  // ... end of the state machine
  // below are the constantly running processes

  // robot head movements
  // ...

  // robot eye movement
  // ...

}