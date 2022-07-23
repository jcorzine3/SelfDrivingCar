#include <Servo.h>
Servo head;
#define SERVO_PIN 9
#define FRONT 80

int RIGHT=FRONT+45;
int LEFT=FRONT-45;

void setup() {
  head.attach(SERVO_PIN);
}

void loop() {
  // put your setup code here, to run once:
  head.write(FRONT);
  delay(1000);
  head.write(LEFT);
  delay(1000);
  head.write(FRONT);
  delay(1000);
  head.write(RIGHT);
  delay(1000);
}
