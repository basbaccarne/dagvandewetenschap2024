// This code tests how the punches are detected by the FSR
// Wiring:
// - (-) side of the velostat [to] side 1 of resistor (1.6 KΩ, brow/blue/red)) 
// - side 1 of the resistor [to] A0
// - side 2 of the resistor [to] GND
// - (+) side of the velostat [to] 3.3V

// This code calibrates the pressure sensor, and sends the peak pressure to the serial monitor.
// Punch as hard as you can!

// pins
const int sensorPin = A0;

// variables for the sensor readings
int sensorReading = 0;
int punchThreshold = 50;
int peakReading = 0;
bool punching = false;

bool sending = false;

void setup() {
  pinMode(A0, INPUT);
  Serial.begin(9600);
}

void loop() {
  FSR_reading();
}


void FSR_reading() {
  sensorReading = analogRead(sensorPin);

  while (sensorReading > punchThreshold) {
    punching = true;
    sensorReading = analogRead(sensorPin);
    if (peakReading < sensorReading) {
      peakReading = sensorReading;
    }
    delay(10);
  }

  if (punching == true) {
    Serial.println(peakReading);
    peakReading = 0;
    punching = false;
  }
  delay(10);
}
