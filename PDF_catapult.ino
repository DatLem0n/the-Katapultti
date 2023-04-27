#include <Servo.h>

Servo launchServo;

const int motorSpeedPin = 5;
const int motorDirPin1 = 6;
const int motorDirPin2 = 7;
const int IRSensorPin = 13;
const int servoPin = 9;
const int buttonPin = A0;
int treshold = 0;
int val = 0;

bool motorDir = 0;
bool loaded = false;
bool blocked = true;


void changeMotorDir(void)
{
  if (!motorDir)
  {
    digitalWrite(motorDirPin1, HIGH);
    digitalWrite(motorDirPin2, LOW);
    motorDir = 1;
  }
  else
  {
    digitalWrite(motorDirPin1, LOW);
    digitalWrite(motorDirPin2, HIGH);
    motorDir = 0;
  }
}

void motorSpeed(int speed)
{
  if (speed >= 0 && speed <= 255)
  {
    analogWrite(motorSpeedPin, speed);
  }
}

void load(void)
{
  changeMotorDir();
  do
  {
    val = analogRead(buttonPin);  // read the input pin
    Serial.println(val);
    motorSpeed(255);
  }
  while (analogRead(buttonPin) > treshold);
  
  launchServo.write(180);
  delay(1500);
  motorSpeed(0);
  changeMotorDir();
  do
  {
    motorSpeed(255);
    delay(500); // shorten delay once sensor is here
  }
  while (analogRead(buttonPin) > treshold);
  motorSpeed(0);
  loaded = true;
  
  
}

void launch(void)
{
  loaded = false;
  launchServo.write(0);
  delay(2000);
  load();
}

void setup()
{
  pinMode(motorSpeedPin, OUTPUT);
  pinMode(motorDirPin1, OUTPUT);
  pinMode(motorDirPin2, OUTPUT);
  pinMode(IRSensorPin, INPUT);
  pinMode(servoPin, OUTPUT);
  pinMode(buttonPin, INPUT);

  Serial.begin(9600);
  treshold = analogRead(buttonPin) - 200;
  launchServo.attach(servoPin);
  launchServo.write(0);
  load();
}

void loop()
{
  val = analogRead(buttonPin);  // read the input pin
  Serial.println(val);
  if(digitalRead(IRSensorPin) == LOW && loaded)
  {
    launch();
  }

}
