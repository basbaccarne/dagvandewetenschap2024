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

#include <ArduinoBLE.h>
BLEService punchService("95ff7bf8-aa6f-4671-82d9-22a8931c5387");
BLEFloatCharacteristic punch("95ff7bf8-aa6f-4671-82d9-22a8931c5387", BLERead | BLENotify);

void setup()
{
    Serial.begin(9600);

    // Initialize BLE
    if (!BLE.begin())
    {
        Serial.println("BLE failed to Initiate");
        delay(500);
        while (1)
            ;
    }

    // Set advertised local name and services UUID
    BLE.setLocalName("BLE Experiment (peripheral)");
    BLE.setAdvertisedService(punchService);
    punchService.addCharacteristic(punch);
    BLE.addService(punchService);

    // Set default values for characteristic
    punch.writeValue(0.0);

    // Start advertising
    BLE.advertise();
    Serial.println("Bluetooth® sending device active, waiting for connections...");
}

void loop()
{
    // Listen for BLE central
    BLEDevice central = BLE.central();
    if (central)
    {
        Serial.print("Connected to central: ");
        Serial.println(central.address());

        // While central is still connected...
        while (central.connected())
        {
            // generate random value
            float pressure;
            pressure = random(0, 100) / 10.0;
            // print & send over BLE
            Serial.print("Pressure ");
            Serial.println(pressure);
            punch.writeValue(pressure);
            // delay
            delay(500);
        }
    }
    // when the central disconnects, print disconnection message
    Serial.print("Disconnected from central: ");
    Serial.println(central.address());
    delay(1000);
}