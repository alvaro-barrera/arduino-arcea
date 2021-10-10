#include <Servo.h> 

Servo servo_main;

const int servo_position_center = 90;
const int servo_position_right = 0;
const int servo_position_left = 180;

const int ultra_main_trig = 10;
const int ultra_main_echo = 9;
const int ultra_secondary_trig = 3;
const int ultra_secondary_echo = 2;

const int IN1 = 12;
const int IN2 = 13;
const int ENA = 11;
const int IN3 = 4;
const int IN4 = 5;
const int ENB = 6;

const String FORWARD = "forward";
const String REVERSE = "reverse";
const String RIGHT = "right";
const String LEFT = "left";
const String STOP = "stop";


void setup(){
  servo_main.attach(7);
  //Motors' Output pins

  pinMode(IN1, OUTPUT);   
  pinMode(IN2, OUTPUT);   
  pinMode(ENA, OUTPUT);   

  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
  pinMode(ENB, OUTPUT);
}

void loop(){
  int aux;
  int ultra_main;
  int ultra_secondary;
  int cont = 0;
  int detection_distance = 10;
  servo_main.write(servo_position_center);

  //Activating the main ultrasonic
  ultra_main = ultra(ultra_main_trig,ultra_main_echo);
  delay(20);
  //Activating the secondary ultrasonic
  ultra_secondary = ultra(ultra_secondary_trig,ultra_secondary_echo);
  delay(20);

  //Activated when ultrasonics detects objects within detection_distancecm
  if(ultra_main < detection_distance || ultra_secondary < detection_distance)  
  {
    aux = sele();
       while(aux < 30 && ultra_secondary < aux)
       {
         aux = sele();
         delay(500);
       }
      motor(FORWARD,180,180);  
      delay(1000);
      //delay(1000);
      servo_main.write(servo_position_left);
    //}
    // else
    // {
    //   aux = sele();
    //   while(aux < 30 && ultra_secondary < aux)
    //   {
    //     aux = sele();
    //     delay(500);
    //   }
    // }
  }else {
    motor(REVERSE,180,180); 
    delay(20);
    servo_main.write(servo_position_center);
    delay(1000);
  }
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

// Motor control function
void motor(String dir,int m1,int m2)
{
  if(dir == "forward")
  {
    digitalWrite(IN1,HIGH);
    digitalWrite(IN2,LOW);
    digitalWrite(IN3,HIGH);
    digitalWrite(IN4,LOW);
  }
  else if(dir == "reverse")
  {
    digitalWrite(IN1,LOW);
    digitalWrite(IN2,HIGH);
    digitalWrite(IN3,LOW);
    digitalWrite(IN4,HIGH);
  }
  else if(dir == "right")
  {
    digitalWrite(IN1,HIGH);
    digitalWrite(IN2,LOW);
    digitalWrite(IN3,LOW);
    digitalWrite(IN4,HIGH);
  }
  else if(dir == "left")
  {
    digitalWrite(IN1,LOW);
    digitalWrite(IN2,HIGH);
    digitalWrite(IN3,HIGH);
    digitalWrite(IN4,LOW);
  }
  else if(dir == "stop")
  {
    digitalWrite(IN1,LOW);
    digitalWrite(IN2,LOW);
    digitalWrite(IN3,LOW);
    digitalWrite(IN4,LOW);
  }

  analogWrite(ENA,m1);
  analogWrite(ENB,m2);
}

int getDistance(servo_position, ultra_trig, ultra_echo) {
  servo_main.write(servo_position);
  delay(2000);
  distance = ultra(ultra_trig, ultra_echo);
  delay(20);

  return distance;
}

// Function that detects whether the left or right path is the longest one
int sele()             
{
  int left_distance;
  int right_distance;
  int aux;
  motor(STOP,0,0);
  delay(200);

  /*servo_main.write(servo_position_right);
  delay(2000);
  right_distance = ultra(ultra_main_trig,ultra_main_echo);
  delay(20);*/

  right_distance = getDistance(servo_position_right, ultra_main_trig, ultra_main_echo);
  left_distance = getDistance(servo_position_left, ultra_main_trig, ultra_main_echo);

  servo_main.write(servo_position_center);
  delay(500);
  
  if(right_distance > left_distance)
  {
    aux = ultra(ultra_secondary_trig,ultra_secondary_echo);
    delay(20);
    while(aux < de)
    {
      motor(2,150,150);
      aux = ultra(ultra_secondary_trig,ultra_secondary_echo);
      delay(20);
    }
    return right_distance;
  }
    
  else
  {
    aux = ultra(ultra_secondary_trig,ultra_secondary_echo);
    delay(20);
    while(aux < left_distance)
    {
      motor(-2,150,150);
      aux = ultra(ultra_secondary_trig,ultra_secondary_echo);
      delay(20);
    }
    return left_distance;
  }
}