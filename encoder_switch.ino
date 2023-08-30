#include <Servo.h>
#include <Adafruit_NeoPixel.h>

#define LED_COUNT 30
#define LED_PIN    11

Adafruit_NeoPixel strip(LED_COUNT, LED_PIN, NEO_GRB + NEO_KHZ800);

#define encoderCLK 4
#define encoderDT 5
#define encoderBUTTON 12
int buttonState = HIGH;

int counter = 0;
int state;
int lastState;

Servo servomotor;
//byte servo_position = 0;

bool conditionMet = false;

void setup() {
  Serial.begin(9600);

  strip.begin();
  strip.show();
  strip.setBrightness(50);
  
  pinMode(encoderCLK, INPUT);
  pinMode(encoderDT, INPUT);
  pinMode (encoderBUTTON, INPUT_PULLUP);
  
  lastState = digitalRead(encoderCLK);

  servomotor.attach(9);
  //servomotor.write(servo_position);

  if (!conditionMet) {
    Serial.println("fct 000");
    led_on(0,30,0);
    conditionMet = true; // Set the flag to indicate the condition has been met
  }
  
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
      counter +=2;
      if(counter>36){
        counter=0;
      }
    }
    else{
      counter -=2;
      if(counter<0){
        counter=36;
      }
    }
    servomotor.write(counter);
    Serial.println(counter);

    if (counter >= 0 && counter < 12) {
    Serial.println("fct 000");
    led_on(0,30,0);
    } 
    else if (counter >= 13 && counter < 24) {
    Serial.println("fct 111");
    led_on(0,30,255);
    }
    else if (counter >= 25 && counter < 36) {
    Serial.println("fct 222");
    led_on(0,30,100);
    }
  }
  lastState = state;
}

void led_on (int x, int y, int z) {
  for(int i=x; i<y; i++) {
    strip.setPixelColor(i, 255, 0, z);
    }
    strip.show();
}
