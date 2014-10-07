int ledPin = 9;
int brightness = 0;
int sensorLow = 1020;
int sensorHigh = 1023;

void setup(){
  Serial.begin(9600);
  pinMode(ledPin, OUTPUT);
}

void loop(){
  int sensorValue = analogRead(A0);
  Serial.println(sensorValue);
  brightness = map(sensorValue, sensorLow, sensorHigh, 0, 255);
  Serial.println(brightness);
  analogWrite(ledPin, brightness);
  delay(100);
}
