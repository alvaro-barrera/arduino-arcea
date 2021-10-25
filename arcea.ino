#include <Servo.h>  

Servo servo_main;

// Motor - Right
const int IN2 = 13;
const int IN1 = 12;
// Motor - Left
const int IN3 = 4;
const int IN4 = 5;

// Servo
const int servo_pin = 10;
const int servo_position_center = 90;
const int servo_position_right = 0;
const int servo_position_left = 180;

// Ultrasonic
const int ultra_main_trig = 8;
const int ultra_main_echo = 9;
const int ultra_secondary_trig = 3;
const int ultra_secondary_echo = 2;

// Movement direction's values
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
// Shared
const char STOP = '9';

// Speaker
const int speaker_pin = 6;
int frequency = 220;
int counter;
float refresh_value = 1.059; 

// Input remote: Manual or Voice
char input_remote;
char last_input_remote;

// Detection distance in cm
const long detection_distance = 20.0;

// Initializing variables for distance measurement
long front_distance;
long back_distance;
long left_distance;
long right_distance;

void setup() 
{
  Serial.begin(9600);  
  servo_main.attach(servo_pin); 
  pinMode (IN1, OUTPUT);
  pinMode (IN2, OUTPUT);
  pinMode (IN3, OUTPUT);
  pinMode (IN4, OUTPUT);
}

void loop() 
{ 
  // Move the main ultrasonic to the center
  servo_main.write(servo_position_center);

  // Bluetooth's conection
  if(Serial.available() > 0){
    input_remote = Serial.read();
  }
  
  // When the last input of remote control is in range of manual values and current value is zero (No command), 
  // or the last value is in range of manual or voice values and the last value is different to current value, then stop motor
  if((String(last_input_remote).toInt() > 0 && String(last_input_remote).toInt() < 5 && String(input_remote).toInt() == 0) || 
    (String(last_input_remote).toInt() > 0 && String(last_input_remote).toInt() < 9 && last_input_remote != input_remote)) {
    stopMotor();
  }
  
  // When the last input is not empty, is in range of voice values and current is zero (No command), then the last value remains active
  if (last_input_remote != "" && String(last_input_remote).toInt() > 4 && String(last_input_remote).toInt() < 7 && String(input_remote).toInt() == 0) {
    input_remote = last_input_remote;
  }

  // Save the last value
  last_input_remote = input_remote;

  // Forward
  if(input_remote == MANUAL_FORWARD || input_remote == VOICE_FORWARD) {
    front_distance = getDistance(servo_position_center, ultra_main_trig, ultra_main_echo);
    if(front_distance < detection_distance) {
      stopForward();
      activateAlarm();
    }else { 
      goForward();
    }
  } 

  // Reverse
  if(input_remote == MANUAL_REVERSE || input_remote == VOICE_REVERSE) {
    back_distance = getDistanceByUltra(ultra_secondary_trig, ultra_secondary_echo);
    if (back_distance < detection_distance) {
      stopReverse();
      activateAlarm();
    }else {
      goReverse();
    }
  } 

  // Right
  if(input_remote == MANUAL_RIGHT || input_remote == VOICE_RIGHT) {
    right_distance = getDistance(servo_position_right, ultra_main_trig, ultra_main_echo);
    if(right_distance < detection_distance) {         
      stopRight();
      activateAlarm();
    }else { 
      goRight();
      delay(1500);
      stopRight();
    }
  } 

  // Left
  if(input_remote == MANUAL_LEFT || input_remote == VOICE_LEFT) {
    left_distance = getDistance(servo_position_left, ultra_main_trig, ultra_main_echo);    
    if (left_distance < detection_distance) {
      stopLeft(); 
      activateAlarm();
    }else {
      goLeft();
      delay(1500);
      stopLeft();
    }
  } 

  // Stop
  if(input_remote == STOP) {
    stopMotor();
  }
}

// Ultrasonic control function
long getDistanceByUltra(int Trig, int Echo)
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
  distance = getDistanceByUltra(ultra_trig, ultra_echo);
  delay(20);

  return distance;
}

// Activating alarm with different frequencies
void activateAlarm(){
  for(counter = 0, frequency = 220; counter < 2; counter++){
    frequency = frequency * refresh_value; // Refresh value
    tone(speaker_pin,frequency); 
    delay(1500);                 //1.5 seconds
    noTone(speaker_pin); 
    delay(500);
  }
}

/*
* Switching motors on and off in different directions
**/ 

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