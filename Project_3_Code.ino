/*
  Project 3: Digital Display
 
  Description: This code powers the electronics within a Science Museum Art Piece
  that shows information about nanosatellites that have been launched into our atmosphere.

  The circuit:
    - LCD Screen that displays nanosatellite information
    - Three buttons used to select between nanosatellites
    - Servo used to physically rotate images of the selected nanosatellites


  Created by: Rouhan and Gregory
  Modified 27 February 2025
*/

// Included Libraries
#include <Servo.h>
#include <LiquidCrystal.h>

/* Objects */
// Servo Objects
Servo myServo1;
Servo myServo2;
Servo myServo3;

// LCD Object
LiquidCrystal lcd(13, 12, 11, 10, 9, 8);

/* Constant Variables */
const int leftButton = 2;
const int rightButton = 3;
const int selectButton = 4;
// const int ledPin = 13; // Used to test whether buttons were working

/* Variables */
// Button States
int leftButtonState = 0;
int rightButtonState = 0;
int selectButtonState = 0;
int lastSelectButtonState = HIGH;

// Servo Angle
int angle = 40; // set to the first satellite position on servo

/* Global Variables for Screen Switching */
unsigned long lastSwitchTime = 0; // Track last switch time
bool showingMission = true; // Toggle between mission and launch status

// Satellite Data Struct
struct Satellite {
    String name;
    String mission;
    String launched;
};

// Array of Satellites
const int numSatellites = 4;
Satellite satellites[numSatellites] = {
    {"HAUSat-1", "CubeSat tech", "Failure"},
    {"Flock-4be 1", "Earth imaging", "Operational"},
    {"VELOX-II", "Data transfer", "Reentered"},
    {"TUBSAT-N", "Store/Send Comms", "Reentered"},
};

int selectedIndex = 0; // Index of selected satellite
bool selected = false; // Whether a satellite is selected

void setup() {
  // initialize the LED pin as an output for button testing:
  // pinMode(ledPin, OUTPUT);

  /* Button Initialization */
  pinMode(leftButton, INPUT_PULLUP);
  pinMode(rightButton, INPUT_PULLUP);
  pinMode(selectButton, INPUT_PULLUP);

  /* Servo Initialization */
  myServo1.attach(5); // Attach to pin
  myServo1.write(angle); //Initialize starting angle

  /* LCD Initialization */
  lcd.begin(16, 2);
  updateLCD();

  // Serial monitor for testing purposes
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  leftButtonState = digitalRead(leftButton);
  rightButtonState = digitalRead(rightButton);
  selectButtonState = digitalRead(selectButton);
 
  if (leftButtonState == LOW) {
    // Call MoveSelection:
    moveSelection(-1);
  }
  if (rightButtonState == LOW) {
    // Call moveSelection:
    moveSelection(1);
  }
  if (selectButtonState == LOW && lastSelectButtonState == HIGH) {
    // Call selectSatellite
    selectSatellite();
  }
  lastSelectButtonState = selectButtonState;
  updateLCD(); // Keep calling updateLCD() so the screen toggles
  delay(200);
  // Serial.println(angle);
  // Serial.println("right button: " + String(rightButtonState) + " left button: " + String(leftButtonState) + " angle: " + String(angle));
}

void moveSelection(int direction) {
    if (!selected) { // Only allow movement if no satellite is selected
        selectedIndex += direction;

        // Ensures index wraps around instead of going out of bounds
        if (selectedIndex < 0) selectedIndex = 0;
        if (selectedIndex >= numSatellites) selectedIndex = numSatellites - 1;

        angle = map(selectedIndex, 0, numSatellites - 1, 40, 140);
        myServo1.write(angle);
        updateLCD();
    }
}

void selectSatellite() {
    selected = !selected; // Toggle selection mode
    updateLCD();
}

void updateLCD() {
  if (!selected) {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Satellite: ");
    lcd.setCursor(0, 1);
    lcd.print(satellites[selectedIndex].name);
  }
  else {
    if (millis() - lastSwitchTime >= 3000) { // Switch every 3 seconds
      lcd.clear();
      if (showingMission) {
        lcd.setCursor(0, 0);
        lcd.print("Mission: ");
        lcd.setCursor(0, 1);
        lcd.print(satellites[selectedIndex].mission);
      }   
      else {
        lcd.setCursor(0, 0);
        lcd.print("Launch Status: ");
        lcd.setCursor(0, 1);
        lcd.print(satellites[selectedIndex].launched);
      }
      showingMission = !showingMission;  // Toggle screen
      lastSwitchTime = millis();  // Reset timer
    }
  }
}
