#include <PWMServo.h>
#include<SoftwareSerial.h>

#define TX 4
#define RX 11
SoftwareSerial btSerial(RX, TX);


#define IN1 7
#define IN2 8
#define ENA 5

#define FRONT 75        // steering to front 
int SHARP_RIGHT= FRONT+40;
int SHARP_LEFT = FRONT-40;
int RIGHT= FRONT + 35;
int LEFT= FRONT - 35;
int SLIGHT_RIGHT = FRONT + 20;
int SLIGHT_LEFT = FRONT - 20;
int slightRight = SLIGHT_RIGHT;
int slightLeft = SLIGHT_LEFT;
int left = LEFT;
int right = RIGHT;

// IR Sensors
#define LFSensor_0 A0 
#define LFSensor_1 A1
#define LFSensor_2 A2
#define LFSensor_3 A3
#define LFSensor_4 A4
char sensor[5];
int prevTurn = 0; // 0 denotes straight, 1 denotes right, -1 denotes left
int sector = 1;
int prevSector = 0;
int lapTime = 0;

int carSpeed = 125;
int turnSpeed = 200;
int reverseSpeed = 250;
#define MAX_SPEED 250

#define SERVO_PIN 9
PWMServo head;

void reverse(int speed) {
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  analogWrite(ENA, speed);
}

void throttle(int speed) {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2,HIGH);
  analogWrite(ENA,speed);
}

void accelerate(int milliseconds) {
  throttle(MAX_SPEED);
  delay(milliseconds);
}

void turn(int angle) {
  head.write(angle);
}

void brake() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2,LOW);
  analogWrite(ENA,0);
}

void setup() {
  pinMode(ENA, OUTPUT); 
  pinMode(IN1, OUTPUT); 
  pinMode(IN2, OUTPUT); 
  head.attach(SERVO_PIN);
  turn(FRONT);  
  brake();
  pinMode(RX, INPUT);
  pinMode(TX, OUTPUT);
  btSerial.begin(9600);
  Serial.begin(9600);
}

void loop() {
  if (btSerial.available() > 0) {
    sector = btSerial.readStringUntil('\n').toInt();
    Serial.println(sector);
  }
  sectortracking();
  //linetracking();
  
}

void sectortracking() {
  if ((sector == 1 || sector == 3) && (prevSector != 1 && prevSector != 3)) {
    // set preconditions for straight
    carSpeed = 125;
    turn(FRONT);
    // set adjustment settings for straight
    int slightRight = SLIGHT_RIGHT;
    int slightLeft = SLIGHT_LEFT;
    int left = LEFT;
    int right = RIGHT;
    // set previous sector
    prevSector = sector;
  } else if ((sector == 2 || sector == 4) && (prevSector != 2 && prevSector != 4)) {
    // set preconditions for turn
    carSpeed = 100;
    turn(LEFT);
    // set adjustment setting for turn
    int slightRight = RIGHT;
    int slightLeft = SLIGHT_LEFT;
    int left = LEFT;
    int right = SHARP_RIGHT;
    // set previous sector
    prevSector = sector;
  }
}

String readIR() {
  int sensorVal_DEC = 32;
  sensor[0] = digitalRead(LFSensor_0);
  sensor[1] = digitalRead(LFSensor_1);
  sensor[2] = digitalRead(LFSensor_2);
  sensor[3] = digitalRead(LFSensor_3);
  sensor[4] = digitalRead(LFSensor_4);
  sensorVal_DEC += sensor[0]*16 + sensor[1]*8 + sensor[2]*4 + sensor[3]*2 + sensor[4];
  String sensorString = String(sensorVal_DEC,BIN).substring(1,6);
  //Serial.println("Sensor String: " + sensorString);
  //Serial.println();
  return sensorString;
}

void linetracking() {
  String irString = readIR();
  
  if (irString == "10000" || irString == "11000") {
        turn(slightRight);
        if (prevTurn != 1) accelerate(250);
        throttle(turnSpeed);
        prevTurn = 1;
  }
  
  if (irString == "11100" || irString == "11110") {
        turn(right);
        if (prevTurn != 2) accelerate(250);
        throttle(turnSpeed);
        prevTurn = 2;
  }

  if (irString == "00001" || irString == "00011") {
        turn(slightLeft);
        if (prevTurn != -1) accelerate(250);
        throttle(turnSpeed);
        prevTurn = -1;
  }

  if (irString == "00111" || irString == "01111") {
        turn(left);
        if (prevTurn != -2) accelerate(250);
        throttle(turnSpeed);
        prevTurn = -2;
  }

  if (irString == "00000") {
      turn(FRONT);
      throttle(carSpeed);
      prevTurn = 0;
  }

  if (irString == "11111") {
      brake();
      if (prevTurn == 1 || prevTurn == 2) {
        turn(LEFT);
        while (readIR() == "11111" ) {
          reverse(reverseSpeed);
        }
        prevTurn == -2;
      } else if (prevTurn == -1 || prevTurn == -2) {
        turn(RIGHT);
        while (readIR() == "11111") {
          reverse(reverseSpeed);
        }
        prevTurn == 2;
      }
      
  }
}
