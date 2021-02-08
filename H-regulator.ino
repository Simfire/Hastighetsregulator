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

// initialize the library by associating any needed LCD interface pin
// with the arduino pin number it is connected to
// rs = register select, en = enable, d0 - d3 = 4 bit data
const int  rs = 7, en = 6, d4 = 5, d5 = 4, d6 = 3, d7 = 2 ;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

bool wheelSensorOK;        // Status of wheel sensor.
const int wheelSensorTestPin1 = 8;  // For testing the condition of the wheelsensor.
const int wheelSensorTestPin2 = 11;  //              - " -
const int pulsePin = 12;            // Pin for reading the puls from the wheelsensor.
const int ignitionPin = 10;         // Controls the status of the ignitioncoil.
long pulseHigh;                      // High duration time of pulse.
long pulseLow;                       // Low duration time of pulse.
float pulseTotal;                   // Period time of the pulse.
float frequency;                    // Frequency of the pulse.
float velocity;                     // Speed of wheel.

void setup() {
 
  pinMode(wheelSensorTestPin1,OUTPUT);
  pinMode(wheelSensorTestPin2,INPUT);
  pinMode(pulsePin,INPUT);
  pinMode(ignitionPin,OUTPUT);
  wheelSensorOK == true;
  lcd.begin(16,2);
  lcd.clear();
 
  // Continuity test for the wheel sensor. If it's OK the ignition coil is enabled
  // otherwise the coil is dissabled.
  digitalWrite(ignitionPin,HIGH);            // Power to ignition coil OFF.
  digitalWrite(wheelSensorTestPin1,HIGH);   // Initiates test
  lcd.setCursor(0,0);
  lcd.print("Test hjulsensor");
  delay(1000);
 
  if (digitalRead(wheelSensorTestPin2) == HIGH){
    wheelSensorOK = true;
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Hjulsensor OK!");
    delay(3000);
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("K");
    lcd.print(char(239));
    lcd.print("r f");
    lcd.print(char(239));
    lcd.print("rsiktigt!");
    delay(4000);
    }
   else {
    wheelSensorOK = false;
    digitalWrite(ignitionPin, LOW);          
   while(1){
    lcd.clear();
    lcd.setCursor(1,0);
    lcd.print("Hjulsensor ur");
    lcd.setCursor(3,1);
    lcd.print("funktion!");
    delay(2000);
    lcd.clear();
    lcd.setCursor(3,0);
    lcd.print("Startsp");
    lcd.print(char(225));
    lcd.print("rr");
    lcd.setCursor(3,1);
    lcd.print("aktiverad!");
    delay(2000);
//    }
    }
//  lcd.clear();
  digitalWrite(wheelSensorTestPin1,LOW);    // Abort test
  lcd.setCursor(0,1);
    lcd.print("Hjulsensor OK!");
    delay(3000);
  }
}
void loop() {
//  lcd.clear();
  
  pulseHigh = pulseIn(pulsePin, HIGH);
  pulseLow = pulseIn(pulsePin, LOW);
  pulseTotal = pulseHigh + pulseLow;        // Time period of the pulse in micro seconds.
  
  if (pulseTotal == 0) {
    digitalWrite(ignitionPin, HIGH);
    frequency = 0;
   }
  else {
    frequency = 1000000/ pulseTotal;      // Frequency in Hertz. (Hz)
    
    if (frequency < 200) {                       // Equals a velocity of 13 km/h
    digitalWrite(ignitionPin, HIGH);
//    lcd.setCursor(0,0);
//    lcd.print(frequency);
//    lcd.print("  Hz");
//    delay(500);
    }
    if (frequency > 230) {            // Equals a velocity of 16 km/h
    digitalWrite(ignitionPin, LOW);
    }
  velocity = frequency * 0.1425; // Speed in km/h.
  lcd.clear();
  lcd.setCursor(0,1);
  lcd.print(frequency);
  lcd.print(" Hz");
  lcd.setCursor(0,0);
  lcd.print(velocity);
  lcd.print(" km/h");
  delay(500);
    
          }
}
