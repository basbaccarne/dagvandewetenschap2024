// test script to test the state machine

// Definition of the states
enum State
{
    BOOTING,
    IDLE,
    WELCOME,
    CHALLENGE1,
    CHALLENGE1_DEBRIEF,
    CHALLENGE2,
    CHALLENGE2_DEBRIEF,
    CHALLENGE3,
    CHALLENGE3_DEBRIEF,
    CHALLENGE4,
    CHALLENGE4_DEBRIEF,
};

State currentState = BOOTING; // Initial state

void setup()
{
    Serial.begin(9600);
}

void loop()
{
    switch (currentState)
    {
        case BOOTING:
        Serial.println("Booting ...");
        currentState = IDLE;
        break;

        case IDLE:
        Serial.println("Idle ...");
        currentState = WELCOME;
        break;

        case WELCOME:
        Serial.println("Welcome ...");
        currentState = CHALLENGE1;
        break;

        case CHALLENGE1:
        Serial.println("Challenge 1 ...");
        currentState = CHALLENGE1_DEBRIEF;
        break;

        case CHALLENGE1_DEBRIEF:
        Serial.println("Challenge 1 Debrief ...");
        currentState = CHALLENGE2;
        break;

        case CHALLENGE2:
        Serial.println("Challenge 2 ...");
        currentState = CHALLENGE2_DEBRIEF;
        break;

        case CHALLENGE2_DEBRIEF:
        Serial.println("Challenge 2 Debrief ...");
        currentState = CHALLENGE3;
        break;

        case CHALLENGE3:
        Serial.println("Challenge 3 ...");
        currentState = CHALLENGE3_DEBRIEF;
        break;

        case CHALLENGE3_DEBRIEF:
        Serial.println("Challenge 3 Debrief ...");
        currentState = CHALLENGE4;
        break;

        case CHALLENGE4:
        Serial.println("Challenge 4 ...");
        currentState = CHALLENGE4_DEBRIEF;
        break;

        case CHALLENGE4_DEBRIEF:
        Serial.println("Challenge 4 Debrief ...");
        currentState = IDLE;
        break;
    }

    // Transition to the next state
    delay(1000);
   
}