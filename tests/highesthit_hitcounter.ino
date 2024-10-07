const int sensorPin = A0;
    int sensorValue = 0;
    int maxforce=0;
    int tresholdforce=200;
    bool punching= false;
    unsigned long hitcounter=0;

    void setup() {
      Serial.begin(9600);
      pinMode(sensorPin, INPUT);
    }

    void loop() {
      sensorValue = analogRead(sensorPin);
      Serial.println(sensorValue);
      delay(50);


      if (sensorValue>maxforce)
      {
        maxforce=sensorValue;
      }
      
      if (sensorValue>tresholdforce && !punching)
      {  
          punching=!punching;
      }

      if(sensorValue<150 && punching)
      {
        punching=!punching;
        hitcounter=hitcounter+1;
      }
      Serial.print("maxforce=");
      Serial.print(maxforce);
      Serial.print("hitcount=");
      Serial.println(hitcounter);
      
    }
