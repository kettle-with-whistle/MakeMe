#include <TinyWireM.h>
#include <USI_TWI_Master.h>
#include <MMA865xM.h>
#include <Maths.h>
#include <avr/sleep.h>
#include <avr/power.h>
#include <avr/interrupt.h>

#define ledON LOW
#define ledOFF HIGH

#define ledR 8
#define ledG 7
#define ledB 5

#define RED 0
#define GREEN 1
#define BLUE 2
#define YELLOW 3
#define MAGENTA 4
#define CYAN 5
#define WHITE 6
#define OFF 7

//only used in debug
#define PRINT(x) 
#define PRINTLN(x) 
  
// Mode 1 parameters
#define samplingPeriod 20    //in ms
#define numIBI  5
#define sensitivity 40
unsigned long lastsampleTime = 0;

int BPM;                   // used to hold the pulse rate
int shakeAvg = 0;    //use calibration values from acc later. 
//int baseline = 0;  
unsigned long lightON = 0;
boolean beatDetected = false;
//int statCollected = 0;
unsigned long sampleCounter = 0; 
unsigned long lastBeatTime = 0;
//int P = baseline + sensitivity;                      // used to find peak in pulse wave
//int T = baseline - sensitivity;                     // used to find trough in pulse wave
int P = sensitivity;    
int T = -sensitivity;
int thresh = 0;   

int IBI = 300;             // holds the time between beats, the Inter-Beat Interval
boolean QS = false;        // becomes true when Arduoino finds a beat.
//int amp = 100;
//#define minST  150    

uint8_t numBeats = 0;

//Mode 2 parameters
int totalScore = 0;
uint8_t xyzState = 0;
boolean tempMode = false;
int numRead = 0;

// Both Modes
//float lastX=0, lastY=0, lastZ=0;

float accDX=0, accDY=0, accDZ=0;
float accVX=0, accVY=0, accVZ=0;
float accAX=0, accAY=0, accAZ=0;

// Button
#define buttonPin 9
boolean buttonState = LOW;         // variable for reading the pushbutton status
boolean wakeupflag = false;
unsigned long lastActiveTime;
unsigned long buttonDebounce;

void setup() {                
  // initialize the digital pin as an output.
  //for (led=0; led<11; led++) {
  pinMode(ledR, OUTPUT); 

  pinMode(ledG, OUTPUT); 

  pinMode(ledB, OUTPUT); 

  // Initialize button pin
  pinMode(buttonPin, INPUT); 

  //Opening sequence LEDs)
  digitalWrite(ledR, ledOFF);   // turn the LED off by making the voltage HIGH  
  digitalWrite(ledG, ledOFF);
  digitalWrite(ledB, ledOFF);
  LEDblip(WHITE);
  delay(200);
  LEDblip(WHITE);
  delay(500);
  
  uint8_t result = MMA865xM.begin();
  
  if (!result){
    blinkTimes(ledR, 1);
    blinkTimes(ledR, 1);
    blinkTimes(ledR, 1);
  }


  tempMode = false;
  
  setupInterrupts();
  setupADC();
  //don't do anything for 5 seconds, when they finish building the MakeMe Cube.
  delay(5000);  
  lastActiveTime = millis();
  buttonDebounce = millis();
}


// the loop routine runs over and over again forever:
void loop() {
  buttonState = digitalRead(buttonPin);
  // check if the pushbutton is pressed. If it is, the buttonState is HIGH:
  if (wakeupflag) {
    LEDblip(WHITE);
    delay(200);
    LEDblip(WHITE);
    delay(500);
    wakeupflag = false;
  }
  
  if (buttonState == HIGH) 
  { 
    if (!tempMode && (millis() - buttonDebounce)>3000) {
      //delay(1000);
      setBaseTemp();
      tempMode = TRUE;
      buttonDebounce = millis();
    }
    if (tempMode && (millis() - buttonDebounce)>3000) {
      buttonDebounce = millis();
      ledSetColour(OFF);
      tempMode = FALSE;
    }
      
    
  }
  else { //when button is low
    //if (xyzMode == TRUE && xyzState < 2) {
      //xyzMode = FALSE;
    //}
  }

  ////////////////Mode 1 - gesture mode

  if (tempMode) {
    /*
    if ((millis() - lastsampleTime) >= samplingPeriod){
      xyzCheckState();
    }
    */
    TemperatureLEDs();
    if ((millis() - lastsampleTime) >= 60000){
      tempMode = FALSE;
    }
  }

  ///////////////Mode 2 - shaking mode

  if (!tempMode) {
    if ((millis() - lastsampleTime) >= samplingPeriod){ 
      
      //boolean result = xyzSimple();
      //if (!result) 
          shakeMode();
      //xyzCheckState();
    }
  }

  if (millis() - lastActiveTime > 60000) {
    LEDblip(WHITE);
    enterSleep();
  }

}















