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

// Mode 1 parameters
#define samplingPeriod 20    //in ms
#define numIBI  6
//#define sensitivity 50
unsigned long lastsampleTime = 0;
//#define P0 80
//#define T0 -80

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
int P = 70;
int T = - 70;
int thresh = 0;   

int IBI = 500;             // holds the time between beats, the Inter-Beat Interval
boolean QS = false;        // becomes true when Arduoino finds a beat.
//int amp = 100;
//#define minST  150    

uint8_t numBeats = 0;

//Mode 2 parameters
int totalScore = 0;
uint8_t xyzState = 0;
boolean xyzMode = false;
int numRead = 0;

// Both Modes
float lastX=0, lastY=0, lastZ=0;

// Button
#define buttonPin 9
boolean buttonState = LOW;         // variable for reading the pushbutton status
//unsigned long buttonStarts;
//uint8_t clickState = 0;
boolean wakeupflag = false;
unsigned long lastActivity;

//boolean buttonHigh = false;
// the setup routine runs once when you press reset:
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
  //blinkTimes(ledR, 1);
  //blinkTimes(ledG, 1);
  //blinkTimes(ledB, 1);
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


  xyzMode = false;
  
  setupInterrupts();
  lastActivity = millis();
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
    xyzMode = TRUE;
    
    
  }
  else { //when button is low
    if (xyzMode == TRUE && xyzState < 2) {
      xyzMode = FALSE;
      
    }
  }
/*  
  if (buttonState == HIGH) 
  { 
    //buttonHigh = true;    
    LEDblip(BLUE);
    if (clickState==0) {
      buttonStarts = millis();
      clickState++;  //click state = 1
    }
    //touch twice
    else if (clickState == 2 && (millis() - buttonStarts)>100) {
       
      LEDblip(MAGENTA);
        xyzMode = TRUE;
        clickState = 0;
        //buttonHigh = false;
        delay(500);
    }
    // We are in x-y-z mode
    
  } 
  else {
    if (clickState == 1 && millis() - buttonStarts >100) {
      clickState ++;    //click state = 2
      buttonStarts = millis();
    }
    if (clickState == 2 && millis() - buttonStarts >2500) {
      clickState = 0;
      LEDblip(YELLOW); 
      xyzMode = FALSE;
      delay(500);
    //buttonHigh = false
    }
  }
  */
  //Mode 1
  if (xyzMode) {
    ledRGB(1,1,0);
    if ((millis() - lastsampleTime) >= samplingPeriod){
      xyzCheckState();
      lastsampleTime = millis();
    }
  }

  //Mode 2 

  if (!xyzMode) {
    //ledRGB(0,1,1);
    if ((millis() - lastsampleTime) >= samplingPeriod){ 
      sampleAcc();
      lastsampleTime = millis();
    }
  }

  if (millis() - lastActivity > 60000) {
    LEDblip(WHITE);
    enterSleep();
  }

}















