/*
 This is the main robot script. The script entails the following modules:
 - BLE control to reveice pressure readings from the punchbag
 - Speaker control to play instructions
 - Motor control to move the robot head
 - LED control to light up the robot eyes
 - Protopie Control to send instructions to the Protopie app

The program is organized across a switch case state machine. The states are:
- BOOTING: the start up and calibration state, right after the robot is turned
on
- IDLE: the initial state, waiting for a signal to start
- WELCOME: the state where the robot welcomes the user and gives the first
instructions
- CHALLENGE1: the first challenge state (punch as hard as you can)
- CHALLENGE1_DEBRIEF: the debrief state after the first challeng
- CHALLENGE2: the second challenge state (punch as fast as you can)
- CHALLENGE2_DEBRIEF: the debrief state after the second challenge
- CHALLENGE3: the third challenge state (punch as accurate/steady as you can)
- CHALLENGE3_DEBRIEF: the debrief state after the third challenge
- CONCLUSION: the final state, where the robot gives the final instructions and
the user can leave > goes back the IDLE state
*/

// Modules
#include <ArduinoBLE.h>

#include "audio.h"
#include "behavior.h"

// BLE information (UUID)
// Robot - old but working
// BLEService punchService("95ff7bf8-aa6f-4671-82d9-22a8931c5387");
// BLEFloatCharacteristic punch("95ff7bf8-aa6f-4671-82d9-22a8931c5387", BLERead
// | BLENotify);

// Robot 1
// BLEService punchService("123e4567-e89b-12d3-a456-426614174000");
// BLEFloatCharacteristic punch("123e4567-e89b-12d3-a456-426614174001",
//                             BLERead | BLENotify);

// Robot 2
// BLEService punchService("123e4567-e89b-12d3-a456-426614174002");
//  BLEFloatCharacteristic punch("123e4567-e89b-12d3-a456-426614174003", BLERead
// | BLENotify);

// Robot 3
BLEService punchService("123e4567-e89b-12d3-a456-426614174004");
BLEFloatCharacteristic punch("123e4567-e89b-12d3-a456-426614174005", BLERead
| BLENotify);

// States
enum State {
  BOOTING,
  IDLE,
  WELCOME,
  CHALLENGE1,
  CHALLENGE1_DEBRIEF,
  CHALLENGE2,
  CHALLENGE2_DEBRIEF,
  CHALLENGE3,
  CHALLENGE3_DEBRIEF,
  CONCLUSION
};
State currentState = BOOTING;
State previousState = CONCLUSION;

// Delays and timers
unsigned long previousMillis_general = 0;
int generalInterval = 10;

unsigned long previousMillis_BLE = 0;
int BLEInterval = 500;

unsigned long previousMillis_voice = 0;
int voiceInterval = 3000;
int previousMillis_snap = 0;
int snapInterval = 500;

int audiofile;

float punchData;
float previousPunchData;

unsigned long stateStart;
int stateStage = 0;
bool punched;

// Main variables for the challenges
float maxPunch = 0.0;
int punchCounter = 0;
float punchVariance = 0.0;

// benchmarks
float mean_punchForce = 400;
float mean_punchCounter = 100;
float mean_punchVariance = 200;

float sd_punchForce = 100;
float sd_punchCounter = 40;
float sd_punchVariance = 100;  // mean - 2*SD  mag niet onder nul gaan

// supporting variables
unsigned long previousMillis_C3 = 0;  // 1st timer for interval calculation
unsigned long currentMillis_C3 = 0;   // 2nd timer for interval calculation
int interval;
int previousInterval = 0;  // previous interval for variance calculation
int punchCounter_C3 = 0;   // punch counter for variance calculation
float cumulativeSum_C3 =
    0;  // cumulative (absolute) sum for variance calculation

int mantis = 0;
int hummingbird = 0;
int maki = 0;
int jaguar = 0;
int final_animal;

void setup() {
  // initialize serial commnication
  Serial.begin(9600);

  // initialize the audio and the eye/head movement
  setupDFPlayer();
  setupEyeHeadMovement();

  // initialize BLE
  if (!BLE.begin()) {
    Serial.println("starting BLE failed!");
    while (1);
  }

  // Set advertised local name and services UUID
  BLE.scanForUuid(punchService.uuid());
  Serial.println("Bluetooth® device active, scanning for connections...");
}

