
//float accVX = 0, accVY=0, accVZ=0;
float accDX=0, accDY=0, accDZ=0;


//int lightontime = 0;
//int totalScore = 0;
//int xyzState = 0;
unsigned long lastState;
uint8_t lastDir;

void xyzCheckState(){
  float xyz[3];
  //float x, y, z;
  float dx, dy, dz;
 
  MMA865xM.xyz(xyz);

  //x = xyz[0];
  //y = xyz[1];
  //z = xyz[2];
  //differentiate to take out the gravity component
  dx = xyz[0]-lastX;
  dy = xyz[1]-lastY;
  dz = xyz[2]-lastZ;
  
  numRead++;
  /*
  if (numRead ==1){
    lastX = x;
    lastY = y;
    lastZ = z;
    return;
  }  
  */
  if (numRead <=2) {
    lastX = xyz[0];
    lastY = xyz[1];
    lastZ = xyz[2];
    //accVX = dx*samplingPeriod;
    //accVY = dy*samplingPeriod;
    //accVZ = dz*samplingPeriod;
    return;
  }
  
  int dirThres = 50;
  // when moving
  if (abs(dx)>=0.1||abs(dy)>=0.1||abs(dz)>=0.1) {
    //integrate to get acceleration back
    if (numRead >=3 ){
      accDX += (dx)*samplingPeriod;  //in cm      
      accDY += (dy)*samplingPeriod;  //in cm
      accDZ += (dz)*samplingPeriod;  //in cm
    }

  }

  else { // it has stopped moving, calculate the movement
    int tem = 1;
    for (int i=0; i<xyzState; i++)
        tem*=10;
        
    if ((abs(accDX) > dirThres || abs(accDY) > dirThres || abs(accDZ) > dirThres) && lastState == 0) {
      if (abs(accDX) >= abs(accDY)  && abs(accDX) >= abs(accDZ)){
          //LEDblip(RED);
          totalScore += 1*tem;
          
      }
      
      else if (abs(accDY) >= abs(accDX)  && abs(accDY) >= abs(accDZ)){
          //LEDblip(GREEN);
          totalScore += 2*tem;
        
      }
      else if (abs(accDZ) >= abs(accDY)  && abs(accDZ) >= abs(accDX)){
          //LEDblip(BLUE);
          totalScore += 3*tem;
        
      }
      if (xyzState <2) {
          xyzState ++;
      }
      else {
        evaluateScore();
       
      }
      lastState = millis();    //register the time we find a state
      
    }
    
  
    accDX = 0;
    accDY = 0;
    accDZ = 0;
    //accVX = 0;
    //accVY = 0;
    //accVZ = 0;
    numRead = 0;
    //
    
  }

  if (millis() - lastState > 500) 
      lastState = 0;
      
  lastX = xyz[0];
  lastY = xyz[1];
  lastZ = xyz[2];
 
 // register the last movement for sleeping purpose
 if (dx + dy + dz > 1) {
   lastActivity = millis();
 }
 
}

void evaluateScore() {
  uint8_t co;
  
  switch (totalScore) {
    case 111:case 222:case 333:  //same-same-same
      co = RED;
      break;
    case 112: case 113 : case 221: case 223 :case 331:case 332:
      co = BLUE;  //same-different-different
      break;
    case 121:case 131:case 212:case 232:case 313:case 323:
      co = GREEN;  //same-different-same
      break;
    case 122:case 133:case 211:case 233:case 311:case 322:
      co = YELLOW;  //same-same-different
      break;
    default:
      co = MAGENTA; //different-different-different 
      break;
  }
  delay(500);
  /*
  for (uint8_t i=0; i<5; i++) {
    ledSetColour(co);
    delay(900);
    ledSetColour(OFF);
    delay(100);
  }
  */
  fading(co);
  xyzState = 0;
  totalScore = 0;
  lastState=0;
}
