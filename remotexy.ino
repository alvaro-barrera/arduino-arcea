/*
   -- New project --
   
   This source code of graphical user interface 
   has been generated automatically by RemoteXY editor.
   To compile this code using RemoteXY library 3.1.6 or later version 
   download by link http://remotexy.com/en/library/
   To connect using RemoteXY mobile app by link http://remotexy.com/en/download/                   
     - for ANDROID 4.8.01 or later version;
     - for iOS 1.5.1 or later version;
    
   This source code is free software; you can redistribute it and/or
   modify it under the terms of the GNU Lesser General Public
   License as published by the Free Software Foundation; either
   version 2.1 of the License, or (at your option) any later version.    
*/

//////////////////////////////////////////////
//        RemoteXY include library          //
//////////////////////////////////////////////

// RemoteXY select connection mode and include library 
#define REMOTEXY_MODE__HARDSERIAL

#include <RemoteXY.h>

// RemoteXY connection settings 
#define REMOTEXY_SERIAL Serial
#define REMOTEXY_SERIAL_SPEED 9600


// RemoteXY configurate  
#pragma pack(push, 1)
uint8_t RemoteXY_CONF[] =
  { 255,5,0,0,0,51,0,13,13,1,
  4,128,14,15,33,5,2,26,1,0,
  25,30,12,12,2,31,88,0,1,0,
  6,47,12,12,2,31,88,0,1,0,
  41,47,12,12,2,31,88,0,1,0,
  24,65,12,12,2,31,88,0 };
  
// this structure defines all the variables and events of your control interface 
struct {

    // input variables
  int8_t slider_1; // =0..100 slider position 
  uint8_t button_1; // =1 if button pressed, else =0 
  uint8_t button_2; // =1 if button pressed, else =0 
  uint8_t button_3; // =1 if button pressed, else =0 
  uint8_t button_4; // =1 if button pressed, else =0 

    // other variable
  uint8_t connect_flag;  // =1 if wire connected, else =0 

} RemoteXY;
#pragma pack(pop)

/////////////////////////////////////////////
//           END RemoteXY include          //
/////////////////////////////////////////////

#include <Servo.h>  

Servo servo_main;

const int PIN_BUTTON_1 = 13; //IN1 Derecha Atrás
const int PIN_BUTTON_2 = 12; //IN2 Derecha 
const int PIN_BUTTON_3 = 4; //IN3 Izquierda
const int PIN_BUTTON_4 = 5; //IN4 Izquierda Atrás


const int servo_position_center = 90;
const int servo_position_right = 0;
const int servo_position_left = 180;

const int ultra_main_trig = 8;
const int ultra_main_echo = 9;
const int ultra_secondary_trig = 3;
const int ultra_secondary_echo = 2;

void setup() 
{
  RemoteXY_Init (); 

  servo_main.attach(10); 
  RemoteXY.slider_1 = 50;
  
  pinMode (PIN_BUTTON_1, OUTPUT);
  pinMode (PIN_BUTTON_2, OUTPUT);
  pinMode (PIN_BUTTON_3, OUTPUT);
  pinMode (PIN_BUTTON_4, OUTPUT);

  
  // TODO you setup code
  
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
  RemoteXY_Handler ();

  /*int ms = (2500 - RemoteXY.slider_1 * 20); 
  servo_main.writeMicroseconds(ms);*/

  if(RemoteXY.button_1==1) {
    ultra_main = ultra(ultra_main_trig,ultra_main_echo);
    if(ultra_main > detection_distance) {
        goForward();
    }else { 
        //Alarm
    }
    /*digitalWrite(PIN_BUTTON_2, HIGH); //Derecha
    digitalWrite(PIN_BUTTON_3, HIGH); //Izquierda*/
  } else {
    stopForward();
    /*digitalWrite(PIN_BUTTON_2, LOW); //Derecha
    digitalWrite(PIN_BUTTON_3, LOW); //Izquierda*/
  }

  if(RemoteXY.button_2==1) {
    right_distance = getDistance(servo_position_right, ultra_main_trig, ultra_main_echo);
    if (right_distance > detection_distance) {
        goRight();
    }else {
        //Alarm
    }
    //digitalWrite(PIN_BUTTON_2, HIGH); //Derecha
  } else {
    //digitalWrite(PIN_BUTTON_2, LOW); Not
  }

  if(RemoteXY.button_3==1) {
    left_distance = getDistance(servo_position_left, ultra_main_trig, ultra_main_echo);
    if (left_distance > detection_distance) {
        goLeft(); 
    }else {
        //Alarm
    }
    //digitalWrite(PIN_BUTTON_3, HIGH); //Izquierda
  } else {
    //digitalWrite(PIN_BUTTON_3, LOW); Not
  }

  if(RemoteXY.button_4==1) {
    ultra_secondary = ultra(ultra_secondary_trig,ultra_secondary_echo);
    if (ultra_secondary > detection_distance) {
        goReverse();
    }else {
        //Alarm
    }
    /*digitalWrite(PIN_BUTTON_1, HIGH); //Derecha atrás
    digitalWrite(PIN_BUTTON_4, HIGH); //Izquierda atrás*/
  } else {
    stopReverse();
    /*digitalWrite(PIN_BUTTON_1, LOW); //Derecha
    digitalWrite(PIN_BUTTON_4, LOW); //Izquierda*/
  }
  
    
  // TODO you loop code
  // use the RemoteXY structure for data transfer
  // do not call delay() 


}

// Ultrasonic control function
int ultra(int Trig, int Echo)
{
  long duration;
  long distance;
  
  digitalWrite(Trig,LOW);
  delayMicroseconds(2);
  digitalWrite(Trig,HIGH);
  delayMicroseconds(10);
  digitalWrite(Trig,LOW);
  
  duration = pulseIn(Echo,HIGH);
  distance = (duration/2)/29;
  
  return distance;
}

// Get distance in any position
int getDistance(int servo_position, int ultra_trig, int ultra_echo) {
  long distance;
  servo_main.write(servo_position);
  delay(2000);
  distance = ultra(ultra_trig, ultra_echo);
  delay(20);

  return distance;
}


void goForward() {
    digitalWrite(PIN_BUTTON_2, HIGH); //Derecha
    digitalWrite(PIN_BUTTON_3, HIGH); //Izquierda
}

void stopForward() {
    digitalWrite(PIN_BUTTON_2, LOW); //Derecha
    digitalWrite(PIN_BUTTON_3, LOW); //Izquierda
}

void goRight() {
    digitalWrite(PIN_BUTTON_2, HIGH); //Derecha
}

void goLeft() {
    digitalWrite(PIN_BUTTON_3, HIGH); //Izquierda
}

void goReverse() {
    digitalWrite(PIN_BUTTON_1, HIGH); //Derecha atrás
    digitalWrite(PIN_BUTTON_4, HIGH); //Izquierda atrás
}

void stopReverse() {
    digitalWrite(PIN_BUTTON_1, LOW); //Derecha
    digitalWrite(PIN_BUTTON_4, LOW); //Izquierda
}