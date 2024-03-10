#include <Adafruit_NeoPixel.h>
#include <AccelStepper.h>

//LEDs
#define LED_COUNT 30
#define LED_PIN    6

int inProgressLed1 = 0;
int inProgressLed2 = 0;
int inProgressLed3 = 0;

Adafruit_NeoPixel strip(LED_COUNT, LED_PIN, NEO_GRB + NEO_KHZ800);

// PUMP A

const int enableA = 3; //pwm pin
const int in1 = 4;
const int in2 = 5;

//STEPPER
#define STEPPER_PIN_1 8
#define STEPPER_PIN_2 9
#define STEPPER_PIN_3 10
#define STEPPER_PIN_4 11

const int STEPS_PER_REVOLUTION = 2048;

const float MAX_SPEED = 300.0;    // steps per second
const float ACCELERATION = 100.0; 

//BUTTONS
#define START_BUTTON_PIN A5
#define BUTTON_PIN1 A0
#define BUTTON_PIN2 A1
#define BUTTON_PIN3 A2

// correct values below
const int fullCircle = 11580;
const int stepCircle = 1930;
// test values below
//const int fullCircle = 2316;
//const int stepCircle = 386;
int n = 1;

boolean changestate = 0;

//MILLIS
unsigned long previousMillis = 0;  // Variable to store the last time the command was executed
const unsigned long delayTime = 2000;  // Time to delay in milliseconds

// Initialize the AccelStepper object with the appropriate pins, steps per revolution, motor speed, and acceleration
AccelStepper stepper(AccelStepper::FULL4WIRE, STEPPER_PIN_1, STEPPER_PIN_2, STEPPER_PIN_3, STEPPER_PIN_4);

void setup() {
  
  //LEDs
  strip.begin();
  strip.show();
  strip.setBrightness(50);

  //PUMP A
  pinMode(enableA, OUTPUT);
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
  
  //BUTTONS
  pinMode(START_BUTTON_PIN, INPUT_PULLUP);
  pinMode(BUTTON_PIN1, INPUT_PULLUP);
  pinMode(BUTTON_PIN2, INPUT_PULLUP);
  pinMode(BUTTON_PIN3, INPUT_PULLUP);
  
  stepper.setMaxSpeed(MAX_SPEED);
  //stepper.setAcceleration(ACCELERATION);

}

void loop() {
  
  
  int buttonState1 = digitalRead(BUTTON_PIN1);
  int buttonState2 = digitalRead(BUTTON_PIN2);
  int buttonState3 = digitalRead(BUTTON_PIN3);

  analogWrite (enableA, 255); //Voltage regulation of motorA
  

   switch(buttonState1)
{
case LOW:
    switch(inProgressLed1)
    {
      case 1:
      led_on(0,3,255,255,0);
      break;
      default:
      led_on(0,3,0,255,0);
      break;
    }
    break;
  default:
  led_off(0,3);
  break;
}

switch(buttonState2)
{
case LOW:
    switch(inProgressLed2)
    {
      case 1:
      led_on(3,6,255,255,0);
      break;
      default:
      led_on(3,6,0,255,0);
      break;
    }
    break;
  default:
  led_off(3,6);
  break;
}

switch(buttonState3)
{
case LOW:
    switch(inProgressLed3)
    {
      case 1:
      led_on(6,9,255,255,0);
      break;
      default:
      led_on(6,9,0,255,0);
      break;
    }
    break;
  default:
  led_off(6,9);
  break;
}
  
  if (int buttonState4 = digitalRead(START_BUTTON_PIN) == LOW) {
    delay(200);
    changestate = 1;
    n=1;
    stepper.setCurrentPosition(0);
  }
  
  if (changestate == 1) {
    stepper.moveTo(fullCircle);
    stepper.setSpeed(MAX_SPEED);
    unsigned long currentMillis = millis();
    if (currentMillis - previousMillis >= delayTime){
    //Pump stop
    digitalWrite(in1, LOW);
    digitalWrite(in2, LOW);
    //Leds reset
    inProgressLed1 = 0;
    inProgressLed2 = 0;
    inProgressLed3 = 0;
    stepper.run();
    }
    if (stepper.distanceToGo() == fullCircle - n*stepCircle){
      if (n==1 && buttonState1 == LOW){
        stepper.stop();   
        previousMillis = currentMillis; 
        //delay(2000);
        
        //Pump start
        digitalWrite(in1, HIGH);
        digitalWrite(in2, LOW);

        //Leds change 
        inProgressLed1 = 1;

        
        
      }
      if (n==2 && buttonState2 == LOW){    
        stepper.stop();
        previousMillis = currentMillis; 
        //delay(2000);

        //Pump start
        digitalWrite(in1, HIGH);
        digitalWrite(in2, LOW);

         //Leds change 
        inProgressLed2 = 1;
      }
      if (n==3 && buttonState3 == LOW){
        stepper.stop();
        previousMillis = currentMillis; 
        //delay(2000);

        //Pump start
        digitalWrite(in1, HIGH);
        digitalWrite(in2, LOW);

         //Leds change 
        inProgressLed3 = 1;
      }
      n++;  
    }
  
  if (fullCircle - n*stepCircle == 0){
      stepper.stop();
      //delay(2000);
      changestate = 0;
       
    }
  }
}

//LED function

void led_on (int x, int y, int r, int g, int b) {
  for(int i=x; i<y; i++) {
    strip.setPixelColor(i, r, g, b);
    }
    strip.show();
}

void led_off (int x, int y) {
  for(int i=x; i<y; i++) {
    strip.setPixelColor(i, 255, 0, 0);
    }
    strip.show();
}
