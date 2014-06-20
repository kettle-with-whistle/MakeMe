
void sampleAcc() {
  
  float xyz[3];
  //float x, y, z;
  float Signal=0.0f;
  int resultacc=0; 
  MMA865xM.xyz(xyz);
  //EngduinoAccelerometer.xyz(xyz);
  /*
  x = xyz[0];
  y = xyz[1];
  z = xyz[2];
  */
  //resultacc = sqrt((x*x + y*y + z*z)/3); 
  Signal =   xyz[0] + xyz[1] + xyz[2] - lastX - lastY - lastZ;
  // here we normalise the reading to LED density
  
  //check what's the range is in driver
  resultacc = (int)( Signal * 255 / 2);   //4G range = 255
  
  shakeAvg = 0.9 * shakeAvg + 0.1 * resultacc;
  
  boolean beat = processAcc(shakeAvg);

  if (beat) {
    stepLight (BPM);  
    //EngduinoLEDs.setAll(OFF);
    resetParameters(); 
  }
  
  if (Signal > 1) {
    lastActivity = millis();
  }
    
  lastX = xyz[0];
  lastY = xyz[1];
  lastZ = xyz[2];
}
