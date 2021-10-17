#include <Servo.h>  

Servo servo_main;

const int IN2 = 13; //IN1 Derecha Atrás
const int IN1 = 12; //IN2 Derecha 

const int IN3 = 4; //IN3 Izquierda
const int IN4 = 5; //IN4 Izquierda Atrás


const int servo_position_center = 90;
const int servo_position_right = 0;
const int servo_position_left = 180;

const int ultra_main_trig = 8;
const int ultra_main_echo = 9;
const int ultra_secondary_trig = 3;
const int ultra_secondary_echo = 2;

const String MANUAL_FORWARD = "manual_forward";
const String MANUAL_REVERSE = "manual_reverse";
const String MANUAL_RIGHT = "manual_right";
const String MANUAL_LEFT = "manual_left";

const String VOICE_FORWARD = "voice_forward";
const String VOICE_REVERSE = "voice_rverse";
const String VOICE_RIGHT = "voice_right";
const String VOICE_LEFT = "voice_left";

const String STOP = "stop";
const String ROTATE = "rotate";

const int speaker = 6;
int frequency = 220;    // frequency correspondiente a la nota La
int counter;          // variable para el counter
const float m = 1.059; 

 
/*

input_remote=='1' => forward
input_remote=='2' => reverse
input_remote=='3' => left
input_remote=='4' => right
*/


char input_remote;
char last_input_remote = "";

void setup() 
{
  Serial.begin(9600);  
  servo_main.attach(10); 
    MANUAL_LEFT
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

  // conexion a bluetooth
  if(Serial.available()>0){
    input_remote = Serial.read();
  }

  if (last_input_remote != "" && last_input_remote.find("voice") != std::string::npos && input_remote == "empty") {
    input_remote = last_input_remote;
  }

  last_input_remote = last_input_remote;

  Serial.println("input_remote: "+ String(input_remote));

  // Forward
  if(input_remote == MANUAL_FORWARD) {
    ultra_main = ultra(ultra_main_trig,ultra_main_echo);
    
    if(ultra_main < detection_distance) {
        stopForward();
        alarm();
    }else { 
        goForward();
        //Alarm
    }
    /*digitalWrite(IN1, HIGH); //Derecha
    digitalWrite(IN3, HIGH); //Izquierda*/
  } else {
    // stopForward();
    /*digitalWrite(IN1, LOW); //Derecha
    digitalWrite(IN3, LOW); //Izquierda*/
  }

  if(input_remote == MANUAL_RIGHT) {
    right_distance = getDistance(servo_position_right, ultra_main_trig, ultra_main_echo);
    if(right_distance < detection_distance) {         
        //  alarm();        
         stopRight();
    }else { 
        // alarm();        
        goRight();
        //Alarm
    }
  } else {
    //digitalWrite(IN1, LOW); Not
  }

  if(input_remote == MANUAL_LEFT) {
    left_distance = getDistance(servo_position_left, ultra_main_trig, ultra_main_echo);    
    if (left_distance < detection_distance) {
      stopLeft(); 
    }else {
      goLeft();
    }
    //digitalWrite(IN3, HIGH); //Izquierda
  } else {
    //digitalWrite(IN3, LOW); Not
  }

  if(input_remote == MANUAL_REVERSE) {
    ultra_secondary = ultra(ultra_secondary_trig,ultra_secondary_echo);
    if (ultra_secondary > detection_distance) {
        goReverse();
    }else {
        //Alarm
    }
    /*digitalWrite(IN2, HIGH); //Derecha atrás
    digitalWrite(IN4, HIGH); //Izquierda atrás*/
  } else {
    // stopReverse();
    /*digitalWrite(IN2, LOW); //Derecha
    digitalWrite(IN4, LOW); //Izquierda*/
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
  digitalWrite(IN1, HIGH); //Derecha
  digitalWrite(IN3, HIGH); //Izquierda
}

void stopForward() {
  digitalWrite(IN1, LOW); //Derecha
  digitalWrite(IN3, LOW); //Izquierda
}

void goRight() {
  digitalWrite(IN3, HIGH); //Derecha
}
void stopRight() {
  digitalWrite(IN3, LOW); //Derecha
}

void goLeft() {
  digitalWrite(IN1, HIGH); //Izquierda
}
void stopLeft() {
  digitalWrite(IN1, LOW); //Izquierda
}

void goReverse() {
  digitalWrite(IN2, HIGH); //Derecha atrás
  digitalWrite(IN4, HIGH); //Izquierda atrás
}

void stopReverse() {
  digitalWrite(IN2, LOW); //Derecha
  digitalWrite(IN4, LOW); //Izquierda
}

void stopMotor() {
  digitalWrite(IN1, LOW); //Derecha
  digitalWrite(IN3, LOW); //Izquierda
  digitalWrite(IN2, LOW); //Derecha
  digitalWrite(IN4, LOW); //Izquierda
}

void alarm(){
  for(counter=0,frequency=220;counter < 2;counter++){
    frequency=frequency*m;     // actualiza la frequency
    tone(speaker,frequency); // emite el tono
    delay(1500);                 // lo mantiene 1.5 segundos
    noTone(speaker);          // para el tono
    delay(500);                  // espera medio segundo
  }
}