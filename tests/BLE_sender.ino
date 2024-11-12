/*
This is a sample script to make two Arduino Nano 33IOTs talk to each other over BLE (Bluetooth Low Energy)
BLE logic:
* https://docs.arduino.cc/tutorials/nano-33-iot/bluetooth/
* A central Bluetooth® device is designed to read data from peripheral devices.
* Peripheral devices continuously post data for other devices to read
* BLE works using 'services, which are a kind of data packages. Central devices can 'subscribe' on such services
* Each 'service' has 'characteristics', which are the variables themselves
* 'Characteristics' are identified by Unique Universal Identifiers (UUIDs)
* Search for services that match your need or create a custom service
* https://www.bluetooth.com/specifications/assigned-numbers/
* (for this project we are using a custom service with a custom UUID)
*/

// Code for the sender (peripheral)
// sending tha status of a button

// libraries
#include <ArduinoBLE.h>

// BLE information
BLEService punchService("95ff7bf8-aa6f-4671-82d9-22a8931c5387");
BLEFloatCharacteristic punch("95ff7bf8-aa6f-4671-82d9-22a8931c5387", BLERead | BLENotify);

// Button pin (pullup)
int button = 2;

// dummy variable
int pressure = 0;

void setup() {
  pinMode(button, INPUT_PULLUP);

  // initialize BLE: part 1
  BLE.begin();

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

  // update values
  measure_dummy();

  // Check BLE connection
  BLEDevice central = BLE.central();

  // Send data if there is a connection
  if (central) {
    if (central.connected()) {
      // send the sensor value over BLE
      punch.writeValue(pressure);
      Serial.print("Sent: ");
      Serial.println(pressure);
    } 
  }
  
  // Let us know if the connection fails
  if(!central){
    Serial.println("Disconnected from central");
  }

  // Poll for BLE events (this processes any pending BLE events)
  BLE.poll();

  // delay for stability
  delay(10);
}

void measure_dummy() {
  pressure = digitalRead(button);
  delay(10);
}