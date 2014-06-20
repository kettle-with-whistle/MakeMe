float vx, vy, vz;

float dx, dy, dz;
uint8_t ST = 0;

float xyz[3];

//*********************//
boolean sampleAcc() { 
  boolean moved = false;

  MMA865xM.xyz(xyz);
  //apply a very week high pass filter to accelerometer
  dx = 0.99 * dx + 0.01 * xyz[0];
  dy = 0.99 * dy + 0.01 * xyz[1];
  dz = 0.99 * dz + 0.01 * xyz[2];
  //low pass
  accAX = xyz[0] - dx;
  accAY = xyz[1] - dy;
  accAZ = xyz[2] - dz; 


  //calculate the velocity
  accVX += (accAX)*ST/10;  //in m/s      
  accVY += (accAY)*ST/10;  //in m/s
  accVZ += (accAZ)*ST/10;  //in m/s
  //Take away the dc component of the velocity (high pass)
  vx = 0.9 * vx + 0.1 * accVX;
  vy = 0.9 * vy + 0.1 * accVY;
  vz = 0.9 * vz + 0.1 * accVZ;
  accVX = accVX - vx;
  accVY = accVY - vy;
  accVZ = accVZ - vz;
      
  //update the sample time
  ST = millis() - lastsampleTime;
  lastsampleTime = millis();
  
  //determine if moving
  if (abs(accVX) + abs(accVY) + abs(accVZ)>0.5) {  
      //accumulative distance traveled
      lastActiveTime = millis();  //update sleep parameter
      moved = true;
      }
   else  { 
     // reset velocity and distance to prevent drifting
       accVX = 0;
       accVY = 0;
       accVZ = 0; 
        
       moved = false;
        //lastState = millis();    //register the time we find a state
   } 
   return moved;
      
}

//*********************//
// This smoothing (high pass) routine for accelerometer is undirectional. We intend to get the "wave" of shaking
void shakeMode(){
  boolean moved = sampleAcc();
  if (moved) {
    
      shakeAvg = (accVX + accVY + accVZ);
    
      boolean beat = processAcc(shakeAvg);
    
      if (beat) {
        stepLight (BPM);  
        resetParameters(); 
      }
  }
   else  { 
      resetParameters();
   } 
  
}
