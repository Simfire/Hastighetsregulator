// This program monitors the wheelspeed by counting the pulse frequency from the wheel ABS sensor.
// If it exeeds 230 Hz (32 km/h) it cuts of the power suply to the ignition coil.
//
// On a Volvo 850 -97 with tyre dimension 205/55 R16 and 48 tooths on the ABS ring you get the following values:
// velocity/circumferense = rounds/sec, rounds/sec * (number of tooths on ABS ring) = pulses/sec (Hz)
// km/h / 3,6 = m/s, tyre dimension 205/55 R16 equals 1,9 m in circumferense
// 27 km/h = a pulse frequency of 189 Hz
// 30 km/h = a pulse frequency of 210 Hz
// 33 km/h = a pulse frequency of 231 Hz
//
// Author: Anders Hansson
// Date: Februari 2020
// Tested on Arduino Uno rev 3
//************************************************************************************************************
#include <LiquidCrystal.h>

// Declarations of global constants and variables *****************************

const int accessCodePin = 1;               // Pin for control of access code.
const int d4 = 5, d5 = 4, d6 = 3, d7 = 2 ; // lcd ports: d4 - d7 = data ports
const int rs = 7, en = 6;                  // rs = register select, e = enable
const int wheelSensorTestPin1 = 8;         // For testing the condition of the wheelsensor.
const int wheelSensorTestPin2 = 9;         //              - " -
const int wheelSensorPulsePin = 10;        // Pin for reading the puls from the wheelsensor.
const int ignitionPulsePin = 11;           // Pin for reading the pulse to the ignition coil.
const int ignitionPin = 12;                // Controls the status of the ignitioncoil.
float velocity;                            // Speed of wheel.
float frequency;                           // Pulse frequency.


// Initialize the library by associating any needed LCD interface pin
// with the arduino pin number it is connected to
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);


// Declarations of functions ***************************************************

// This function cuts off the power to the ignition coil and
// displays "Startspärr aktiverad!" on lcd.
void immobilizer(void) {
  while(1){                                 // Endless loop, restart to exit!
    digitalWrite(ignitionPin, LOW);
    lcd.clear();
    lcd.setCursor(3, 0);
    lcd.print("Startsp"); lcd.print(char(225)); lcd.print("rr");
    lcd.setCursor(3, 1);
    lcd.print("aktiverad!");
    delay(2000);
  }
}


// This function controles the status of access code.
// Returns "true" if access code correct otherwise "false".
bool codeOk(void) {
  int i;
  for (i = 2; i >= 0; i--) {
    if (digitalRead(accessCodePin) == HIGH) {
      lcd.setCursor(0, 0);
      lcd.print("Koden okej!");
      delay(1000);
      return true;
    } else {
      lcd.setCursor(0, 0);
      lcd.print ("Fel kod!");
      lcd.setCursor(0, 1);
      lcd.print(i);
      lcd.print(" f");lcd.print(char(239));lcd.print("rs");lcd.print(char(239));lcd.print("k kvar!");
      delay(3000);
    }
  }
  immobilizer();
}


// This function controles the status of wheel sensor.
// Returns "true" if sensor is okey otherwise "false".
bool wheelSensorOk(void) {
  digitalWrite(wheelSensorTestPin1, HIGH);        // Initiates test
  delay(100);
  if (digitalRead(wheelSensorTestPin2) == HIGH) {
    digitalWrite(wheelSensorTestPin1, LOW);       // Abort test
    return true;
  } else {
    digitalWrite(wheelSensorTestPin1, LOW);       // Abort test
    return false;
  }
}


// This function calculates and returns the frequency of the pulse [Hz].
float pulseFrequency(int pulsePin) {
  long pulseHigh;                                 // High duration time of pulse.
  long pulseLow;                                  // Low duration time of pulse.
  float pulsePeriod;                              // Period time of the pulse.

  pulseHigh = pulseIn(pulsePin, HIGH);            // High time in micro seconds.
  pulseLow = pulseIn(pulsePin, LOW);              // Low time in micro seconds.
  pulsePeriod = pulseHigh + pulseLow;             // Period time of the pulse in micro seconds.
  if (pulsePeriod == 0){
    frequency = 0;
  } else {
    frequency = (1000000/pulsePeriod);         // Frequency in Hertz. (Hz)
  }
  return frequency;
}


