// This program monitors the wheelspeed by counting the frequency on the pulse from the wheel ABS sensor. 
// If it exeeds 33 km/h it cuts of the powersuply to the ignition coil.
// 
// On a Volvo 850 -97 with tyre dimension 205/55 R16 and 48 tooths on the ABS ring you get the following values:
// velocity/circumferense = rounds/sec, rounds/sec * (number of tooths on ABS ring) = pulses/sec (Hz)
// km/h / 3,6 = m/s, tyre dimension 205/55 R16 equals 1,9 m in circumferense
// 27 km/h = a pulse frequency of 189 Hz
// 30 km/h = a pulse frequency of 210 Hz
// 33 km/h = a pulse frequency of 231 Hz
//************************************************************************************************************

const int wheelsensorTestPin1 = 5; // For testing the condition of the wheelsensor.
const int wheelsensorTestPin2 = 6; //
const int pulsePin = 12; // Pin for reading the puls from the wheelsensor
const int ignitionPin = 4; // Controls the status of the ignitioncoil.
int pulseHigh;
int pulseLow;
float pulseTotal;
float frequency;


void setup() {
 
  pinMode(wheelsensorTestPin1,OUTPUT);
  pinMode(wheelsensorTestPin2,INPUT);
  pinMode(pulsePin,INPUT);
  pinMode(ignitionPin,OUTPUT);

  // Continuity test for the wheelsensor. If it's OK the ignition coil is enabled 
  // otherwise the coil is dissabled.
  
  wheelsensorTestPin1 = HIGH;   // Initiates test
  delay(100);
  if (wheelsensorTestPin2 == HIGH){
    ignitionPin = HIGH;
    }
   else {
    ignitionPin = LOW;
   }
  wheelsensorTestPin1 = LOW; // Abort test
}

void loop() {
  pulseHigh = pulseIn(pulsePin,HIGH);
  pulseLow = pulseIn(pulsePin,LOW);

  pulseTotal = pulseHigh + pulseLow; // Time period of the pulse in microseconds
  frequency = 1000000/ pulseTotal; // Frequency in Hertz (Hz)


  if (frequency < 225){   // A litlle bit of margin
   ignitionPin = HIGH;
  }
  else {
  ignitionPin = LOW;
  }
}
