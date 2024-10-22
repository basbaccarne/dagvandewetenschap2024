// include the .h file
#include "BLE_control.h"

// define the constructor (for easier referencing in the .ino script)
BLEControl::BLEControl() {
}

// define the service UIDs (change this for each set-up)
BLEService punchService("95ff7bf8-aa6f-4671-82d9-22a8931c5387");
BLEFloatCharacteristic punch("95ff7bf8-aa6f-4671-82d9-22a8931c5387", BLERead | BLENotify);

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
void BLEControl::readSignal() {

  // Check for available peripherals
  BLEDevice peripheral = BLE.available();
  Serial.println("looking for signals");
  if (peripheral) {
    Serial.print("Found ");
    Serial.print(peripheral.address());
    Serial.print(" '");
    Serial.print(peripheral.localName());
    Serial.print("' ");
    Serial.print(peripheral.advertisedServiceUuid());
    Serial.println();
    BLE.stopScan();
  }
  // Read the value of the characteristic
  peripheral.connect();
  if (peripheral) {
    Serial.println("Connected");

    // search for attributes
    if (peripheral.discoverAttributes()) {
      Serial.println("Attributes discovered");
      BLECharacteristic characteristic = peripheral.characteristic(punch.uuid());

      // subscribe to attribute
      if (!characteristic.subscribe()) {
        peripheral.disconnect();
      }

      // while connected check if there are updates
      while (characteristic) {
        if (characteristic.valueUpdated()) {
          // and get the value if there is an update
          float floatValue;
          characteristic.readValue(&floatValue, 4);
          Serial.println(floatValue);
        }
      }
    }
  }
}