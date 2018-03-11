/*  Author: Obaid Khan
 *  Facebook: facebook.com/gatsantech
 *  Website: www.gatsan.com
 *  Email: projects@gatsan.com
 *  Mobile: 03038170020
 */


#include<Servo.h>

#define LEFT 46
#define RIGHT 47

Servo rightServo, leftServo;
bool objectPickedUp = false, junction = false;
int spd = 100;

void setup() {
  Serial3.begin(115200);  //GSM A6 
  releaseObject();
  pinMode(44, INPUT);

  setupMotors();
  setupArm();
  setupLineSensors();


  delay(10000);
  Serial3.print("AT+CMGF=1\r\n");
  delay(1000);
  Serial3.print("AT+CNMI=1,2,0,0,0\r\n");
  delay(500);

}

void loop() {
  if (noObject() || objectPickedUp)
  {
    followLine();
  }
  else
  {
    stop();
    if (!objectPickedUp)
    {
      forward();
      delay(500);
      pickupObject();
    }
  }
}

void setupMotors() 
{
  pinMode(8, OUTPUT);
  pinMode(9, OUTPUT);
  pinMode(10, OUTPUT);
  pinMode(11, OUTPUT);
}


void setupArm()
{
  leftServo.attach(6);
  rightServo.attach(7);
}

void setupLineSensors()
{
  pinMode(LEFT, INPUT);
  pinMode(RIGHT, INPUT);
}

void followLine()
{
  if (digitalRead(LEFT) == HIGH && digitalRead(RIGHT) == HIGH)
  {
    stop();
    if (junction)
      releaseObject();
    byte s = followNewLine();
    if (s == 1)
    {
      goLeft();
    }
    else if (s == 2)
    {
      goMid();
    }
    else if (s == 3)
    {
      goRight();
    }
    else
    {
      stop();
    }
    
    junction = true;

    Serial.println("JUNCTION");

  }



  if (digitalRead(LEFT) == LOW && digitalRead(RIGHT) == LOW)\\white and white
  {
    forward();
  }

  if ((digitalRead(LEFT) == LOW && digitalRead(RIGHT) == HIGH))
  {
    left();
  }

  if (digitalRead(LEFT) == HIGH && digitalRead(RIGHT) == LOW)
  {
    right();
  }

}

bool noObject()
{

  if (digitalRead(44) == LOW)
    return false;
  else
    return true;
}


int followNewLine()
{
  Serial.println("FNL is called");
  while (Serial3.available())
  {
    char c = (char)Serial3.read();
    if (c == '#')
    {
      delay(20);
      char c1 = (char)Serial3.read();

      if (c1 == '1')
      {
        return 1;

      }
      else if (c1 == '2')
      {
        return 2;
      }
      else if (c1 == '3')
      {
        return 3;
      }
      else
      {
        followNewLine();
      }
    }
  }
  followNewLine();
}

void goLeft()
{
  left();
  delay(500);
  forward();
  delay(250);
}

void goMid()
{
  forward();
  delay(250);
}

void goRight()
{
  right();
  delay(500);
  forward();
  delay(250);
}


void pickupObject()
{
  leftServo.write(20);
  delay(50);
  rightServo.write(140);
  delay(50);
  objectPickedUp = true;
}

void releaseObject()
{
  leftServo.write(10);
  delay(50);
  rightServo.write(170);
  delay(50);
  backward();
  delay(500)
  objectPickedUp = false;
}

void right()
{
  analogWrite(8, spd);
  analogWrite(10, spd);
  analogWrite(9, 0);
  analogWrite(11, 0);
}
void left()
{
  analogWrite(9, spd);
  analogWrite(11, spd);
  analogWrite(8, 0);
  analogWrite(10, 0);
}
void forward()
{
  analogWrite(9, spd);
  analogWrite(10, spd);
  analogWrite(8, 0);
  analogWrite(11, 0);
}
void backward()
{
  analogWrite(8, spd);
  analogWrite(11, spd);
  analogWrite(9, 0);
  analogWrite(10, 0);
}

void stop()
{
  analogWrite(8, 0);
  analogWrite(9, 0);
  analogWrite(10, 0);
  analogWrite(11, 0);

}
