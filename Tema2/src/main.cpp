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

void setLedColor(int red, int green, int blue) {
    analogWrite(ledRgbRedPin, red);
    analogWrite(ledRgbGreenPin, green);
    analogWrite(ledRgbBluePin, blue);
}

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

void displayNextWord() {
    int randomIndex = random(wordCount);
    currentWord = wordList[randomIndex];
    Serial.print("Word: ");
    Serial.println(currentWord);
    userInput = "";
    newWordReady = false;
    wordStartTime = millis();
}

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

void startNewRound() {
    currentState = RUNNING;
    correctWordsCount = 0;
    userInput = "";
    newWordReady = true;
    Serial.println("Starting new round!");
    startTime = millis();
}

void stopRound() {
    currentState = IDLE;
    Serial.println("Round stopped!");
    setLedColor(255, 255, 255);
}

void endRound() {
    currentState = IDLE;
    Serial.print("Round ended! You scored: ");
    Serial.println(correctWordsCount);
    setLedColor(255, 255, 255);
}

void handleStartStop() {
    if (millis() - lastButtonPress > debounceDelay) {
        startStopPressed = true;
        lastButtonPress = millis();
    }
}

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
