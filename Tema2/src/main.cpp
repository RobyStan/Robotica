#include <Arduino.h>

const int buttonStartStopPin = 3;
const int buttonDifficultyPin = 2;

const int ledRgbBluePin = 4;
const int ledRgbGreenPin = 5;
const int ledRgbRedPin = 6;

enum GameState { IDLE, RUNNING };
GameState currentState = IDLE;

volatile int difficultyLevel = 0;
const char* difficulties[] = {"Easy", "Medium", "Hard"};
volatile bool startStopPressed = false;
volatile bool difficultyPressed = false;

unsigned long startTime;
unsigned long countdownTime = 3000;
unsigned long roundDuration = 30000;
unsigned long wordDisplayTime = 3000;

int correctWordsCount = 0;
const char* wordList[] = {"tema", "led", "lab", "mere", "timp", "luna", "casa", "soare", "curs", "program", "tehnologie"};
const int wordCount = sizeof(wordList) / sizeof(wordList[0]);

String currentWord = "";
String userInput = "";
bool newWordReady = true;
unsigned long wordStartTime;
unsigned long lastButtonPress = 0;
const unsigned long debounceDelay = 500;

// Function to set the color of the RGB LED based on the provided RGB values
void setLedColor(int red, int green, int blue) {
    analogWrite(ledRgbRedPin, red);
    analogWrite(ledRgbGreenPin, green);
    analogWrite(ledRgbBluePin, blue);
}

// Function to display a countdown from 3 to 1 before starting the round
void countdownDisplay() {
    for (int i = 3; i > 0; i--) {
        Serial.println(i);
        setLedColor(0, 0, 0);
        delay(500);
        setLedColor(255, 255, 255);
        delay(500);
    }
    setLedColor(0, 255, 0);
}

// Function to randomly select and display the next word for the user to type
void displayNextWord() {
    int randomIndex = random(wordCount);
    currentWord = wordList[randomIndex];
    Serial.print("Word: ");
    Serial.println(currentWord);
    userInput = "";
    newWordReady = false;
    wordStartTime = millis();
}

// Function to check the user's input against the current word and update the score
void checkUserInput() {
    if (Serial.available()) {
        char inputChar = Serial.read();
        if (inputChar == 8) {
            if (userInput.length() > 0) {
                userInput.remove(userInput.length() - 1);
                Serial.print("\b \b");
            }
            return;
        }
        userInput += inputChar;

        if (userInput.length() == currentWord.length()) {
            if (userInput.equals(currentWord)) {
                correctWordsCount++;
                newWordReady = true;
                setLedColor(0, 255, 0);
            } else {
                setLedColor(255, 0, 0);
            }
        }
    }
}

// Function to initialize a new round of the game, resetting relevant variables
void startNewRound() {
    currentState = RUNNING;
    correctWordsCount = 0;
    userInput = "";
    newWordReady = true;
    Serial.println("Starting new round!");
    startTime = millis();
}

// Function to stop the current round and return to the IDLE state
void stopRound() {
    currentState = IDLE;
    Serial.println("Round stopped!");
    setLedColor(255, 255, 255);
}

// Function to conclude the round, displaying the user's score and resetting the state
void endRound() {
    currentState = IDLE;
    Serial.print("Round ended! You scored: ");
    Serial.println(correctWordsCount);
    setLedColor(255, 255, 255);
}

// Function to handle the start/stop button press with debouncing logic
void handleStartStop() {
    if (millis() - lastButtonPress > debounceDelay) {
        startStopPressed = true;
        lastButtonPress = millis();
    }
}

// Function to handle the difficulty button press and cycle through difficulty levels
void handleDifficulty() {
    if (currentState == IDLE) {
        if (millis() - lastButtonPress > debounceDelay) {
            difficultyLevel = (difficultyLevel + 1) % 3;
            Serial.print(difficulties[difficultyLevel]);
            Serial.println(" mode on!");
            switch (difficultyLevel) {
                case 0: wordDisplayTime = 3000; break;
                case 1: wordDisplayTime = 1500; break;
                case 2: wordDisplayTime = 1000; break;
            }
            lastButtonPress = millis();
        }
    }
}

// Arduino setup function to initialize serial communication and pin modes
void setup() {
    Serial.begin(9600);
    pinMode(buttonStartStopPin, INPUT_PULLUP);
    pinMode(buttonDifficultyPin, INPUT_PULLUP);
    pinMode(ledRgbGreenPin, OUTPUT);
    pinMode(ledRgbBluePin, OUTPUT);
    pinMode(ledRgbRedPin, OUTPUT);
    attachInterrupt(digitalPinToInterrupt(buttonStartStopPin), handleStartStop, FALLING);
    attachInterrupt(digitalPinToInterrupt(buttonDifficultyPin), handleDifficulty, FALLING);
    setLedColor(255, 255, 255);
}

// Main loop function that manages the game state and user interactions
void loop() {
    if (startStopPressed) {
        startStopPressed = false;
        if (currentState == IDLE) {
            startNewRound();
        } else {
            stopRound();
        }
    }

    if (difficultyPressed) {
        difficultyPressed = false;
        handleDifficulty();
    }

    if (currentState == RUNNING) {
        if (millis() < startTime + countdownTime) {
            countdownDisplay();
        } else if (millis() < startTime + countdownTime + roundDuration) {
            if (newWordReady) {
                displayNextWord();
            } else if (millis() - wordStartTime >= wordDisplayTime) {
                displayNextWord();
            }
            checkUserInput();
        } else {
            endRound();
        }
    }
}
