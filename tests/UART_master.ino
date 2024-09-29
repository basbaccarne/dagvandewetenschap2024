// This scripts tests UART (Tx/Rx) communication between two Arduinos. 
// Connect the TX pin of the master Arduino to the RX pin of the slave Arduino, 
// Connect the RX pin of the master Arduino to the TX pin of the slave Arduino. 
// Connect the GND pin of the master Arduino to GND TX pin of the slave Arduino. 

// Master Arduino
void setup() {
  Serial.begin(9600);
}

void loop() {
  int peakPressure = random(0, 255); 
  Serial.println(peakPressure);
  delay(1000);
}