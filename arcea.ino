#include <Servo.h> 

Servo servo_main;

const int servo_position_center = 90;
const int servo_position_right = 0;
const int servo_position_left = 180;

const int ultra_main_trig = 8;
const int ultra_main_echo = 9;
const int ultra_secondary_trig = 3;
const int ultra_secondary_echo = 2;

// Right
const int IN1 = 12;
const int IN2 = 13;
const int ENA = 11;
// Left
const int IN3 = 4;
const int IN4 = 5;
const int ENB = 6;

const String FORWARD = "forward";
const String REVERSE = "reverse";
const String RIGHT = "right";
const String LEFT = "left";
const String STOP = "stop";
const String ROTATE = "rotate";

// Rotate time
const int turning_time = 2000;


void setup(){
  Serial.begin(9600);//iniciailzamos la comunicación

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
  int detection_distance = 20;
  servo_main.write(servo_position_center);

  //Activating the main ultrasonic
  ultra_main = ultra(ultra_main_trig,ultra_main_echo);
  delay(20);
  //Activating the secondary ultrasonic
  ultra_secondary = ultra(ultra_secondary_trig,ultra_secondary_echo);
  delay(20);

  //Activated when ultrasonics detects objects within detection_distance cm
  if(ultra_main < detection_distance || ultra_secondary < detection_distance)  
  {
    aux = getLongestWay();
    motor(STOP,150,150);

    while(aux < detection_distance)
    {
      motor(REVERSE,180,180); 
      motor(STOP,150,150);
      aux = getLongestWay();
      delay(500);

    }
    /*if (aux < detection_distance)
    {
      if (ultra_secondary > detection_distance)
      {
        motor(REVERSE,180,180); 
        motor(STOP,150,150);
      }
    }*/
    
    motor(STOP,150,150);
    motor(FORWARD,180,180); 
    /*Serial.print(aux);
        
    //while(aux < detection_distance && ultra_secondary < aux)
    while(false)
    {
      aux = getLongestWay();
      delay(500);
    }
    //motor(FORWARD,180,180);  
    delay(1000);
    servo_main.write(servo_position_left);*/
  }else {
    motor(FORWARD,180,180); 
    /*delay(20);
    servo_main.write(servo_position_center);
    delay(1000);*/
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
    delay(turning_time);
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
    digitalWrite(IN1,LOW);
    digitalWrite(IN2,HIGH);
    digitalWrite(IN3,HIGH);
    digitalWrite(IN4,LOW);
    delay(turning_time);    // Espera hasta que gire los 90°
  }
  else if(dir == "left")
  {
    digitalWrite(IN1,HIGH);
    digitalWrite(IN2,LOW);
    digitalWrite(IN3,LOW);
    digitalWrite(IN4,HIGH);
    delay(turning_time);
  }
  else if(dir == "stop")
  {
    digitalWrite(IN1,LOW);
    digitalWrite(IN2,LOW);
    digitalWrite(IN3,LOW);
    digitalWrite(IN4,LOW);
  }
else if(dir == "rotate")
  {
    digitalWrite(IN1,LOW);
    digitalWrite(IN2,LOW);
    digitalWrite(IN3,LOW);
    digitalWrite(IN4,LOW);
    delay(turning_time * 2);
  }


  analogWrite(ENA,m1);
  analogWrite(ENB,m2);
}

// Function that detects whether the left or right path is the longest one
int getLongestWay()             
{
  int left_distance;
  int right_distance;
  int aux;
  motor(STOP,0,0);
  delay(200);

  right_distance = getDistance(servo_position_right, ultra_main_trig, ultra_main_echo);
  left_distance = getDistance(servo_position_left, ultra_main_trig, ultra_main_echo);

  servo_main.write(servo_position_center);
  delay(500);
  
  if(right_distance > left_distance)
  {
    motor(RIGHT,150,150);

    /*aux = ultra(ultra_secondary_trig,ultra_secondary_echo);
    delay(20);
      motor(RIGHT,150,150);

  While(aux < right_distance)
    {
      motor(RIGHT,150,150);
      aux = ultra(ultra_secondary_trig,ultra_secondary_echo);
      delay(20);
    }*/
    return right_distance;
  }
  else
  {
    motor(LEFT,150,150);
    
        /*aux = ultra(ultra_secondary_trig,ultra_secondary_echo);
    delay(20);
    motor(LEFT,150,150);

    while(aux < left_distance)
    {
      motor(LEFT,150,150);
      aux = ultra(ultra_secondary_trig,ultra_secondary_echo);
      delay(20);
    }*/
    return left_distance;
  }
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
