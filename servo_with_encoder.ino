#include <Servo.h>

#define encoderCLK 4
#define encoderDT 5
#define encoderBUTTON 12
int buttonState = HIGH;

float counter = 0;
int state;
int lastState;

Servo servomotor;
//byte servo_position = 0;

void setup() {
  Serial.begin(9600);
  pinMode(encoderCLK, INPUT);
  pinMode(encoderDT, INPUT);
  pinMode (encoderBUTTON, INPUT_PULLUP);
  
  lastState = digitalRead(encoderCLK);

  servomotor.attach(9);
  //servomotor.write(servo_position);
}

void loop() {
  
  buttonState = digitalRead(encoderBUTTON);    // Read the digital value of the switch (LOW/HIGH)

  // If the switch is pressed (LOW), print message
  if (buttonState == LOW) {
      Serial.println("Switch pressed");
      counter=0;
  }
  
  state = digitalRead(encoderCLK);
  if(state != lastState){
    if(digitalRead(encoderDT) != state){
      counter +=4;
      if(counter>180){
        counter=0;
      }
    }
    else{
      counter -=4;
      if(counter<0){
        counter=0;
      }
    }
    servomotor.write(counter);
    Serial.println(counter);
  }
  lastState = state;
}
