/*
Simple test code to test the HC-05 module
HC-05 vs HC-06: HC-06 can only be a slave, while HC-05 can be both master and slave.
Communication over Serial

Always check voltage!

Wiring:
- VCC to 5V
- GND to GND
- TX to RX
- RX to TX
- (optional) STATE to pin 7 (to check if the module is connected)

*/

/*
Pairing challenge:
To pair multiple diadic pairs of HC-05 Bluetooth modules, you need to configure each pair separately using AT commands.

    Enter AT Command Mode:
        Connect the HC-05 module to your Arduino.
        Hold the EN pin button while powering the module to enter AT command mode. 
        The LED should blink slowly, indicating that you are in AT command mode.

    Configure the Slave Module:
        Open the Serial Monitor in the Arduino IDE with a baud rate of 38400 and both NL & CR selected.
        Send the following AT commands to configure the slave module:

        AT+ROLE=0       // Set the module as a slave
        AT+ADDR?        // Get the address of the slave module

        Note down the address of the slave module.

    Configure the Master Module:
        Connect the master module to your Arduino and enter AT command mode.
        Send the following AT commands to configure the master module:

        AT+ROLE=1       // Set the module as a master
        AT+CMODE=0      // Set the connection mode to fixed address
        AT+BIND=<slave_address>  // Bind the master to the slave's address (Replace <slave_address> with the address you noted from the slave module)

    Repeat for Each Pair:
        Repeat the above steps for each pair of HC-05 modules, ensuring that each master is bound to its corresponding slave.

    Exit AT Command Mode:
        Once configured, power cycle the modules to exit AT command mode and enter normal operation mode.
*/

// this is the script for the sender (slave)

void setup() {
  Serial.begin(9600);
  // open commz with the BT module
  Serial1.begin(9600); // check the baud rate of the  module
  Serial.println("Slave ready");
}

void loop() {
  if (Serial1.available()) {
    String command = Serial1.readString();
    if (command == "READ") {
      int randomNumber = random(0, 100); // Generate random number between 0 and 99
      Serial1.println(String(randomNumber)); // Send random number to master
      Serial.println("Sent data: " + String(randomNumber));
    }
  }
}