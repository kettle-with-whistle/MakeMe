float baseTemp = 0;

void TemperatureLEDs() {
  uint16_t r, b, g;
  //int t;
  float t;

  int TBLUE = baseTemp;
  int TRED = baseTemp + 5;
  int TRANGE = TRED - TBLUE;
  
  for (int i=0;i<100; i++) 
      t = (i*t + getTemp())/(i+1);
      
    // Choose the next point in RGB space by temperature - this is a 
    // linear scale that maps from pure blue for the coldest to pure
    // red for the hottest, with intermediate temperatures
    // represented by the appropriate mixture of blue and red.
    //
    // We constrain the temperature so that the lowest temperature we
    // represent is TBLUE and the highest is TRED. MAX_BRIGHTNESS is
    // the maximum we can set an LED channel to.
    //
    r = (constrain(t - TBLUE, 0, TRANGE) * 255) / TRANGE;
    b = (constrain(TRED - t,  0, TRANGE) * 255) / TRANGE;
    
    // Now set the colour of the all the LEDs appropriately.
    analogWrite(ledB, 255-b);
    analogWrite(ledR, 255-r);
}

int getTemp(){
  ADCSRA |= (1 << ADSC);  //Start temperature conversion
  while (bit_is_set(ADCSRA, ADSC));  //Wait for conversion to finish
  byte low  = ADCL;
  byte high = ADCH;
  int temperature = (high << 8) | low;  //Result is in kelvin
  return temperature - 273;
}

void setBaseTemp(){
  for (int i=0;i<500; i++) 
      baseTemp = (i*baseTemp + getTemp())/(i+1);  
}
