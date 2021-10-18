#include <Servo.h>  

Servo servo_main;

// Rigth
const int IN2 = 13;
const int IN1 = 12;

// Left
const int IN3 = 4;
const int IN4 = 5;

// Servo
const int servo_position_center = 90;
const int servo_position_right = 0;
const int servo_position_left = 180;

// Ultrasonic
const int ultra_main_trig = 8;
const int ultra_main_echo = 9;
const int ultra_secondary_trig = 3;
const int ultra_secondary_echo = 2;

// Directions
// 1. Manual
const char MANUAL_FORWARD = '1';
const char MANUAL_REVERSE = '2';
const char MANUAL_RIGHT = '4';
const char MANUAL_LEFT = '3';
// 2. Voice
const char VOICE_FORWARD = '5';
const char VOICE_REVERSE = '6';
const char VOICE_RIGHT = '8';
const char VOICE_LEFT = '7';

const char STOP = '9';

// Speaker
const int speaker = 6;
int frequency = 220;
int counter;
const float m = 1.059; 

// Input remote: Manual or Voice
char input_remote;
char last_input_remote;

void setup() 
{
  Serial.begin(9600);  
  servo_main.attach(10); 
  pinMode (IN1, OUTPUT);
  pinMode (IN2, OUTPUT);
  pinMode (IN3, OUTPUT);
  pinMode (IN4, OUTPUT);
}

void loop() 
{ 
  int aux;
  int ultra_main;
  int ultra_secondary;
  int cont = 0;
  int detection_distance = 20;
  int left_distance;
  int right_distance;

  servo_main.write(servo_position_center);

  // Bluetooth's conection
  if(Serial.available()>0){
    input_remote = Serial.read();
  }
  
  if((String(last_input_remote).toInt() > 0 && String(last_input_remote).toInt() < 5 && String(input_remote).toInt() == 0) || 
    (String(last_input_remote).toInt() > 0 && String(last_input_remote).toInt() < 9 && last_input_remote != input_remote)) {
    stopMotor();
  }
  
  if (last_input_remote != "" && String(last_input_remote).toInt() > 4 && String(last_input_remote).toInt() < 9 && String(input_remote).toInt() == 0) {
    input_remote = last_input_remote;
  }

  last_input_remote = input_remote;

  // Forward
  if(input_remote == MANUAL_FORWARD || input_remote == VOICE_FORWARD) {
    ultra_main = ultra(ultra_main_trig, ultra_main_echo);
    if(ultra_main < detection_distance) {
      stopForward();
      alarm();
    }else { 
      goForward();
    }
  } 

  // Reverse
  if(input_remote == MANUAL_REVERSE || input_remote == VOICE_REVERSE) {
    ultra_secondary = ultra(ultra_secondary_trig, ultra_secondary_echo);
    if (ultra_secondary < detection_distance) {
      stopReverse();
      alarm();
    }else {
      goReverse();
    }
  } 

  // Right
  if(input_remote == MANUAL_RIGHT || input_remote == VOICE_RIGHT) {
    right_distance = getDistance(servo_position_right, ultra_main_trig, ultra_main_echo);
    if(right_distance < detection_distance) {         
      stopRight();
      alarm();
    }else { 
      goRight();
    }
  } 

  // Left
  if(input_remote == MANUAL_LEFT || input_remote == VOICE_LEFT) {
    left_distance = getDistance(servo_position_left, ultra_main_trig, ultra_main_echo);    
    if (left_distance < detection_distance) {
      stopLeft(); 
      alarm();
    }else {
      goLeft();
    }
  } 

  // Stop
  if(input_remote == STOP) {
    stopMotor();
  }
}

// Ultrasonic control function
long ultra(int Trig, int Echo)
{
  long duration;
  long distance;
  
  digitalWrite(Trig,LOW);
  delayMicroseconds(2);
  digitalWrite(Trig,HIGH);
  delayMicroseconds(10);
  digitalWrite(Trig,LOW);
  
  duration = pulseIn(Echo,HIGH);
  distance = duration / 58;
  
  return distance;
}

// Get distance in any position
long getDistance(int servo_position, int ultra_trig, int ultra_echo) {
  long distance;
  servo_main.write(servo_position);
  delay(2000);
  distance = ultra(ultra_trig, ultra_echo);
  delay(20);

  return distance;
}

void goForward() {
  digitalWrite(IN1, HIGH);
  digitalWrite(IN3, HIGH);
}

void stopForward() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN3, LOW);
}

void goRight() {
  digitalWrite(IN3, HIGH);
}

void stopRight() {
  digitalWrite(IN3, LOW);
}

void goLeft() {
  digitalWrite(IN1, HIGH);
}
void stopLeft() {
  digitalWrite(IN1, LOW);
}

void goReverse() {
  digitalWrite(IN2, HIGH);
  digitalWrite(IN4, HIGH);
}

void stopReverse() {
  digitalWrite(IN2, LOW);
  digitalWrite(IN4, LOW);
}

void stopMotor() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN2, LOW);
  digitalWrite(IN4, LOW);
}

void alarm(){
  for(counter = 0, frequency = 220; counter < 2; counter++){
    frequency = frequency * m; // Refresh value
    tone(speaker,frequency); 
    delay(1500);                 //1.5 seconds
    noTone(speaker); 
    delay(500);
  }
}