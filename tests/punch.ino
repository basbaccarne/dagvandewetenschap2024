// This code calibrates the pressure sensor, and sends the peak pressure to the serial monitor.
// Punch as hard as you can!

// pins
const int sensorPin = A0;

// variables for the sensor readings
int sensorReading = 0; // main reading
int peakReading = 0;   // peak reading
int sensorMin = 1023;  // reverse defaults for calibration
int sensorMax = 0;     // reverse defaults for calibration
bool sending = false;

void setup()
{
    pinMode(A0, INPUT);
    Serial.begin(9600);

    // calibrate the sensor for [[5 seconds]]
    // the sensor will calibrate to the peak and bottom pressure
    Serial.print("Calibrating for 5 seconds, punch as hard as you can ...");
    while (millis() < 5000)
    {
        calibrate();
    }
    Serial.println("");
    Serial.println("Punch Sensor Calibrated.");
    Serial.print("Sensor Min: ");
    Serial.println(sensorMin);
    Serial.print("Sensor Max: ");
    Serial.println(sensorMax);
}

void loop()
{
    // read and map the presure data
    sensorReading = analogRead(sensorPin);
    int calibratedPressure = map(sensorReading, sensorMin, sensorMax, 0, 255);
    // Serial.print("Calibrated Pressure: ");
    // Serial.println(calibratedPressure);

    // Calculate the moving peak (trigger when punched)
    int noiseThreshold = sensorMax / 100; // adjust trigger threshold if needed
    if (calibratedPressure > sensorMin + noiseThreshold)
    {
        if (calibratedPressure > peakReading)
        {
            peakReading = calibratedPressure;
        }
        sending = true;
    }
    else
    {
        if (sending)
        {
            Serial.print("[!] Peak Value: ");
            Serial.println(peakReading);
            peakReading = calibratedPressure;
            sending = false;
        }
    }

    delay(10); // adjust for accuracy
}

void calibrate()
{
    sensorReading = analogRead(sensorPin);
    if (sensorReading > sensorMax)
    {
        sensorMax = sensorReading;
    }
    if (sensorReading < sensorMin)
    {
        sensorMin = sensorReading;
    }
}


void ledDriver(reading){
  
}