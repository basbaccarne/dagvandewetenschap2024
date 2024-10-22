// this script is a simple sender script
// the Arduino sends values over BLE and pulses a LED to check the sending status

#include <ArduinoBLE.h>
BLEService punchService("95ff7bf8-aa6f-4671-82d9-22a8931c5387");
BLEFloatCharacteristic punch("95ff7bf8-aa6f-4671-82d9-22a8931c5387", BLERead | BLENotify);

// timers
unsigned long previousMillis = 0;
const int delay_ms = 10;
int ledState = LOW;

// dummy variable
int pressure = 0;
bool goingUp = true;

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);

  // initialize BLE: part 1
  if (!BLE.begin()) {
    Serial.println("BLE failed to Initiate");
    delay(500);
    while (1)
      ;
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
  unsigned long currentMillis = millis();

  if (currentMillis - previousMillis >= delay_ms) {
    previousMillis = currentMillis;

    measure_dummy();
    Serial.println(pressure);

    // send the sensor value over BLE
    punch.writeValue(pressure);

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
}

void measure_dummy() {
  if (pressure > 1023) {
    goingUp = false;
  }
  if (pressure < 0) {
    goingUp = true;
  }

  if (goingUp) {
    pressure = pressure + 10;
    digitalWrite(LED_BUILTIN, HIGH);
  } else {
    pressure = pressure - 10;
    digitalWrite(LED_BUILTIN, LOW);
  }
}
