#include <Servo.h>
Servo head;
#define SERVO_PIN 9
#define FRONT 90

int RIGHT=FRONT+33;
int LEFT=FRONT-40;

void setup() {
  
}

void loop() {
  // put your setup code here, to run once:
  head.attach(SERVO_PIN);
  head.write(LEFT);
  delay(1000);
  head.write(RIGHT);
  delay(1000);
  head.write(FRONT);
  delay(1000);
}
