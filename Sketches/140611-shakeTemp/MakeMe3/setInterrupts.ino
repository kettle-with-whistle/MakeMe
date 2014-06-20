
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
  
  numRead = 0;
  tempMode = false;
  lastActiveTime = millis();
  resetParameters();
  buttonDebounce = millis();

}

void setupADC(){

  //ADC Multiplexer Selection Register
  ADMUX = 0;
  ADMUX |= (1 << REFS1);  //Internal 1.1V Voltage Reference with external capacitor on AREF pin
  ADMUX |= (0 << REFS0);  //Internal 1.1V Voltage Reference with external capacitor on AREF pin
  ADMUX |= (1 << MUX5);  //Temperature Sensor - 100010
  ADMUX |= (0 << MUX4);  //Temperature Sensor - 100010
  ADMUX |= (0 << MUX3);  //Temperature Sensor - 100010
  ADMUX |= (0 << MUX2);  //Temperature Sensor - 100010
  ADMUX |= (1 << MUX1);  //Temperature Sensor - 100010
  ADMUX |= (0 << MUX0);  //Temperature Sensor - 100010
  //ADMUX = 0b1000010;
  //ADC Control and Status Register A 
  ADCSRA = 0;
  ADCSRA |= (1 << ADEN);  //Enable the ADC
  ADCSRA |= (1 << ADPS1);  //ADC Prescaler - 8 (8MHz -> 1MHz)

  //ADC Control and Status Register B 
  ADCSRB = 0;
  ADCSRB |= (1 << MUX5);  //Temperature Sensor - 100111
}

