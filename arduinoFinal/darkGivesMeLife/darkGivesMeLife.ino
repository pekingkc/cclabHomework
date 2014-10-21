#include <Servo.h> 

Servo myservo;

int speaker = 9;
int ledPin1 = 6;
int ledPin2 = 5;
//int photoSensor = A0;
//int pressSensor = A1;

void setup(){
  Serial.begin(9600);
  myservo.attach(3);
  pinMode(A0, INPUT);
  pinMode(A1, INPUT);
  pinMode(ledPin1, OUTPUT);
  pinMode(ledPin2, OUTPUT);
  pinMode(speaker, OUTPUT);
}

void loop(){
  noTone;
  int sensorValue = analogRead(A0);
  int sensorValue2 = analogRead(A1);
//  Serial.println(sensorValue);
//  Serial.println(sensorValue2);

  delay(10);
  
  //turn off the light
  if(sensorValue < 50){
//    myservo.write(180);
    int thisPitch = map(sensorValue, 0, 100, 1000, 1500);
    tone(speaker, thisPitch, 800);
    for(int fadeValue = 0; fadeValue <= 255; fadeValue +=1){
      int i = 0;
      myservo.write(i++);
      analogWrite(ledPin1, fadeValue);
//      delay(3);
    }
    for(int fadeValue = 255; fadeValue >= 0; fadeValue -=1){
      int i = 90;
      myservo.write(i--);
      analogWrite(ledPin1, fadeValue);
//      delay(3);
    }
  }
  
  //press
  if(sensorValue2 > 200){
    int powerValue = map(sensorValue2, 400, 900, 1000, 2000);
    tone(speaker, powerValue, 800);
  }
  
}
