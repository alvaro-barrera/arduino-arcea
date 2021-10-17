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

Servo myservo;

#define PIN_BUTTON_1 13 //IN1 Derecha Atrás
#define PIN_BUTTON_2 12 //IN2 Derecha 
#define PIN_BUTTON_3 4 //IN3 Izquierda
#define PIN_BUTTON_4 5 //IN4 Izquierda Atrás


void setup() 
{
  RemoteXY_Init (); 

  myservo.attach(10); 
  RemoteXY.slider_1 = 50;
  
  pinMode (PIN_BUTTON_1, OUTPUT);
  pinMode (PIN_BUTTON_2, OUTPUT);
  pinMode (PIN_BUTTON_3, OUTPUT);
  pinMode (PIN_BUTTON_4, OUTPUT);

  
  // TODO you setup code
  
}

void loop() 
{ 
  RemoteXY_Handler ();

  int ms = (2500 - RemoteXY.slider_1 * 20); 
  myservo.writeMicroseconds(ms);

  if(RemoteXY.button_1==1) {
    goForward();
    /*digitalWrite(PIN_BUTTON_2, HIGH); //Derecha
    digitalWrite(PIN_BUTTON_3, HIGH); //Izquierda*/
  } else {
    stopForward()
    /*digitalWrite(PIN_BUTTON_2, LOW); //Derecha
    digitalWrite(PIN_BUTTON_3, LOW); //Izquierda*/
  }

  if(RemoteXY.button_2==1) {
    goRight();
    //digitalWrite(PIN_BUTTON_2, HIGH); //Derecha
  } else {
    //digitalWrite(PIN_BUTTON_2, LOW); Not
  }

  if(RemoteXY.button_3==1) {
    goLeft(); 
    //digitalWrite(PIN_BUTTON_3, HIGH); //Izquierda
  } else {
    //digitalWrite(PIN_BUTTON_3, LOW); Not
  }

  if(RemoteXY.button_4==1) {
    goReverse();
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