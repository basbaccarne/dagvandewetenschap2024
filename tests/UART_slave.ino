// This scripts tests UART (Tx/Rx) communication between two Arduinos. 
// Connect the TX pin of the master Arduino to the RX pin of the slave Arduino, 
// Connect the RX pin of the master Arduino to the TX pin of the slave Arduino. 
// Connect the GND pin of the master Arduino to GND TX pin of the slave Arduino. 

// Slave Arduino
void setup() {
  Serial.begin(9600);
}

void loop() {
  if (Serial.available() > 0) { // Check if data is available to read
    String message = Serial.readString(); // Read the incoming message
    Serial.println("Received: " + message); // Print the received message
  }
}