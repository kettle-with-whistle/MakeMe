#define L1 160
uint8_t colours[16] = {RED, GREEN, BLUE, YELLOW, MAGENTA, CYAN, WHITE, RED, GREEN, BLUE, YELLOW, MAGENTA, CYAN, WHITE, RED, GREEN};

void blinkTimes(uint8_t LEDcolour, uint8_t numTimes) {
  digitalWrite(LEDcolour, ledOFF);
  for (int i=0; i<numTimes; i++) {
    digitalWrite(LEDcolour, ledON);
    delay(800);
    digitalWrite(LEDcolour, ledOFF);
    delay(200);
  }
  
}

void ledSetColour(uint8_t c) {
  switch (c) {
        case RED:
	    digitalWrite(ledR, ledON);
            digitalWrite(ledG, ledOFF);
            digitalWrite(ledB, ledOFF);
	    break;
	case GREEN:
	    digitalWrite(ledR, ledOFF);
            digitalWrite(ledG, ledON);
            digitalWrite(ledB, ledOFF);
	    break;
	case BLUE:
	    digitalWrite(ledR, ledOFF);
            digitalWrite(ledG, ledOFF);
            digitalWrite(ledB, ledON);	
	    break;
	case YELLOW:
	    digitalWrite(ledR, ledON);
            digitalWrite(ledG, ledON);
            digitalWrite(ledB, ledOFF);	
            break;
        case MAGENTA:
	    digitalWrite(ledR, ledON);
            digitalWrite(ledG, ledOFF);
            digitalWrite(ledB, ledON);
	    break;
        case CYAN:
	    digitalWrite(ledR, ledOFF);
            digitalWrite(ledG, ledON);
            digitalWrite(ledB, ledON);
	    break;
	case WHITE:
	    digitalWrite(ledR, ledON);
            digitalWrite(ledG, ledON);
            digitalWrite(ledB, ledON);
	    break;
	case OFF:
            digitalWrite(ledR, ledOFF);
	    digitalWrite(ledG, ledOFF);
            digitalWrite(ledB, ledOFF);
	    break;
	}
}

void ledRGB(uint8_t r, uint8_t g, uint8_t b) {
  analogWrite(ledR, 255-r);
  analogWrite(ledG, 255-g);
  analogWrite(ledB, 255-b);
}

void crazyLight(){
  
  for (int i=0; i< 2; i++) {
       for (int j=0; j<16;j++) {
         ledSetColour(colours[j]);
         delay(50);
         ledSetColour(OFF);
         delay(100);
       }
       }
}

/*
void stepLight(int value) {
  //uint8_t stepsize = 40;
  if (value < L1) {
       crazyLight();
    }
    //RED
    else if ( value >= L1  && value < L1+50) { 
      //ledRGB(255, 0, 0);
      ledSetColour(RED);
    }
    //ORANGE
    else if ( value < L1+100) { 
      ledRGB(255, 50, 0);
    }
    //YELLOW
    else if ( value < L1+150) { 
      ledSetColour(YELLOW);
      
    }
    //GREEN
    else if ( value < L1+200) { 
      //ledRGB(0, 255, 0);
      ledSetColour(GREEN);
    }
    //CYAN
    else if ( value < L1+250) { 
      //ledRGB(0, 230 , 255);
      ledSetColour(CYAN);
    }
    //BLUE
    else if ( value < L1+300) { 
      //ledRGB(0, 0, 255);
      ledSetColour(BLUE);
    }
    //PURPLE    
    else if ( value >= L1+350  ) { // fast
      ledRGB(250, 0, 250);
      //ledSetColour(MAGENTA);
    }
    delay(3000);
    ledSetColour(OFF);
}
*/

void stepLight(int value) {
  //uint8_t stepsize = 40;
  if (value < L1) {
       crazyLight();
       return;
  }
  else if (value < L1+100) { 
    ledRGB(255, 50, 0); //ORANGE
  }
  else if ( value < L1+200) { 
    ledSetColour(YELLOW); 
  }
  else if ( value < L1+300) { 
   ledSetColour(GREEN); 
  }
  else {
   ledSetColour(CYAN); 
  }
  delay(3000);
  ledSetColour(OFF);
}


void LEDblip(uint8_t co) {
  ledSetColour(co);
  delay(50);
  ledSetColour(OFF);
}

void fading(uint8_t co) {
  uint8_t Led=1, Led1=0;
  
  digitalWrite(ledR, HIGH);  
  digitalWrite(ledG, HIGH);
  digitalWrite(ledB, HIGH);
  
  switch (co) {
    case RED: Led = ledR; break;
    case GREEN: Led = ledG; break;
    case BLUE: Led = ledB; break;
    case YELLOW: Led = ledR;Led1 = ledG; break;
    case MAGENTA: Led = ledB;Led1 = ledR; break;
  }
  
  for(int n=0; n<4; n++) {
    for (int i=0; i<256; i++) {
      analogWrite(Led, i);  // PWM the LED from 0 to 255 (max)
      if (Led1 >0)
         analogWrite(Led1, i);  // PWM the LED from 0 to 255 (max)
      delay(5);
    }
    for (int i=255; i>=0; i--) {
      analogWrite(Led, i);  // PWM the LED from 255 (max) to 0
      if (Led1 >0)
         analogWrite(Led1, i);  // PWM the LED from 0 to 255 (max)
      delay(5);
    }
  }
  delay(1000);
  digitalWrite(ledR, HIGH);  
  digitalWrite(ledG, HIGH);
  digitalWrite(ledB, HIGH);
}
