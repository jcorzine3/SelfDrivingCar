#include <PWMServo.h>
#include <SoftwareSerial.h>
SoftwareSerial BLTSerial(11, 4);  //RX,TX

#define IN1 7
#define IN2 8
#define ENA 5

#define FRONT 80        // steering to front 
int SHARP_RIGHT=FRONT+30;
int SHARP_LEFT=FRONT-30;
int RIGHT=FRONT+20;
int LEFT=FRONT-20;
int SLIGHT_RIGHT = FRONT + 10;
int SLIGHT_LEFT = FRONT - 10;

// IR Sensors
#define LFSensor_0 A0 
#define LFSensor_1 A1
#define LFSensor_2 A2
#define LFSensor_3 A3
#define LFSensor_4 A4
char sensor[5];
int prevTurn = 0; // 0 denotes straight, 1 denotes right, -1 denotes left

#define SPEED 125
#define TURN_SPEED 200
#define REVERSE_SPEED 250
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
  Serial.begin(9600);
}

void loop() {
  linetracking();
  
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
  Serial.println("Sensor String: " + sensorString);
  Serial.println();
  return sensorString;
}

void linetracking() {
  String irString = readIR();
  
  if (irString == "10000" || irString == "11000") {
        if (prevTurn != 1) accelerate(250);
        turn(RIGHT);
        throttle(TURN_SPEED);
        prevTurn = 1;
  }
  
  if (irString == "11100" || irString == "11110") {
        if (prevTurn != 2) accelerate(250);
        turn(SHARP_RIGHT);
        throttle(TURN_SPEED);
        prevTurn = 2;
  }

  if (irString == "00001" || irString == "00011") {
        if (prevTurn != -1) accelerate(250);
        turn(LEFT);
        throttle(TURN_SPEED);
        prevTurn = -1;
  }

  if (irString == "00111" || irString == "01111") {
        if (prevTurn != -2) accelerate(250);
        turn(SHARP_LEFT);
        throttle(TURN_SPEED);
        prevTurn = -2;
  }

  if (irString == "00000") {
      turn(FRONT);
      throttle(SPEED);
      prevTurn = 0;
  }

  if (irString == "11111") {
      brake();
      if (prevTurn == 1 || prevTurn == 2) {
        turn(SHARP_LEFT);
        while (readIR() == "11111" ) {
          reverse(REVERSE_SPEED);
        }
        prevTurn == -2;
      } else if (prevTurn == -1 || prevTurn == -2) {
        turn(SHARP_RIGHT);
        while (readIR() == "11111") {
          reverse(REVERSE_SPEED);
        }
        prevTurn == 2;
      }
      
  }
}
