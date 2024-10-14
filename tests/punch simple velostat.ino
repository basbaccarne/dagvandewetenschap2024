// This code tests how the punches are detected by the velostat
// Wiring:
// - (-) side of the velostat [to] side 1 of resistor (1.6 KΩ, brow/blue/red)) 
// - side 1 of the resistor [to] A0
// - side 2 of the resistor [to] GND
// - (+) side of the velostat [to] 3.3V

const int sensorPin = A0;
int sensorValue = 0;
unsigned long previousMillis = 0;
const long sensorInterval = 100;

void setup() {
  Serial.begin(9600);
  pinMode(sensorPin, INPUT);
}

void loop() {
  unsigned long currentMillis = millis();

  if (currentMillis - previousMillis >= sensorInterval) {
    previousMillis = currentMillis;
    sensorValue = analogRead(sensorPin);
    Serial.println(sensorValue);
  }
}