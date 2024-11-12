/*
So this is the problem: the Nano only has one set of serial pins, so you can't use the hardware serial for both the HC-05 and the computer. You can use the SoftwareSerial library to create a second serial port.
SofwareSerial is not available for the Nano 33 IOT. However,
it's possible to create additional hardware serial ports using the microcontroller's spare SERCOM (Serial Communication) modules.
These modules can be configured as additional Serial, SPI or I2C comms ports.
Extra documentation can be found here: https://learn.adafruit.com/using-atsamd21-sercom-to-add-more-spi-i2c-serial-ports/overview

In a netshell:
- the SAMD21 has 6 SERCOMs (SERial COMmunication modules)
- optional pads
    D0 (RX) and D1 (TX): default serial pins for Serial1.
    D2 and D3
    D4 and D5
    D6 and D7
    D8 and D9
    D10 and D11
*/

#include <Arduino.h>
#include "wiring_private.h" 

// We make D10 = TX and D11 = RX
// These will be accessed through Serial3
Uart Serial3 (&sercom1, 11, 10, SERCOM_RX_PAD_0, UART_TX_PAD_2);
void SERCOM1_Handler() {
  Serial3.IrqHandler();
}

void setup() {
  // open Serial Communication
  Serial.begin(115200);
  Serial3.begin(115200);

  // Assign pins 10 & 11 SERCOM functionality
  pinPeripheral(10, PIO_SERCOM);
  pinPeripheral(11, PIO_SERCOM);

}

void loop() {
 //
}