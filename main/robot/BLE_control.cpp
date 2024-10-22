// include the .h file
#include "BLE_control.h"

// define the service UIDs (change this for each set-up)
BLEService punchService("95ff7bf8-aa6f-4671-82d9-22a8931c5387");
BLEFloatCharacteristic punch("95ff7bf8-aa6f-4671-82d9-22a8931c5387", BLERead | BLENotify);

// define the constructor (for easier referencing in the .ino script)
BLEControl::BLEControl() {
}

// initialize the BLE connection
void BLEControl::begin() {
  if (!BLE.begin()) {
    Serial.println("Failed initializing BLE");
    while (1)
      ;
  }

  // Set advertised local name and services UUID
  BLE.scanForUuid(punchService.uuid());
  Serial.println("Bluetooth® device active, scanning for connections...");
}

// Read the BLE signal
void BLEControl::checkForSignal() {

  // Check for available peripherals
  if (!peripheral) {
    peripheral = BLE.available();
    if (peripheral) {
      Serial.print("Found ");
      Serial.print(peripheral.address());
      Serial.print(" '");
      Serial.print(peripheral.localName());
      Serial.print("' ");
      Serial.print(peripheral.advertisedServiceUuid());
      Serial.println();
      BLE.stopScan();
      peripheral.connect();
    }
  }

  if (peripheral && peripheral.connected()) {
    if (!characteristic) {
      if (peripheral.discoverAttributes()) {
        Serial.println("Attributes discovered");
        characteristic = peripheral.characteristic(punch.uuid());
        if (!characteristic.subscribe()) {
          peripheral.disconnect();
        }
      }
    }

    if (characteristic && characteristic.valueUpdated()) {
      // Get the value if there is an update
      float floatValue;
      characteristic.readValue(&floatValue, 4);
      Serial.println(floatValue);
    }
  }
}