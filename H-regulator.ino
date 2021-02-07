// This program monitors the wheelspeed by counting the pulse frequency from the wheel ABS sensor.
// If it exeeds 225 Hz (32 km/h) it cuts of the power suply to the ignition coil.
//
// On a Volvo 850 -97 with tyre dimension 205/55 R16 and 48 tooths on the ABS ring you get the following values:
// velocity/circumferense = rounds/sec, rounds/sec * (number of tooths on ABS ring) = pulses/sec (Hz)
// km/h / 3,6 = m/s, tyre dimension 205/55 R16 equals 1,9 m in circumferense
// 27 km/h = a pulse frequency of 189 Hz
// 30 km/h = a pulse frequency of 210 Hz
// 33 km/h = a pulse frequency of 231 Hz
//************************************************************************************************************
#include <LiquidCrystal.h>

LiquidCrystal lcd(2, 3, 4, 5, 6, 7);// LCD pin connections

bool wheelSensorOK;        // Status of wheel sensor.
const int wheelSensorTestPin1 = 8;  // For testing the condition of the wheelsensor.
const int wheelSensorTestPin2 = 9;  //              - " -
const int pulsePin = 12;            // Pin for reading the puls from the wheelsensor.
const int ignitionPin = 10;         // Controls the status of the ignitioncoil.
int pulseHigh;                      // High duration time of pulse.
int pulseLow;                       // Low duration time of pulse.
float pulseTotal;                   // Period time of the pulse.
float frequency;                    // Frequency of the pulse.
float velocity;                     // Speed of wheel.

void setup() {
 
  pinMode(wheelSensorTestPin1,OUTPUT);
  pinMode(wheelSensorTestPin2,INPUT);
  pinMode(pulsePin,INPUT);
  pinMode(ignitionPin,OUTPUT);
  wheelSensorOK == false;
  lcd.begin(16, 2);
  lcd.clear();
 
  // Continuity test for the wheel sensor. If it's OK the ignition coil is enabled
  // otherwise the coil is dissabled.
  digitalWrite(ignitionPin,LOW);            // Power to ignition coil OFF.
  digitalWrite(wheelSensorTestPin1,HIGH);   // Initiates test
  delay(500);
  digitalRead(wheelSensorTestPin2);
  if (wheelSensorTestPin2 == HIGH){
    wheelSensorOK == true;
    lcd.setCursor(0,0);
    lcd.print("Hjulsensor OK!");
    delay(3000);
    lcd.setCursor(0,1);
    lcd.print(“Kör försiktigt!”);
delay(3000);
    }
   else {
wheelSensorOK == false;
digitalWrite(ignitionPin, LOW);          
    while(){
lcd.setCursor(0,0);
    lcd.print("Hjulsensor ur");
    lcd.setCursor(0,1);
    lcd.print("funktion!");
    delay(1000);
    lcd.setCursor(0,0);
    lcd.print("  Startspärr  ");
    lcd.setCursor(0,1);
    lcd.print("  aktiverad!  ");
    delay(5000);
    }
   }
  lcd.clear();
  digitalWrite(wheelsensorTestPin1,LOW);    // Abort test
}

void loop() {

  pulseHigh = pulseIn(pulsePin, HIGH);
  pulseLow = pulseIn(pulsePin, LOW);
  pulseTotal = pulseHigh + pulseLow;        // Time period of the pulse in micro seconds.
 
  if (pulseTotal == 0) {
    digitalWrite(ignitionPin, HIGH);
    frequency = 0;
   }
  else {
    frequency = 1000000/ pulseTotal;      // Frequency in Hertz. (Hz)
    
    if (frequency   < 91) {                       // Equals a velocity of 13 km/h
    digitalWrite(ignitionPin, HIGH);
    }
    if (frequency   > 112) {            // Equals a velocity of 16 km/h
    digitalWrite(ignitionPin, LOW);
    }
velocity = frequency * 0,1425;            // Speed in km/h.
    lcd.setCursor(0,0);
    lcd.print(velocity);
    lcd.print(" km/h");
          }
}
