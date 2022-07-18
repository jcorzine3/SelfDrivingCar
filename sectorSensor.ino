#define echo_PIN1    2 // Ultrasonic Echo pin
#define trig_PIN1    3  // Ultrasonic Trig pin
#define echo_PIN2    4
#define trig_PIN2    5

long duration;
int distance;
int minDistance = 15;
int sensor1 = 0; // denotes whether sensor 1 was the last sensor triggered
int sensor2 = 1; // denotes whether sensor 2 was the last sensor triggered
double lapStart;
double lapEnd;
double lapTime;
double sector1Start;
double sector1End;
double sector1Time;

void setup() {
  pinMode(trig_PIN1, OUTPUT); 
  pinMode(echo_PIN1,INPUT);
  pinMode(trig_PIN2, OUTPUT); 
  pinMode(echo_PIN2,INPUT);  
  Serial.begin(9600);
}

/*detection of ultrasonic distance*/
int scan(int trig_PIN, int echo_PIN){
  long duration; //microseconds
  int distance;
  digitalWrite(trig_PIN,LOW);
  delayMicroseconds(2);                                                                              
  digitalWrite(trig_PIN,HIGH);
  delayMicroseconds(10);
  digitalWrite(trig_PIN,LOW);
  duration = pulseIn(echo_PIN,HIGH);
  distance = duration * 0.017; //how far away the object is in cm
  return round(distance);
}

void scanSensor1() {
  int dist = scan(trig_PIN1, echo_PIN1);
  if (dist < 15 && sensor2 == 1) {
    lapStart = millis();
    sector1Start = lapStart;
    sensor1 = 1;
    sensor2 = 0;
    Serial.print("Lap Start: ");
    Serial.println(lapStart);
  }
}

void scanSensor2() {
  int dist = scan(trig_PIN2, echo_PIN2);
  if (dist < 38 && sensor1 == 1) {
    lapEnd = millis();
    sector1End = lapEnd;
    sensor2 = 1;
    sensor1 = 0;
    lapTime = lapEnd - lapStart;
    sector1Time = lapTime;
    Serial.print("Lap End: ");
    Serial.println(lapEnd);
    Serial.println();
    Serial.print("Lap Time: ");
    Serial.println(lapTime);
    Serial.println();
  }
}

void loop() {
  //Serial.println(scan(trig_PIN1, echo_PIN1));
  //Serial.println(scan(trig_PIN2, echo_PIN2));
  //delay(500);

  scanSensor1();
  delay(10);
  scanSensor2();
  delay(10);
  
  
}
