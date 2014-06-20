// Acc interrupt PCINT0 interrupt service routine, called when a rising/falling edge is seen on the source line.
/*
ISR(PCINT0_vect)
{
	delay(5);
	GIFR = 0xFF; // Clear interrupt flag.
        //ledBlink(ledB, 300);
}
*/

// PCINT1, button interrupt interrupt service routine, called when a rising/falling edge is seen on the source line.
ISR(PCINT1_vect)
{
        cli(); // Disable interrupts
	delay(5);
	GIFR = 0xFF; // Clear interrupt flag.
        if (wakeupflag)
            wakeUp();
        sei();  // Enable interrupts
}

void enableWakeUp(void)
{
	GIMSK  = 0x30;	// Enable Acc and Button interrupt wake up.
      
}

void disableWakeUp(void)
{
	GIMSK  = 0x00;	// Disable Acc and Button interrupt wake up.
}

void setupInterrupts(void)
{
    //Now set up the interrupt
  cli(); // Disable interrupts
  disableWakeUp();
  //PCMSK0 = 0x01; // PCINT0 Accelerometer interrupt
  PCMSK1 = 0x02; // PCINT9 Button interrupt

  sei();  // Enable interrupts
}

// Description: Enters the arduino into sleep mode.
void enterSleep(void)
{
	enableWakeUp();
  
	set_sleep_mode(SLEEP_MODE_PWR_DOWN);
	sleep_enable();
	sleep_mode();
  
	/* The program will continue from here. */
  
	/* First thing to do is disable sleep. */
	sleep_disable(); 
        wakeupflag = TRUE;
	//disableWakeUp();
}

void wakeUp() {
  /*
  digitalWrite(ledR, ledOFF);   // turn the LED off by making the voltage HIGH  
  digitalWrite(ledG, ledOFF);
  digitalWrite(ledB, ledOFF);
  blinkTimes(ledR, 1);
  blinkTimes(ledG, 1);
  blinkTimes(ledB, 1);
  */
  
  numRead = 0;
  xyzMode = false;
  //clickState = 0;
  lastActivity = millis();
  resetParameters();
  
  //delay(200);
  //LEDblip(WHITE);
}
