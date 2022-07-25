#include "SevSeg.h"
SevSeg sevseg;

#define echo_PIN1    2 // Ultrasonic Echo pin
#define trig_PIN1    3  // Ultrasonic Trig pin
#define echo_PIN2    4
#define trig_PIN2    5
#define echo_PIN3    6
#define trig_PIN3    7
#define echo_PIN4    8
#define trig_PIN4    9

long duration;
int distance;
int minDistance = 15;
int prevSensor = 0;
int sectorLocation = 1;
boolean raceStarted = false;
double lapStart = 0;
double lapEnd;
double lapTime;
double sectorStart;
double sectorEnd;
double sectorTime;

void setup() {
  //initialize 7-segement display 
  byte numDigits = 4;
  byte digitPins[] = {A8, A9, A10, A11};
  byte segmentPins[] = {A0, A1, A2, A3, A4, A5, A6, A7};
  bool resistorsOnSegments = false; // 'false' means resistors are on digit pins
  byte hardwareConfig = COMMON_CATHODE; // See README.md for options
  bool updateWithDelays = true; // Default 'false' is Recommended
  bool leadingZeros = false; // Use 'true' if you'd like to keep the leading zeros
  bool disableDecPoint = false; // Use 'true' if your decimal point doesn't exist or isn't connected. Then, you only need to specify 7 segmentPins[]
  sevseg.begin(hardwareConfig, numDigits, digitPins, segmentPins, resistorsOnSegments,
  updateWithDelays, leadingZeros, disableDecPoint);
  
  // initialize ultrasonic sensor pins
  pinMode(trig_PIN1, OUTPUT); 
  pinMode(echo_PIN1,INPUT);
  pinMode(trig_PIN2, OUTPUT); 
  pinMode(echo_PIN2,INPUT);
  pinMode(trig_PIN3, OUTPUT); 
  pinMode(echo_PIN3,INPUT);
  pinMode(trig_PIN4, OUTPUT); 
  pinMode(echo_PIN4,INPUT);    
  Serial.begin(115200);
}

/*detection of ultrasonic distance*/
int scan(int trig_PIN, int echo_PIN){
  long duration; //microseconds
  int distance;
  digitalWrite(trig_PIN,LOW);
  //delayMicroseconds(2);                                                                              
  digitalWrite(trig_PIN,HIGH);
  //delayMicroseconds(10);
  digitalWrite(trig_PIN,LOW);
  duration = pulseIn(echo_PIN,HIGH);
  distance = duration * 0.017; //how far away the object is in cm
  return round(distance);
}

void scanSensor1() {
  int dist = scan(trig_PIN1, echo_PIN1);
  if (dist < 40) {
    if (prevSensor == 0) {
      // start of the race
      lapStart = millis();
      sectorStart = lapStart;
      prevSensor = 1;
      //Serial.print("Lap Start: ");
      //Serial.println(lapStart);
      sectorLocation = 1;
      Serial.println(sectorLocation); 
      raceStarted = true;
    } else if (prevSensor == 4) {
      // next lap
      lapEnd = millis();
      sectorEnd = lapEnd;
      lapTime = lapEnd - lapStart;
      sectorTime = sectorEnd - sectorStart;
      sectorStart = sectorEnd;
      lapStart = lapEnd;
      prevSensor = 1;
      //Serial.print("Sector 4: ");
      //Serial.println(sectorTime);
      //Serial.print("Lap End: ");
      //Serial.println(lapEnd);
      //Serial.print("Lap Time: ");
      //Serial.println(lapTime);
      //Serial.println();
      //Serial.print("Lap Start: ");
      //Serial.println(lapStart);
      sectorLocation = 1;
      Serial.println(sectorLocation); 
    }
    
  }
}

void scanSensor2() {
  int dist = scan(trig_PIN2, echo_PIN2);
  if (dist < 40 && prevSensor == 1) {
    sectorEnd = millis();
    sectorTime = sectorEnd - sectorStart;
    sectorStart = sectorEnd;
    prevSensor = 2;
    //Serial.print("Sector 1: ");
    //Serial.println(sectorTime);
    sectorLocation = 2;
    Serial.println(sectorLocation);
  }
}

void scanSensor3() {
  int dist = scan(trig_PIN3, echo_PIN3);
  if (dist < 40 && prevSensor == 2) {
    sectorEnd = millis();
    sectorTime = sectorEnd - sectorStart;
    sectorStart = sectorEnd;
    prevSensor = 3;
    //Serial.print("Sector 2: ");
    //Serial.println(sectorTime);
    sectorLocation = 3;
    Serial.println(sectorLocation);
  }
}

void scanSensor4() {
  int dist = scan(trig_PIN4, echo_PIN4);
  if (dist < 40 && prevSensor == 3) {
    sectorEnd = millis();
    sectorTime = sectorEnd - sectorStart;
    sectorStart = sectorEnd;
    prevSensor = 4;
    //Serial.print("Sector 3: ");
    //Serial.println(sectorTime);
    sectorLocation = 4;
    Serial.println(sectorLocation);
  }
}

void loop() {
  //Serial.println(scan(trig_PIN3, echo_PIN3));
  //Serial.println(scan(trig_PIN2, echo_PIN2));
  //delay(500);

  scanSensor1();
  scanSensor2();
  scanSensor3();
  scanSensor4();
  sevseg.setNumber((millis() - lapStart)/1000,0);
  sevseg.refreshDisplay();
  
}
