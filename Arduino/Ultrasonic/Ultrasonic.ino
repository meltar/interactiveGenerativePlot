///*
//* Ultrasonic Sensor HC-SR04 and Arduino Tutorial
//*
//* Modified by Melissa Holmes
//* Tutorial by Dejan Nedelkovski,
//* www.HowToMechatronics.com
//*
//*/
//

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
//  int distance1 = 0.0343 * (timedelay / 2);
  int distance1 = 0;
  distance1 = timedelay / 2;

  delayMicroseconds(2);

  digitalWrite(TriggerPIN2, LOW);
  delayMicroseconds(2);
  digitalWrite(TriggerPIN2, HIGH);
  delayMicroseconds(2);
  digitalWrite(TriggerPIN2, LOW);

  long td = pulseIn(EchoPIN2, HIGH);
//  int distance2 = 0.0343 * (td / 2);
  int distance2 = 0;
  distance2 = td / 2;

  Serial.print(distance1);
  Serial.print(",");
  Serial.println(distance2);

//  Serial.println(distance1 + "," + distance2);
}
