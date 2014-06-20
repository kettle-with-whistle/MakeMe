#define movingThres  0.4
#define dirThres  5

//int movingThres = 0.4;
//int dirThres = 5;
//float dx, dy, dz;

//unsigned long lastStateTime;
//uint8_t noMove = 0;
//uint8_t ST = 0;

  
//#define DEBUG

/**************************/
/* xyzCheckState */
/*****************/
/*
void xyzCheckState()  {
  //float xyz[3];
  boolean validMove = false;

  //float accAX=0, accAY=0, accAZ=0;
  
   boolean moved = sampleAcc();
   
   ST = millis() - lastsampleTime;
   //High pass filter to take out gravity
   dx = 0.8 * dx + 0.2 * xyz[0];
   dy = 0.8 * dy + 0.2 * xyz[1];
   dz = 0.8 * dz + 0.2 * xyz[2];
   accAX = xyz[0] - dx;
   accAY = xyz[1] - dy;
   accAZ = xyz[2] - dz; 
   

  // when moving
   //delibrate movement rather than noise
  //if (abs(accAX)>=movingThres||abs(accAY)>=movingThres||abs(accAZ)>=movingThres) {
    if (abs(accAX) + abs(accAY) + abs(accAZ)>0.3) {
    
      if (numRead == 0) {
        accDX = 0;
        accDY = 0;
        accDZ = 0;
        accVX = 0;
        accVY = 0;
        accVZ = 0;
      }
      
      
      accVX += (accAX)*ST/1000;  //in m/s      
      accVY += (accAY)*ST/1000;  //in m/s
      accVZ += (accAZ)*ST/1000;  //in m/s
      
      //accumulative distance traveled
      accDX += abs(accVX)*ST/1000 * 100;  //in ??      
      accDY += abs(accVY)*ST/1000 * 100;  //in ??
      accDZ += abs(accVZ)*ST/1000 * 100;  //in ??
      numRead ++ ;
      noMove = 5;
      lastActivityTime = millis();  //update sleep parameter
      
  if (moved) {
      accDX += abs(accVX)*ST/10;  //in ??      
      accDY += abs(accVY)*ST/10;  //in ??
      accDZ += abs(accVZ)*ST/10;  //in ??
      numRead ++ ;
      noMove = 5;
  } //end if moving
  else  { 
    ///////if It has been moved to a certain distance but less than a certain distance to distinguish from shaking
    //if (abs(accDX) < 50 && abs(accDY) < 50 && abs(accDZ) < 50) { 
      if ((abs(accDX) > dirThres || abs(accDY) > dirThres || abs(accDZ) > dirThres) && lastStateTime == 0 && numRead > 10) { 
        // The base of the score based on the number of movements 
        int tem = 10^xyzState;
        //for (int i=0; i<xyzState; i++)
        //    tem*=10;
          
        if (abs(accDX) >= abs(accDY)  && abs(accDX) >= abs(accDZ)){

              totalScore += 1*tem;
        }
        
        else if (abs(accDY) >= abs(accDX)  && abs(accDY) >= abs(accDZ)){
            validMove = true;
            totalScore += 2*tem;
        }
        else {  //Z has moved
            validMove = true;
            totalScore += 3*tem;
        }
  
      
        //check if there is a valid move  
        if (validMove) {      
          if (xyzState <2) {
              xyzState ++;
          }
          else {
            evaluateScore();
          }
          accDX = 0;
          accDY = 0;
          accDZ = 0;
          //accVX = 0;
          //accVY = 0;
          //accVZ = 0;
          lastStateTime = millis();    //register the time we find a state
         } //end validMove 
      } ///end distance travelled    
    //} ///end distance travelled maximum  
      
      // This last bit execute everytime there is no movement
      if (millis() - lastStateTime > 500) 
          lastStateTime = 0;  
          
      if (noMove > 0)
          noMove -- ;
       else
          numRead = 0;  
          
    }  ////////// end No Movement else statement
    
   //////Other things that need to be done in each loop
  // register the last movement for sleeping purpose
  
   //if (dx + dy + dz > 1) {
   //  lastActivity = millis();
   //}
   
} //xyzCheckState
*/
/************************************************************/
/* To evaluate the gesture detected and produce the results */
/************************************************************/
/*
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
  //delay(500);
  fading(co);
  xyzState = 0;
  totalScore = 0;
  lastStateTime=0;
}
*/
boolean xyzSimple() {
  boolean moved = sampleAcc();
  boolean result = false;
  if (moved) {
      accDX += abs(accVX)*ST/10;  //in ??      
      accDY += abs(accVY)*ST/10;  //in ??
      accDZ += abs(accVZ)*ST/10;  //in ??
      numRead ++ ;
  } //end if moving
  
  else  {
      if ((abs(accDX) > dirThres || abs(accDY) > dirThres || abs(accDZ) > dirThres) && numRead > 10 && (abs(accDX) + abs(accDY) + abs(accDZ)) < 200) { 
        xyzState ++;
      }
      if (xyzState ==  2) {
        fading(BLUE);
        xyzState = 0;
        accDX = 0;
        accDY = 0;
        accDZ = 0;
        result = true;
      }
        
  }
  return result;
}
