#include <Arduino.h>

const int buttonStartPin = 3;  
const int buttonStopPin = 2;   

const int ledRGBGreenPin = 5;  
const int ledRGBRedPin = 6;     
const int ledL4Pin = 7;         
const int ledL3Pin = 8;         
const int ledL2Pin = 9;         
const int ledL1Pin = 10;        

enum State {FREE, LOADING};
State stationState = FREE;

unsigned long previousMillis = 1000; 
const long interval = 3000; 
int loadingStep = 0; 

int buttonStartState = HIGH;         
int buttonStopState = HIGH;         

void allLedsOn();  
void allLedsOff(); 
void startCharging(); 
void updateLoading(); 
void finishCharging(); 
void resetCharging(); 

void setup() {
  pinMode(buttonStartPin, INPUT_PULLUP);
  pinMode(buttonStopPin, INPUT_PULLUP); 
  pinMode(ledRGBGreenPin, OUTPUT);
  pinMode(ledRGBRedPin, OUTPUT);
  pinMode(ledL4Pin, OUTPUT);
  pinMode(ledL3Pin, OUTPUT);
  pinMode(ledL2Pin, OUTPUT);
  pinMode(ledL1Pin, OUTPUT);

  digitalWrite(ledRGBGreenPin, HIGH); 
  digitalWrite(ledRGBRedPin, LOW);     
  allLedsOff();                        
}

void loop() {
  int readingStart = digitalRead(buttonStartPin);
  if (readingStart == LOW && stationState == FREE) {
    startCharging();
  }

  if (stationState != FREE) {
    int readingStop = digitalRead(buttonStopPin);
    if (readingStop == LOW) {
      resetCharging(); 
    }
  }

  if (stationState == LOADING) {
    unsigned long currentMillis = millis();
    if (currentMillis - previousMillis >= interval) {
      previousMillis = currentMillis;
      updateLoading();
    }
  }
}

void startCharging() {
  stationState = LOADING;
  loadingStep = 0;
  digitalWrite(ledRGBGreenPin, LOW); 
  digitalWrite(ledRGBRedPin, HIGH);  
  updateLoading();
}

void updateLoading() {
  if (loadingStep < 4) {
    for (int i = 0; i < 3; i++) {
      int readingStop = digitalRead(buttonStopPin);
      if (readingStop == LOW) {
        resetCharging(); 
        return;  
      }
      digitalWrite(ledL1Pin - loadingStep, HIGH);
      delay(500);
      digitalWrite(ledL1Pin - loadingStep, LOW);  
      delay(500);
    }
    digitalWrite(ledL1Pin - loadingStep, HIGH);
    loadingStep++;
  }
  if (loadingStep >= 4) {
    finishCharging();
  }
}

void finishCharging() {
  for (int i = 0; i < 3; i++) {
    allLedsOn();
    delay(500);
    allLedsOff();
    delay(500);
  }
  stationState = FREE;
  digitalWrite(ledRGBGreenPin, HIGH); 
  digitalWrite(ledRGBRedPin, LOW);     
}

void resetCharging() {
  finishCharging();
  allLedsOff(); 
  stationState = FREE; 
  loadingStep = 0; 
  digitalWrite(ledRGBGreenPin, HIGH); 
  digitalWrite(ledRGBRedPin, LOW);     
}

void allLedsOn() {
  digitalWrite(ledL1Pin, HIGH);
  digitalWrite(ledL2Pin, HIGH);
  digitalWrite(ledL3Pin, HIGH);
  digitalWrite(ledL4Pin, HIGH);
}

void allLedsOff() {
  digitalWrite(ledL1Pin, LOW);
  digitalWrite(ledL2Pin, LOW);
  digitalWrite(ledL3Pin, LOW);
  digitalWrite(ledL4Pin, LOW);
}
