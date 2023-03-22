#include <AccelStepper.h>
#include <Adafruit_NeoPixel.h>
#include <Servo.h>

#define LED_COUNT 30
#define LED_PIN    3

Servo servomotor;
byte servo_position = 0;

Adafruit_NeoPixel strip(LED_COUNT, LED_PIN, NEO_GRB + NEO_KHZ800);

// Define stepper motor pins
#define STEPPER_PIN_1 8
#define STEPPER_PIN_2 9
#define STEPPER_PIN_3 10
#define STEPPER_PIN_4 11

// Define the number of steps per revolution for the stepper motor
const int STEPS_PER_REVOLUTION = 2048;

// Define the pin that the button is connected to
const int BUTTON_PIN1 = 19;
const int BUTTON_PIN2 = 18;
const int BUTTON_PIN3 = 17;
const int BUTTON_PIN4 = 16;
// Define the maximum motor speed and acceleration
const float MAX_SPEED = 200.0;    // steps per second
const float ACCELERATION = 400.0; 

boolean changestate = 0;


// Initialize the AccelStepper object with the appropriate pins, steps per revolution, motor speed, and acceleration
AccelStepper stepper(AccelStepper::FULL4WIRE, STEPPER_PIN_1, STEPPER_PIN_2, STEPPER_PIN_3, STEPPER_PIN_4);


void setup() {
  
  //LEDs
  strip.begin();
  strip.show();
  strip.setBrightness(50);
  
  //BUTTONS
  pinMode(BUTTON_PIN1, INPUT_PULLUP);
  pinMode(BUTTON_PIN2, INPUT_PULLUP);
  pinMode(BUTTON_PIN3, INPUT_PULLUP);
  pinMode(BUTTON_PIN4, INPUT_PULLUP);
  
  //STEPPERMOTOR
  stepper.setMaxSpeed(MAX_SPEED);
  stepper.setAcceleration(ACCELERATION);
  
  //SERVOMOTOR
  servomotor.attach(7);
  servomotor.write(servo_position);
}

void loop() {
   action();
}

void action() {
  
  int buttonState1 = digitalRead(BUTTON_PIN1);
  int buttonState2 = digitalRead(BUTTON_PIN2);
  int buttonState3 = digitalRead(BUTTON_PIN3);
  int buttonState4 = digitalRead(BUTTON_PIN4);
  
  switch(buttonState1)
{
case LOW:
    led_on(0,6,0);
    break;
  default:
  led_off(0,6);
}

switch(buttonState2)
{
case LOW:
    led_on(6,12,0);
    break;
  default:
  led_off(6,12);
}

switch(buttonState3)
{
case LOW:
    led_on(12,18,0);
    break;
  default:
  led_off(12,18);
}

  
  if (buttonState4 == LOW) {
    delay(200);
    //changestate = !changestate;
    changestate = 1;
    stepper.setCurrentPosition(0);
  }

  
  if (changestate == 1) {
    stepper.moveTo(500);
    stepper.setSpeed(MAX_SPEED);
    stepper.run();

  
    if (buttonState1 == LOW && stepper.distanceToGo() == 0) {
 
      stepper.stop();
      led_change(0,6);
      delay(2000);
      servomotor.write(90);
      delay(200);
      servomotor.write(0);
      
    }

    
    stepper.moveTo(1000);
    stepper.setSpeed(MAX_SPEED);
    stepper.run();

     
    if (buttonState2 == LOW && stepper.distanceToGo() == 0) {
      stepper.stop();
      led_change(6,12);
      delay(2000);
      servomotor.write(90);
      delay(200);
      servomotor.write(0);
    }

    
    stepper.moveTo(1500);
    stepper.setSpeed(MAX_SPEED);
    stepper.run();

    
    if (buttonState3 == LOW && stepper.distanceToGo() == 0){
      stepper.stop();
      led_change(12,18);
      delay(2000); 
    }

    
    stepper.moveTo(2048);
    stepper.setSpeed(MAX_SPEED);
    stepper.run();

    
    if (stepper.distanceToGo() == 0){
      stepper.stop();
      delay(2000);
      changestate = 0;
    }
  }
}


void led_on (int x, int y, int z) {
  for(int i=x; i<y; i++) {
    strip.setPixelColor(i, 255, 0, z);
    }
    strip.show();
}

void led_off (int x, int y) {
  for(int i=x; i<y; i++) {
    strip.setPixelColor(i, 0, 0, 0);
    }
    strip.show();
}

void led_change (int x, int y) {
  for(int i=x; i<y; i++) {
    strip.setPixelColor(i, 0, 255, 255);
    }
    strip.show();
}
