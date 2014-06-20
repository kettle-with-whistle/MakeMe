// Part of this code is extracted from the accelerometer code, only change the pulse signal to accelerometer amplitude
// Part of this code is extracted from the accelerometer code, only change the pulse signal to accelerometer amplitude
int rate[numIBI]; 
boolean Pulse = false;    // true when pulse wave is high, false when it's low      

//
boolean processAcc(float Signal){                         // triggered when Timer2 counts to 124
    
    sampleCounter = millis();
    int N = sampleCounter - lastBeatTime;       // monitor the time since the last beat to avoid noise
    
  //  NOW IT'S TIME TO LOOK FOR THE CYCLES OF THE SHAKING MOTIONS (or Pulse)
  
  //  find the peak and trough of the pulse wave
  /*
    if(Signal < thresh && N > (IBI/5)*3){       // avoid dichrotic noise by waiting 3/5 of last IBI
        if (Signal < T){                        // T is the trough
            T = Signal;                         // keep track of lowest point in pulse wave 
         }
       }
      
    if(Signal > thresh && Signal > P && N > (IBI/5)*3){          // thresh condition helps avoid noise
        P = Signal;  // P is the peak     
       }    
  */ 
       
  if (N >= 180)  {                                 // avoid high frequency noise // Nobody can shake faster than this rate, let's assume. 
    if ( (Signal > (P/2) && (Pulse == false) ) && (N > (IBI/5)*3) ){ 
      Pulse = true;          // set the Pulse flag when we think there is a pulse
      IBI = N;       // measure time between beats in mS. 
      lastBeatTime = sampleCounter;             // keep track of time for next pulse    
      
      if (numBeats <= 1) {
        numBeats++;    //don't want the first 2 IBI
        return false;
      }
      if (numBeats < numIBI) {
            rate[numBeats] = IBI; 
            //numBeats++;
            //LEDblip(BLUE);
            return false; 
          
      }
      else {
        // keep a running total of the last 10 IBI values
        word runningTotal = 0;                    // clear the runningTotal variable    
    
        for(uint8_t i=1; i<numIBI; i++){                  // shift data in the rate array
              //rate[i] = rate[i+1];                // and drop the oldest IBI value 
              runningTotal += rate[i];            // add up the 9 oldest IBI values
            }
            
        runningTotal /= numIBI;                       // average the last 10 IBI values 
        BPM = runningTotal;      //BPM is actually the interval between periods in our case                           
        return true;
      }  
    }
      
  }

  if (Signal < T/2 && Pulse == true && N > IBI/4){     // when the values are going down, the beat is over
      Pulse = false;                         // reset the Pulse flag so we can do it again
      //thresh = (P - T)/2 + T;                    // set thresh at 50% of the amplitude
      //P = thresh+sensitivity;                            // reset these for next time
      //T = thresh-sensitivity;
      numBeats++;
      //LEDblip(GREEN);
  }           
  
  //so that the P and T won't be stuck with a large peak or trough set by accident.
  
  if (N > 3000){                             // if 2.5 seconds go by without a beat
      resetParameters();
      //LEDblip(RED);
      //P = sensitivity;                              // set P default
      //T = -sensitivity;                               // set T default
      //thresh = 0;    
      //numBeats = 0; //count again 
  }
  
  
  
  return false;
}

void resetParameters() {
      lastBeatTime = millis();    // bring the lastBeatTime up to date  
      ledSetColour(OFF); 
      Pulse = false;
      shakeAvg = 0;
      beatDetected = false;
      //P = sensitivity;                              // set P default
      //T = -sensitivity;                               // set T default
      //thresh = 0;     
      
      //thresh = 0;                          // set thresh default
      numBeats = 0;
      BPM = 0;
      lightON = 0;   
     //LEDblip(RED);
}

