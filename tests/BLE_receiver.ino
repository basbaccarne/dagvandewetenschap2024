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

// Code for the receiver (central)

#include <ArduinoBLE.h>
BLEService punchService("95ff7bf8-aa6f-4671-82d9-22a8931c5387");
BLEFloatCharacteristic punch("95ff7bf8-aa6f-4671-82d9-22a8931c5387", BLERead | BLENotify);

void setup()
{
    Serial.begin(9600);

    // Initialize BLE
    if (!BLE.begin())
    {
        Serial.println("Failed initializing BLE");
        while (1)
            ;
    }

    // Set advertised local name and services UUID
    BLE.scanForUuid(punchService.uuid());
    Serial.println("Bluetooth® device active, scanning for connections...");
}

void loop()
{
    // Check for available peripherals
    BLEDevice peripheral = BLE.available();
    if (peripheral)
    {
        Serial.print("Found ");
        Serial.print(peripheral.address());
        Serial.print(" '");
        Serial.print(peripheral.localName());
        Serial.print("' ");
        Serial.print(peripheral.advertisedServiceUuid());
        Serial.println();
        BLE.stopScan();
    }

    // connect to the peripheral
    peripheral.connect();
    if (peripheral)
    {
        Serial.println("Connected");

        // search for attributes
        if (peripheral.discoverAttributes())
        {
            Serial.println("Attributes discovered");
            BLECharacteristic characteristic = peripheral.characteristic(punch.uuid());

            // subscribe to attribute
            if (!characteristic.subscribe())
            {
                peripheral.disconnect();
            }

            // while connected check if there are updates
            while (characteristic)
            {
                if (characteristic.valueUpdated())
                {
                    // and get the value if there is an update
                    float floatValue;
                    characteristic.readValue(&floatValue, 4);
                    Serial.println(floatValue);
                }

                // set responsiveness
                delay(200);
            }
        }
    }

    // disconnect
    Serial.print("Disconnected from peripheral: ");
    Serial.println(peripheral.address());
    delay(500);
}