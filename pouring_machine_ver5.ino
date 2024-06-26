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
#define DIR_PIN 12
#define STEP_PIN 11
#define ENABLE 13

//const int STEPS_PER_REVOLUTION = 2048;

const float MAX_SPEED = 1200;    // steps per second
const float ACCELERATION = 500; 

//BUTTONS
#define START_BUTTON_PIN A5
#define BUTTON_PIN1 A0
#define BUTTON_PIN2 A1
#define BUTTON_PIN3 A2

int buttonState1 = 0;
int buttonState2 = 0;
int buttonState3 = 0;

//Hall sensor
#define HALL_SENSOR 2
int hallState;
int previousHallState = HIGH;

// correct values below
//const int fullCircle = 11580;
//const int stepCircle = 1930;
// test values below
//const int fullCircle = 2316;
//const int stepCircle = 386;


int n = 1;

boolean changestate = 0;
boolean changeDirection = 0;

boolean pumpStartDelay1 = false;
boolean pumpStartDelay2 = false;

boolean inProgressLedForPump1Off = false;

//MILLIS
unsigned long previousMillis = 0;  // Variable to store the last time the command was executed
const unsigned long delayTime = 3000;  // Time to delay in milliseconds
const unsigned long delayTime2 = 6000;  // Time to delay in milliseconds
const unsigned long pumpDelayTime1 = 1000; //Start after 1 second and ends after 3 sec: 2 sec work time
const unsigned long pumpDelayTime2 = 4000; //Start after 4 seconds and ends after 6 sec: 2 sec work time


// Define a stepper and the pins it will use
// 1 or AccelStepper::DRIVER means a stepper driver (with Step and Direction pins)
AccelStepper stepper(1, STEP_PIN, DIR_PIN); //type (1) of stepper motor not the FULL2WIRE- look up to AccelStepper library 

void setup() {
  
  //LEDs
  strip.begin();
  strip.show();

   //start animation
  led_on(0,6,255,255,255);
  delay(100);
  led_on(0,6,0,0,0);
  delay(200);
  led_on(0,6,255,255,255);
  delay(500);
  fade_in(0, 6);
  //fade_out(0, 6);
  
  //strip.setBrightness(50);

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

  //STEPPER
  stepper.setMaxSpeed(MAX_SPEED);
  stepper.setAcceleration(ACCELERATION);
  //pinMode(DIR_PIN, OUTPUT);
  //digitalWrite(DIR_PIN, HIGH);
  
  stepper.setEnablePin(ENABLE);
  
  //specify that the enable pin is inverted
  stepper.setPinsInverted(false, false, true);
  //stepper.enableOutputs();
  //pinMode(ENABLE, OUTPUT);
  //setEnablePin(ENABLE);

  //HALL SENSOR
  pinMode(HALL_SENSOR, INPUT);
  Serial.begin(9600);
  
  

}

void loop() {
  
  hallState = digitalRead(HALL_SENSOR);
  Serial.println(hallState);
  
  buttonState1 = digitalRead(BUTTON_PIN1);
  buttonState2 = digitalRead(BUTTON_PIN2);
  buttonState3 = digitalRead(BUTTON_PIN3);

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
    //stepper.moveTo(fullCircle);
    if(changeDirection == 0){
    stepper.setSpeed(MAX_SPEED);
    }
    else{
      stepper.setSpeed(-MAX_SPEED);
    }
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
    stepper.enableOutputs();
    stepper.run();
    }
    
    if (hallState != previousHallState){ // tylko jak zmieni sie z 1 na 0
      previousHallState = hallState;
     if (hallState == LOW) {
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
    }
  
  if (n == 7){
      stepper.stop();
      //delay(2000);
      changestate = 0;
      changeDirection = !changeDirection;
      stepper.disableOutputs();
       
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

void fade_in (int x, int y) {
  for(int j = 0; j<255; j++) {
    for(int i=x; i<y; i++) {
      strip.setPixelColor(i, j, 0, 0);
      }
    strip.show();
    delay(10);
  }
}


void fade_out (int x, int y) {
  for(int j = 255; j>0; j--) {
    for(int i=x; i<y; i++) {
      strip.setPixelColor(i, j, j, j);
      }
    strip.show();
    delay(5);
  }
}
