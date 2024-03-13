#include <Adafruit_NeoPixel.h>
#include <AccelStepper.h>

//LEDs
#define LED_COUNT 30
#define LED_PIN    6

//First pump Led change
int inProgressLed1 = 0;
int inProgressLed2 = 0;
int inProgressLed3 = 0;


Adafruit_NeoPixel strip(LED_COUNT, LED_PIN, NEO_GRB + NEO_KHZ800);

// PUMP A

const int enableA = 3; //pwm pin
const int in1 = 4;
const int in2 = 5;

// PUMP B

const int enableB = 9; //pwm pin
const int in3 = 8;
const int in4 = 7;

//STEPPER
#define STEPPER_PIN_1 11
#define STEPPER_PIN_2 10
#define STEPPER_PIN_3 12
#define STEPPER_PIN_4 13

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

boolean pumpStartDelay1 = false;
boolean pumpStartDelay2 = false;

boolean inProgressLedForPump1Off = false;

//MILLIS
unsigned long previousMillis = 0;  // Variable to store the last time the command was executed
const unsigned long delayTime = 3000;  // Time to delay in milliseconds
const unsigned long delayTime2 = 6000;  // Time to delay in milliseconds
const unsigned long pumpDelayTime1 = 1000; //Start after 1 second and ends after 3 sec: 2 sec work time
const unsigned long pumpDelayTime2 = 4000; //Start after 4 seconds and ends after 6 sec: 2 sec work time


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

  //PUMP B
  pinMode(enableB, OUTPUT);
  pinMode(in3, OUTPUT);
  pinMode(in4, OUTPUT);
  
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
  analogWrite (enableB, 255); //Voltage regulation of motorB

   switch(buttonState1)
{
case LOW:
    switch(inProgressLed1)
    {
      case 1:
      led_on(0,3,255,255,0);
      break;
      
      case 2:
      led_on(0,3,255,0,255);
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

      case 2:
      led_on(3,6,255,0,255);
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

      case 2:
      led_on(6,9,255,0,255);
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

    //First pump
    if (currentMillis - previousMillis >= pumpDelayTime1 && pumpStartDelay1 == true){
       //Pump start
        digitalWrite(in1, HIGH);
        digitalWrite(in2, LOW);

        //Leds change 
        // have to check value of n incremented by 1 because of n++ at the and of the loop that is always added
        if (n==2){
        inProgressLed1 = 1;
        }
        if (n==3){
        inProgressLed2 = 1;
        }
        if (n==4){
        inProgressLed3 = 1;
        }
        pumpStartDelay1 = false;
        inProgressLedForPump1Off = true;
    }
    
    //Second pump
    if (currentMillis - previousMillis >= pumpDelayTime2 && pumpStartDelay2 == true){
       //Pump start
        digitalWrite(in3, HIGH);
        digitalWrite(in4, LOW);

        //Leds change 
        // have to check value of n incremented by 1 because of n++ at the and of the loop that is always added
        if (n==2){
        inProgressLed1 = 2;
        }
        if (n==3){
        inProgressLed2 = 2;
        }
        if (n==4){
        inProgressLed3 = 2;
        }
        pumpStartDelay2 = false;
    }

    //Pump 1 Stop
    if (currentMillis - previousMillis >= delayTime){
    //Pump 1 stop
    digitalWrite(in1, LOW);
    digitalWrite(in2, LOW);
    
    if (inProgressLedForPump1Off == true){
    //Leds reset, it should work only once 
    inProgressLed1 = 0;
    inProgressLed2 = 0;
    inProgressLed3 = 0;
    inProgressLedForPump1Off = false;
      }
    }

    //Pump 2 Stop and continuation of program
    if (currentMillis - previousMillis >= delayTime2){
    //Pump 2 stop
    digitalWrite(in3, LOW);
    digitalWrite(in4, LOW);
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
        pumpStartDelay1 = true;
        pumpStartDelay2 = true;
       

        
        
      }
      if (n==2 && buttonState2 == LOW){    
        stepper.stop();
        previousMillis = currentMillis; 
        
        pumpStartDelay1 = true;
        pumpStartDelay2 = true;
      }
      if (n==3 && buttonState3 == LOW){
        stepper.stop();
        previousMillis = currentMillis;
         
        pumpStartDelay1 = true;
        pumpStartDelay2 = true;
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