void loop() {
  // general timer for the delays
  unsigned long currentMillis = millis();

  // robot head & eye movement (also outside while loop)
  updateBehavior();

  // listen for Bluetooth® Low Energy peripherals to connect:
  if (currentMillis - previousMillis_BLE >= BLEInterval) {
    previousMillis_BLE = currentMillis;

    // Check for available peripherals
    BLE.scanForUuid(punchService.uuid());
    BLEDevice peripheral = BLE.available();

    if (peripheral) {
      Serial.print("Found ");
      Serial.print(peripheral.address());
      Serial.print(" '");
      Serial.print(peripheral.localName());
      Serial.print("' ");
      Serial.print(peripheral.advertisedServiceUuid());
      Serial.println();
      BLE.stopScan();

      if (peripheral.connect()) {
        Serial.println("Connected");

        // search for attributes
        if (peripheral.discoverAttributes()) {
          Serial.println("Attributes discovered");
          BLECharacteristic characteristic =
              peripheral.characteristic(punch.uuid());

          // subscribe to attribute
          if (characteristic) {
            characteristic.subscribe();

            // while the central is still connected to peripheral:
            // Start of the big while loop
            while (peripheral.connected()) {
              delay(10);
              // check for punchdata
              if (characteristic.valueUpdated()) {
                // and get the value if there is an update
                punchData;
                characteristic.readValue(&punchData, 4);
                if (punchData > 0) {
                  Serial.print("PunchDetected||");
                  Serial.println(punchData);
                }
              }

              delay(10);
              // robot head & eye movement (within the while loop)
              updateBehavior();

              //////////////////////////////
              /// the big state machine! ///
              //////////////////////////////
              currentMillis = millis();

              switch (currentState) {
                  //---------//
                  // BOOTING //
                  //---------//

                case BOOTING: {
                  if (previousState != BOOTING) {
                    previousState = BOOTING;
                    punchData = 0;

                    // state commz to protopie each 10 ms in this state
                    Serial.print("state||");
                    Serial.println("BOOTING");
                    delay(200);
                  }

                  // get punch value
                  if (punchData > 1) {
                    Serial.println("Punch detected");
                    Serial.println("Going to IDLE state");
                    currentState = IDLE;
                    punchData = 0;
                  }
                  break;
                }
                  //-------//
                  // IDLE //
                  //------//

                case IDLE: {
                  if (previousState != IDLE) {
                    previousState = IDLE;
                    punchData = previousPunchData;

                    // state commz to protopie each 10 ms in this state
                    Serial.print("state||");
                    Serial.println("IDLE");
                    delay(100);

                    previousMillis_voice = 0;
                  }

                  // random voice (2 & 39)
                  int longestAudiotime = 11000 + 3000;
                  int audiofiles[] = {2, 39};

                  currentMillis = millis();
                  if (currentMillis - previousMillis_voice >=
                      longestAudiotime) {
                    previousMillis_voice = currentMillis;
                    audiofile = audiofiles[random(2)];
                    playSound(audiofile);
                    delay(10);

                    if (audiofile == 2) {
                      Serial.print("subtitles||");
                      Serial.println(
                          "Hi! Ik ben Boxy, jouw boksmaatje! Kijk eens naar "
                          "die "
                          "stoere zakken… Durf jij het aan om er een flinke "
                          "klap "
                          "op te geven?");
                      delay(10);
                    }

                    if (audiofile == 39) {
                      Serial.print("subtitles||");
                      Serial.println(
                          "Hola hola! Wijs dat ge hier zijt! Kom eens "
                          "dichterbij, "
                          "stoere held!");
                      delay(10);
                    }
                  }

                  // get punch value
                  if (punchData != previousPunchData) {
                    Serial.println("Punch detected");
                    Serial.println("Going to WELCOME state");
                    currentState = WELCOME;
                    punchData = 0;
                  }
                  break;
                }

                  //---------//
                  // WELCOME //
                  //---------//

                case WELCOME: {
                  if (previousState != WELCOME) {
                    previousState = WELCOME;

                    Serial.print("state||");
                    Serial.println("WELCOME");

                    stateStart = millis();
                    previousMillis_voice = 0;

                    punchData = 0;
                    stateStage = 0;
                    delay(10);

                    playSound(3);
                    delay(10);

                    Serial.print("subtitles||");
                    Serial.println(
                        "Wauw, dat voelde ik! Jij hebt kracht, mannekes! Ik "
                        "heb "
                        "drie zotte uitdagingen voor jou. Zijn je vuisten "
                        "klaar? ");
                  }

                  currentMillis = millis();
                  if (currentMillis - stateStart > 12000 && stateStage == 0) {
                    stateStage = 1;
                    playSound(37);
                    delay(10);

                    Serial.print("subtitles||");
                    Serial.println(
                        "Oké, hier komt de eerste uitdaging: ik wil zien hoe "
                        "sterk "
                        "je bent! Sla zo hard als ge kunt!");
                  }

                  currentMillis = millis();
                  if (currentMillis - stateStart > 10000 + 12000) {
                    Serial.println("Going to CHALLENGE1 state");
                    currentState = CHALLENGE1;
                  }

                  break;
                }

                  //------------//
                  // CHALLENGE1 //
                  //------------//

                case CHALLENGE1: {
                  // once on state switch
                  if (previousState != CHALLENGE1) {
                    previousState = CHALLENGE1;

                    previousMillis_voice = millis();

                    // state commz to protopie each 10 ms in this state
                    Serial.print("state||");
                    Serial.println("CHALLENGE1");

                    stateStart = millis();

                    delay(100);
                    punchData = 0;
                    previousPunchData = 0;
                    maxPunch = 0.0;
                    Serial.println("reset max force");

                    delay(20);
                  }

                  // print actual force
                  if (punchData != previousPunchData) {
                    previousPunchData = punchData;
                    Serial.print("punchforce||");
                    Serial.println(punchData);
                  }

                  // get maximum force
                  if (punchData > maxPunch) {
                    maxPunch = punchData;
                    Serial.print("max_force||");
                    Serial.println(maxPunch);
                  }

                  // timer to end the challenge (10 seconds)
                  unsigned long challenge1Duration = 10000;
                  // Serial.println((millis() - stateStart) / 600);
                  if (millis() - stateStart >= challenge1Duration) {
                    Serial.println("Going to CHALLENGE1_DEBRIEF state");
                    currentState = CHALLENGE1_DEBRIEF;
                  }

                  // random voice (6, 7, 42, 13)
                  int longestAudiotime = 4000 + 1000;
                  int audiofiles[] = {6, 7, 42, 13};

                  currentMillis = millis();
                  if (currentMillis - previousMillis_voice >=
                      longestAudiotime) {
                    previousMillis_voice = currentMillis;
                    audiofile = audiofiles[random(4)];
                    playSound(audiofile);
                    delay(10);

                    if (audiofile == 6) {
                      Serial.print("subtitles||");
                      Serial.println("Kom op, geef alles wat je hebt!");
                      delay(10);
                    }

                    else if (audiofile == 7) {
                      Serial.print("subtitles||");
                      Serial.println("Wauw, dat was krachtig!");
                      delay(10);
                    }

                    if (audiofile == 42) {
                      Serial.print("subtitles||");
                      Serial.println("Denk je dat je nog harder kunt slaan?");
                      delay(10);
                    }

                    if (audiofile == 13) {
                      Serial.print("subtitles||");
                      Serial.println("Veel beloven, veel beloven …");
                      delay(10);
                    }
                  }

                  break;
                }

                  //--------------------//
                  // CHALLENGE1_DEBRIEF //
                  //--------------------//

                case CHALLENGE1_DEBRIEF: {
                  if (previousState != CHALLENGE1_DEBRIEF) {
                    previousState = CHALLENGE1_DEBRIEF;

                    Serial.print("state||");
                    Serial.println("CHALLENGE1_DEBRIEF");

                    delay(100);

                    stateStart = millis();
                    playSound(50);

                    delay(30);

                    Serial.print("subtitles||");
                    Serial.println("Amai, zo hard!");

                    delay(4000);
                    playSound(15);
                    delay(30);

                    Serial.print("subtitles||");
                    Serial.println(
                        "Nu kijken we hoe snel je bent! Sla zo vaak als je kan "
                        "in "
                        "30 seconden! Klaar, set… bam!");
                  }

                  // timer to end the debrief state (15 seconds)
                  if (millis() - stateStart >= 12000) {
                    Serial.println("Going to CHALLENGE 2 state");
                    currentState = CHALLENGE2;
                  }

                  break;
                }

                  //------------//
                  // CHALLENGE2 //
                  //------------//

                case CHALLENGE2: {
                  if (previousState != CHALLENGE2) {
                    previousState = CHALLENGE2;

                    // state commz to protopie
                    Serial.print("state||");
                    Serial.println("CHALLENGE2");

                    delay(100);

                    stateStart = millis();
                    previousMillis_voice = millis();
                    punched = false;

                    // reset punchCounter for every user
                    punchCounter = 0;
                    previousPunchData = 0;
                  }

                  // print actual force & count
                  if (punchData != previousPunchData) {
                    previousPunchData = punchData;
                    Serial.print("punchforce||");
                    Serial.println(punchData);
                    Serial.print("punch_counter||");
                    Serial.println(punchCounter);
                    punchCounter++;
                  }

                  // random voice (18, 19, 22)
                  int longestAudiotime = 4000 + 1000;
                  int audiofiles[] = {18, 19, 22};

                  if (previousPunchData > 4) {
                    punched = true;
                  }

                  currentMillis = millis();
                  if (currentMillis - previousMillis_voice >=
                          longestAudiotime &&
                      punched) {
                    previousMillis_voice = currentMillis;

                    audiofile = audiofiles[random(3)];
                    playSound(audiofile);
                    delay(10);

                    if (audiofile == 18) {
                      Serial.print("subtitles||");
                      Serial.println("Dat gaat snel, ga door! Goed bezig!");
                      delay(10);
                    }

                    else if (audiofile == 19) {
                      Serial.print("subtitles||");
                      Serial.println("Ola, wa een tempo! Kunt ge nog sneller?");
                      delay(10);
                    }

                    else if (audiofile == 22) {
                      Serial.print("subtitles||");
                      Serial.println("Amai, gij hebt nen langen adem!");
                      delay(10);
                    }
                  }

                  // timer to end the debrief state (30 seconds)
                  if (millis() - stateStart >= 30000) {
                    Serial.println("Going to CHALLENGE2_DEBRIEF 2 state");
                    currentState = CHALLENGE2_DEBRIEF;
                  }

                  break;
                }

                  //------------------//
                  // CHALLENGE2_DEBRIEF //
                  //------------------//

                case CHALLENGE2_DEBRIEF: {
                  if (previousState != CHALLENGE2_DEBRIEF) {
                    previousState = CHALLENGE2_DEBRIEF;

                    // state commz to protopie
                    Serial.print("state||");
                    Serial.println("CHALLENGE2_DEBRIEF");
                    delay(100);

                    stateStart = millis();

                    // audio  on state switch
                    playSound(51);
                    delay(30);

                    Serial.print("subtitles||");
                    Serial.println(
                        "Wat een snelheid!");
                    delay(4000);

                    // voice 26 (once on state switch)
                    playSound(26);
                    delay(10);

                    Serial.print("subtitles||");
                    Serial.println(
                        "Nen goeie bokser slaat op het ritme! Dus als laatste "
                        "opdracht, probeer zo ritmisch mogelijk te slaan.");
                  }

                  // timer to end the debrief state (15 seconds)
                  if (millis() - stateStart >= 15000) {
                    Serial.println("Going to CHALLENGE3 2 state");
                    currentState = CHALLENGE3;
                  }

                  break;
                }

                  //------------//
                  // CHALLENGE3 //
                  //------------//

                case CHALLENGE3: {
                  if (previousState != CHALLENGE3) {
                    previousState = CHALLENGE3;

                    Serial.print("state||");
                    Serial.println("CHALLENGE3");

                    delay(100);

                    stateStart = millis();
                    previousMillis_voice = millis();

                    previousPunchData = punchData;
                    previousMillis_C3 = millis();

                    stateStage = 0;

                    // reset punchVariance for every user
                    punchVariance = 0.0;
                    cumulativeSum_C3 = 0;
                    punchCounter_C3 = 0;
                    previousInterval == 0;
                  }

                 

                  
                  if (punchData != previousPunchData) {
                     // hitcounter
                  punchCounter_C3++;
                  Serial.print("punch_counter||");
                  Serial.println(punchCounter_C3);
                    previousPunchData = punchData;
                    currentMillis_C3 = millis();
                    Serial.print("punchforce||");
                    Serial.println(punchData);
                    delay(1);
                    // first hit
                    if (punchCounter_C3 == 1) {
                      Serial.print("First Hit! ");
                      Serial.println(punchData);
                      delay(1);
                      previousMillis_C3 = millis();

                    } else if (punchCounter_C3 > 1) {
                      // get punch variance
                      // calculate the time (in ms) between two punches
                      interval = currentMillis_C3 - previousMillis_C3;
                      previousMillis_C3 = currentMillis_C3;
                      Serial.print("interval_ms||");
                      Serial.println(interval);
                      delay(1);
                    }

                    if (punchCounter_C3 == 2) {
                      // calculate the deviation from the moving baseline
                      previousInterval = interval;
                      Serial.println("baseline_interval||");
                      Serial.println(interval);
                      delay(1);
                    }

                    if (punchCounter_C3 >= 3){
                      int delta = (interval - previousInterval);
                      previousInterval = interval;
                      Serial.print("punch_deviance||");
                      Serial.println(delta);
                      delay(1);

                      // sum the total deviation
                      cumulativeSum_C3 += abs(delta);

                      // Calculate the moving average (mean deviation)
                      punchVariance = cumulativeSum_C3 / (punchCounter_C3 - 2);
                      Serial.print("punch_variance||");
                      Serial.println(punchVariance);
                      delay(1);
                    }
                  }
                  // random voice (47, 33)
                  int longestAudiotime = 17000 + 1000;
                  int audiofiles[] = {47, 33};

                  currentMillis = millis();
                  if (currentMillis - previousMillis_voice >=
                      longestAudiotime) {
                    previousMillis_voice = currentMillis;

                    audiofile = audiofiles[random(2)];
                    playSound(audiofile);
                    delay(10);

                    if (audiofile == 47) {
                      Serial.print("subtitles||");
                      Serial.println(
                          "Ik zie da ge er plezier in hebt, slaat eens op het "
                          "ritme van Happy! Zo gaade in de goeie flow geraken! "
                          " "
                          "“Because I'm happy - Clap along- if you feel like a "
                          "room without a roof - Because I'm happy");
                      delay(10);
                    }

                    else if (audiofile == 33) {
                      Serial.print("subtitles||");
                      Serial.println(
                          "Allez, voel het ritme in uw vuisten! Gelijk ne "
                          "goeie "
                          "danser slaat gij perfect op de maat!");
                      delay(10);
                    }
                  }

                  // timer to end the challenge state (30 seconds)
                  if (millis() - stateStart >= 30000) {
                    Serial.println("Going to CHALLENGE3 3 DEBRIEF state");
                    currentState = CHALLENGE3_DEBRIEF;
                  }

                  break;
                }

                  //--------------------//
                  // CHALLENGE3_DEBRIEF //
                  //--------------------//

                case CHALLENGE3_DEBRIEF: {
                  if (previousState != CHALLENGE3_DEBRIEF) {
                    previousState = CHALLENGE3_DEBRIEF;

                    // state commz to protopie
                    Serial.print("state||");
                    Serial.println("CHALLENGE3_DEBRIEF");
                    delay(200);
                    stateStart = millis();

                    playSound(29);
                    delay(30);

                    Serial.print("subtitles||");
                    Serial.println(
                        "Amai, gij zou beter drummer worden, wat een ritme, "
                          "wat "
                          "een precisie!");
                  }
                  delay(30);

                  // timer to end state
                  if (millis() - stateStart >= 5000) {
                    Serial.println("Going to CONCLUSION state");
                    currentState = CONCLUSION;
                  }

                  break;
                }

                  // ------------ //
                  // CONCLUSION //
                  // ------------ //

                case CONCLUSION: {
                  if (previousState != CONCLUSION) {
                    previousState = CONCLUSION;

                    Serial.print("state||");
                    Serial.println("CONCLUSION");

                    stateStart = millis();

                    // caclucate the final score
                    // use benchmarks for the relative values (measure!!!!)
                    // map (variable, bad benchmark, good benchmark, from 0, to
                    // 100)

                    int strengthMapped =
                        map(maxPunch, mean_punchForce - (2 * sd_punchForce),
                            mean_punchForce + (2 * sd_punchForce), 0, 100);
                    int speedMapped = map(
                        punchCounter, mean_punchCounter - (2 * sd_punchCounter),
                        mean_punchCounter + (2 * sd_punchCounter), 0, 100);
                    int punchVarianceMapped =
                        map(punchVariance,
                            mean_punchVariance + (2 * sd_punchVariance),
                            mean_punchVariance - (2 * sd_punchVariance), 0,
                            100);  // reverse mapping

                    // calculate the animal rank
                    mantis = 0;
                    if (strengthMapped > 75) {
                      mantis = 3;
                    } else if (strengthMapped > 50) {
                      mantis = 2;
                    } else if (strengthMapped > 25) {
                      mantis = 1;
                    }

                    hummingbird = 0;
                    if (speedMapped > 75) {
                      hummingbird = 3;
                    } else if (speedMapped > 50) {
                      hummingbird = 2;
                    } else if (speedMapped > 25) {
                      hummingbird = 1;
                    }

                    maki = 0;
                    if (punchVarianceMapped > 75) {
                      maki = 3;
                    } else if (punchVarianceMapped > 50) {
                      maki = 2;
                    } else if (punchVarianceMapped > 25) {
                      maki = 1;
                    }

                    // welk dier ben je?
                    // vanaf je een 3 ster jaguar bent wint dat dier
                    // anders wint het dier met de hoogste score
                    if (strengthMapped > speedMapped &&
                        strengthMapped > punchVarianceMapped) {
                      final_animal = 1;  // mantis
                    } else if (speedMapped > strengthMapped &&
                               speedMapped > punchVarianceMapped) {
                      final_animal = 2;  // kolibri
                    } else if (punchVarianceMapped > speedMapped &&
                               punchVarianceMapped > strengthMapped) {
                      final_animal = 3;  // snow monkey
                    }

                    delay(1000);

                    // voice 34 (once on state switch)
                    playSound(34);
                    delay(10);

                    Serial.print("subtitles||");
                    Serial.println(
                        "Ojo jij, gij kunt er wat van! Ik heb ontdekt welk "
                        "beestje "
                        "gij zijt. Benieuwd? Check je dierprofiel op het grote "
                        "bord!");
                    delay(10);

                    delay(500);

                    // communication to protopie 
                    Serial.print("score_strength||");
                    Serial.println(maxPunch);
                    delay(20);
                    Serial.print("score_speed||");
                    Serial.println(punchCounter);
                    delay(20);
                    Serial.print("score_ritme||");
                    Serial.println(punchVariance);
                    delay(20);

                    // Serial.print("score_strength||");
                    // Serial.println(strengthMapped);
                    // delay(20);
                    // Serial.print("score_speed||");
                    // Serial.println(speedMapped);
                    // delay(20);
                    // Serial.print("score_ritme||");
                    // Serial.println(punchVarianceMapped);
                    // delay(20);

                    Serial.print("mantis||");
                    Serial.println(mantis);
                    delay(20);
                    Serial.print("hummingbird||");
                    Serial.println(hummingbird);
                    delay(20);
                    Serial.print("maki||");
                    Serial.println(maki);
                    delay(20);
                    Serial.print("jaguar||");
                    Serial.println(jaguar);
                    delay(200);
                    Serial.print("final_animal||");
                    Serial.println(final_animal);
                    delay(20);
                  }

                  // timer to end the conclusion state and go back to IDLE (10
                  // sec)
                  if (millis() - stateStart >= 20000) {
                    Serial.println("Going to IDLE state");
                    currentState = IDLE;
                    punchData = 0;
                  }

                  break;
                }

              }  // ... end of the state machine

            }  // .. end of while loop
          }
        }
        // disconnect
        Serial.print("Disconnected from peripheral: ");
        Serial.println(peripheral.address());
      }
      BLE.scanForUuid(punchService.uuid());
    }
    BLE.scanForUuid(punchService.uuid());
    BLE.poll();
    Serial.println(

        F("Disconnected from central"));  // when the central
                                          // disconnects, print it out:

  }  // .. end of BLE loop
}