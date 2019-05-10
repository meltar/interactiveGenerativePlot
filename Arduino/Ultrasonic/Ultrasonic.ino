///*
//* Ultrasonic Sensor HC-SR04 and Arduino Tutorial
//*
//* by Dejan Nedelkovski,
//* www.HowToMechatronics.com
//*
//*/
//
//// defines pins numbers
//const int trigPin = 11;
//const int echoPin = 10;
//
//// defines variables
//long duration;
//int distance;
//
//void setup() {
//pinMode(trigPin, OUTPUT); // Sets the trigPin as an Output
//pinMode(echoPin, INPUT); // Sets the echoPin as an Input
//Serial.begin(9600); // Starts the serial communication
//}
//
//void loop() {
//// Clears the trigPin
//digitalWrite(trigPin, LOW);
//delayMicroseconds(2);
//
//// Sets the trigPin on HIGH state for 10 micro seconds
//digitalWrite(trigPin, HIGH);
//delayMicroseconds(100);
//digitalWrite(trigPin, LOW);
//
//// Reads the echoPin, returns the sound wave travel time in microseconds
//duration = pulseIn(echoPin, HIGH);
//
//// Calculating the distance
//distance= duration*0.034/2;
//
//// Prints the distance on the Serial Monitor
//Serial.print("Distance: ");
//Serial.println(distance);
//}

int TriggerPIN1 = 11;
int EchoPIN1 = 10;
int TriggerPIN2 = 9;
int EchoPIN2 = 8;

void setup() {
  pinMode(TriggerPIN1, OUTPUT);
  pinMode(EchoPIN2, INPUT);
  pinMode(TriggerPIN2, OUTPUT);
  pinMode(EchoPIN2, INPUT);
  Serial.begin(9600);
}

void loop() {
  digitalWrite(TriggerPIN1, LOW);
  delayMicroseconds(2);
  digitalWrite(TriggerPIN1, HIGH);
  delayMicroseconds(2);
  digitalWrite(TriggerPIN1, LOW);

  long timedelay = pulseIn(EchoPIN1, HIGH);
  int distance1 = 0.0343 * (timedelay / 2);
//  int distance1 = 0;
//  distance1 = timedelay / 2;

  delayMicroseconds(2);

  digitalWrite(TriggerPIN2, LOW);
  delayMicroseconds(2);
  digitalWrite(TriggerPIN2, HIGH);
  delayMicroseconds(2);
  digitalWrite(TriggerPIN2, LOW);

  long td = pulseIn(EchoPIN2, HIGH);
  int distance2 = 0.0343 * (td / 2);
//  int distance2 = 0;
//  distance2 = td / 2;

  if(distance1 > 1500){
    distance1 = 0;
  }

  if(distance2 > 1500){
    distance2 = 0;
  }
  Serial.print(distance1);
  Serial.print(",");
  Serial.println(distance2);
//Serial.println(0);

//  Serial.println(distance1 + "," + distance2);
}
