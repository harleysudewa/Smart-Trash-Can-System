#include <ESP32Servo.h>
#include <Wire.h>

#define SLAVE_ADDR 8
#define LIMIT_SWITCH_1 14  
#define LIMIT_SWITCH_2 27  
#define LIMIT_SWITCH_3 23  
#define SERVO1 13           
#define SERVO2 5           
#define SERVO3 12           
#define BATTERY_1 34
#define BATTERY_2 4
#define BATTERY_3 35
#define ECHOPIN 25
#define TRIGPIN 26
#define SOUND_VELOCITY 0.034

Servo servo1;
Servo servo2;
Servo servo3;
long duration;
float distanceCm;
int batteryPercentage1;
int batteryPercentage2;
int batteryPercentage3;
int potValue1;
int potValue2;
int potValue3;
bool state1;
bool state2;
bool state3; 
int trashCan1;
int trashCan2;
int trashCan3;
int trashCanFull;
int passwordCorrect;

void setup() {
  pinMode(LIMIT_SWITCH_1, INPUT_PULLUP);  
  pinMode(LIMIT_SWITCH_2, INPUT_PULLUP);
  pinMode(LIMIT_SWITCH_3, INPUT_PULLUP);

  pinMode(TRIGPIN, OUTPUT);
  pinMode(ECHOPIN, INPUT); 

  servo1.attach(SERVO1);
  servo1.write(60);
  servo2.attach(SERVO2);
  servo2.write(0);
  servo3.attach(SERVO3);
  servo3.write(60);
  Serial.begin(115200);
  
  Wire.begin(SLAVE_ADDR);
  Wire.onReceive(receiveEvent);
  Wire.onRequest(requestEvent);
  
}

void loop() {

  digitalWrite(TRIGPIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIGPIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIGPIN, LOW);
  duration = pulseIn(ECHOPIN, HIGH);
  distanceCm = duration * SOUND_VELOCITY/2;

  potValue1 = analogRead(BATTERY_1);  
  batteryPercentage1 = map(potValue1, 0, 4095, 0, 100);
  potValue2 = analogRead(BATTERY_2);  
  batteryPercentage2 = map(potValue2, 0, 4095, 0, 100); 
  potValue3 = analogRead(BATTERY_3);  
  batteryPercentage3 = map(potValue3, 0, 4095, 0, 100); 

  state1 = digitalRead(LIMIT_SWITCH_1);
  state2 = digitalRead(LIMIT_SWITCH_2);
  state3 = digitalRead(LIMIT_SWITCH_3); 

  /*Serial.print("State 1: ");
  Serial.print(state1);
  Serial.print("\nState 2: ");
  Serial.print(state2);
  Serial.print("\nState 3: ");
  Serial.println(state3);
  Serial.print("\nDistance (cm): ");
  Serial.println(distanceCm);
  Serial.print("\nBattery 1: ");
  Serial.print(batteryPercentage1);
  Serial.print("\nBattery 2: ");
  Serial.print(batteryPercentage2);
  Serial.print("\nBattery 3: ");
  Serial.println(batteryPercentage3);
  Serial.println(trashCan1);
  Serial.println(trashCan2);
  Serial.println(trashCan3);*/

  if (distanceCm < 10){
    if (state1 == HIGH && state2 == HIGH && state3 == HIGH) {
      if (batteryPercentage1 > 25) { 
        servo1.write(180);
        trashCan1 = 1;
      } else if (batteryPercentage1 < 25 && batteryPercentage2 > 25) { 
        servo2.write(70); 
        trashCan2 = 5;
      } else if (batteryPercentage1 < 25 && batteryPercentage2 < 25 && batteryPercentage3 > 25) { 
        servo3.write(180);
        trashCan3 = 6;
      } else if (batteryPercentage1 < 25 && batteryPercentage2 < 25 && batteryPercentage3 < 25) { 
        trashCanFull = 7;
      } 
    } else if (state1 == LOW && state2 == HIGH && state3 == HIGH) {
      if (batteryPercentage2 > 25) {  
        servo2.write(70);
        trashCan2 = 2;
      } else if (batteryPercentage2 < 25 && batteryPercentage3 > 25) { 
        servo3.write(180);
        trashCan3 = 6;
      } else if (batteryPercentage2 < 25 && batteryPercentage3 < 25) { 
        trashCanFull = 7;
      } 
    } else if (state1 == LOW && state2 == LOW && state3 == HIGH) {
      if (batteryPercentage3 > 25) {
        servo3.write(180);
        trashCan3 = 3;
      } else if (batteryPercentage3 < 25) {
        trashCanFull = 7;
      } 
    } else if (state1 == LOW && state2 == LOW && state3 == LOW) {
      trashCanFull = 7;
    }
  } else if (passwordCorrect == 10) {
    servo1.write(180);
    servo2.write(70);
    servo3.write(180);
    delay(15000);
    passwordCorrect = 0;
  } else {
    delay(500);
    servo1.write(60);
    servo2.write(0);
    servo3.write(60);
    trashCan1 = 0;
    trashCan2 = 0;
    trashCan3 = 0;
    trashCanFull = 0;
  }

  delay(1000);

}

// Fungsi untuk menerima data dari Master
void receiveEvent(int howMany) {
  while (0 < Wire.available()) {
    passwordCorrect = Wire.read();
  }
}

void requestEvent() {
  // Fungsi ini akan dipanggil saat master melakukan permintaan
  if (trashCan1 != 0) {
    Wire.write(trashCan1);
  } else if (trashCan2 != 0) {
    Wire.write(trashCan2);
  } else if (trashCan3 != 0) {
    Wire.write(trashCan3);
  } else if (trashCanFull != 0) {
    Wire.write(trashCanFull);
  } 
}