// This function restricts the engine to 1200 rpm
void safeMode(void) {
  float rpm;
  while (1) {                                        // Endless loop, restart to exit!
    rpm = pulseFrequency(ignitionPulsePin) * 12;     // 5 cylinder => 5 sparks/round. 1 Hz in ignition pulse = 12 rpm.
    if (rpm > 1200) {
      digitalWrite(ignitionPin, LOW);
    } else {
      digitalWrite(ignitionPin, HIGH);
    }
    lcd.clear();
    lcd.setCursor(2, 0);
    lcd.print("S"); lcd.print(char(225)); lcd.print("kerhets");
    lcd.print("l"); lcd.print(char(225)); lcd.print("ge");
    lcd.setCursor(3, 1);
    lcd.print("aktiverat!");
    delay(1000);
  }
}


void setup() {
  pinMode(ignitionPin, OUTPUT);
  pinMode(wheelSensorTestPin1, OUTPUT);
  pinMode(wheelSensorTestPin2, INPUT);
  pinMode(wheelSensorPulsePin, INPUT);
  pinMode(ignitionPulsePin, INPUT);
  pinMode(accessCodePin, INPUT);
  lcd.begin(16, 2);                                  // Initiates lcd.
  lcd.clear();                                       // Clear the lcd.
  digitalWrite(ignitionPin, LOW);                    // Initial status of ignition coil is power OFF.

  if (codeOk() == true) {                            // Check access code.
    // Continuity test for the wheel sensor. If it's OK the ignition coil is enabled
    // otherwise the coil is dissabled.
    lcd.setCursor(0, 0);
    lcd.print("Test hjulsensor");
    delay(2000);
    if (wheelSensorOk() == true) {
      digitalWrite(ignitionPin, HIGH);                 // Power ON to ignition coil.
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Hjulsensor OK!");
      delay(3000);
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("K"); lcd.print(char(239)); lcd.print("r");
      lcd.print(" f"); lcd.print(char(239)); lcd.print("rsiktigt!");
      delay(4000);
      lcd.clear();
    } else {                                    
      lcd.clear();
      lcd.setCursor(1, 0);
      lcd.print("Hjulsensor ur");
      lcd.setCursor(3, 1);
      lcd.print("funktion!");
      delay(2000);
      immobilizer();                                    // Endless loop, restart to exit!
    }
  }
}


// Main program starts here. ********************************************
void loop() {
  float wheelSensorPulsFrequency;
    wheelSensorPulsFrequency = pulseFrequency(wheelSensorPulsePin);
    velocity = wheelSensorPulsFrequency * 0.1425;             // Speed in km/h.
      if (velocity < 29) {
        lcd.setCursor(14, 1);
        lcd.print(":)");
        digitalWrite(ignitionPin, HIGH);                       // Power to ignition coil ON.
        if (velocity == 0){
          if (wheelSensorOk() == false){
            lcd.clear();
            lcd.setCursor(1,0);
            lcd.print("Hjulsensor ur");
            lcd.setCursor(3,1);
            lcd.print("funktion!");
            delay(1000);
            safeMode();
          }
        }
      }
      if (velocity > 32.8) {
        digitalWrite(ignitionPin, LOW);                        // Power to ignition coil OFF.
        lcd.clear();
        lcd.setCursor(3, 0);
        lcd.print("SAKTA NER!");
        delay(1000);
        lcd.clear();
      }   
    lcd.setCursor(0, 0);
    lcd.print(velocity, 1);
    lcd.print(" km/h");
    lcd.setCursor(0, 1);
    lcd.print(wheelSensorPulsFrequency,2);
    lcd.print(" Hz");   
    delay(600);
    lcd.clear();
}
