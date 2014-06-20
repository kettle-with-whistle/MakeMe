// Part of this code is extracted from the accelerometer code, only change the pulse signal to accelerometer amplitude
// Part of this code is extracted from the accelerometer code, only change the pulse signal to accelerometer amplitude
int rate[numIBI]; 
boolean Pulse = false;    // true when pulse wave is high, false when it's low      

//
boolean processAcc(float Signal){                         // triggered when Timer2 counts to 124
    
   // sampleCounter += samplingPeriod;                         // keep track of the time in mS with this variable
    sampleCounter = millis();
    int N = sampleCounter - lastBeatTime;       // monitor the time since the last beat to avoid noise
    
    //  find the peak and trough of the pulse wave
    /*
    if(Signal < thresh && N > (IBI/5)*3){       // avoid dichrotic noise by waiting 3/5 of last IBI
        if (Signal < T){                        // T is the trough
            T = Signal;                         // keep track of lowest point in pulse wave 
         }
       }
      
    if(Signal > thresh && Signal > P && N > (IBI/5)*3){          // thresh condition helps avoid noise
        P = Signal;  // P is the peak     
       }                                        // keep track of highest point in pulse wave
    */
  //  NOW IT'S TIME TO LOOK FOR THE HEART BEAT
  // signal surges up in value every time there is a pulse
 
  if (N > 150){                                 // avoid high frequency noise // Nobody can shake faster than this rate, let's assume. 
    if ( (Signal > (P-thresh)/2) && (Pulse == false) ){ //&& (N > (IBI/5)*3) ){        
      Pulse = true;          // set the Pulse flag when we think there is a pulse
      IBI = (sampleCounter - lastBeatTime);       // measure time between beats in mS. 
      lastBeatTime = sampleCounter;             // keep track of time for next pulse    
    
      if (numBeats < numIBI) {
          rate[numBeats] = IBI; 
          numBeats++;
          return false; 
        }
      else {
        // keep a running total of the last 10 IBI values
        word runningTotal = 0;                    // clear the runningTotal variable    
    
        for(uint8_t i=1; i<numIBI; i++){                  // shift data in the rate array
              //rate[i] = rate[i+1];                // and drop the oldest IBI value 
              runningTotal += rate[i];            // add up the 9 oldest IBI values
            }
            
        //rate[numIBI-1] = IBI;                            // add the latest IBI to the rate array
        //runningTotal += rate[numIBI-1];                  // add the latest IBI to runningTotal
        runningTotal /= numIBI;                       // average the last 10 IBI values 
        BPM = runningTotal;      //BPM is actually the interval between periods in our case                           
      // QS FLAG IS NOT CLEARED INSIDE THIS ISR
        return true;
      }  
    }
      
  }
  
  
  

  if (Signal < thresh && Pulse == true ){     // when the values are going down, the beat is over
      Pulse = false;                         // reset the Pulse flag so we can do it again
      //int amp = P - T;                           // get amplitude of the pulse wave
      //thresh = amp/2 + T;                    // set thresh at 50% of the amplitude
      //P = thresh;                            // reset these for next time
      //T = thresh;
  }      
  
  if (N > 3000){                             // if 2.5 seconds go by without a beat
      resetParameters();
  }

  
  return false;
}

void resetParameters() {
     /*
      if (longreset) {
        lastBeatTime = millis();    // bring the lastBeatTime up to date  
        ledSetColour(OFF);
      }
     */
    lastBeatTime = millis();    // bring the lastBeatTime up to date  
        ledSetColour(OFF); 
      Pulse = false;
      shakeAvg = 0;
      beatDetected = false;
      //P = baseline + sensitivity;                              // set P default
      //T = baseline - sensitivity;                               // set T default
      //P = 50;
      //T = -50;
      //thresh = 0;                          // set thresh default
      numBeats = 0;
      BPM = 0;
      lightON = 0;
      
     
}

