#include <PWMServo.h>

#define IN1 7
#define IN2 8
#define ENA 5

#define FRONT 90        // steering to front 
int SHARP_RIGHT=FRONT+25;
int SHARP_LEFT=FRONT-32;
int RIGHT=FRONT+16;
int LEFT=FRONT-20;
int SLIGHT_RIGHT = FRONT + 8;
int SLIGHT_LEFT = FRONT - 10;

// IR Sensors
#define LFSensor_0 A0 
#define LFSensor_1 A1
#define LFSensor_2 A2
#define LFSensor_3 A3
#define LFSensor_4 A4
char sensor[5];
int prevTurn = 0; // 0 denotes straight, 1 denotes right, -1 denotes left

#define SPEED 200

#define SERVO_PIN 9
PWMServo head;

void reverse(int speed) {
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  analogWrite(ENA, speed);
}

void forward(int speed) {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2,HIGH);
  analogWrite(ENA,speed);
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

void linetracking() {
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

  if (sensorString == "10000" || sensorString == "11000") {
        turn(SLIGHT_RIGHT);
        prevTurn = 1;
  }
  
  if (sensorString == "11100" || sensorString == "11110") {
        turn(RIGHT);
        forward(SPEED);
        prevTurn = 1;
  }

  if (sensorString == "00001" || sensorString == "00011") {
        turn(SLIGHT_LEFT);
        prevTurn = -1;
  }

  if (sensorString == "00111" || sensorString == "01111") {
        turn(LEFT);
        forward(SPEED);
        prevTurn = -1;
  }

  if (sensorString == "00000") {
    forward(SPEED);
    prevTurn = 0;
  }

  if (sensorString == "11111") {
    brake();
    if (prevTurn == 1) {
      turn(LEFT);
    } else if (prevTurn == -1) {
      turn(RIGHT);
    }
    reverse(SPEED);
  }
}
