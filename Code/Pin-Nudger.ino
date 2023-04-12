// USB joystick function using Arduino Leonardo for nudging
// using MPU6050 as analog x, y for pinball nudging
// Code Adapted by Jerry Gilley

// ==============================Credits From Sources Used=================================================================
// Insperation from Pinscape http://mjrnet.org/pinscape/
// Joystick.h library from MHeironimus https://github.com/MHeironimus/ArduinoJoystickLibrary
// MPU6050_tockn.h library from tockn https://github.com/Tockn/MPU6050_tockn
// j2robin https://github.com/j2robin/PinballNudgeRelay
// Pin Chung  
//=========================================================================================================================

#include <Joystick.h>
#include <MPU6050_tockn.h>
#include <Wire.h>

MPU6050 mpu6050(Wire);

Joystick_ Joystick(JOYSTICK_DEFAULT_REPORT_ID, 
  JOYSTICK_TYPE_GAMEPAD, 0, 0,      // Define controller as Gamepad, zero buttons and zero hats
  true, true, false,                // Define X and Y axis, no Z axis
  false, false, false,              // No X, Y or Z rotational axis
  false, false,                     // No rudder or throttle
  false, false, false);             // No accelerator, brake or steeting

//Define some variables
float zeroX=0;  //Start X at Zero
float zeroY=0;  //Start Y at Zero

const float XNUDGE_FORCE = .4;  //Define force on X axis to trigger above and below
const float YNUDGE_FORCE = .4;  //Define force on Y axis to trigger above and below

//upper and lower limit on the acceleration translation from the MPU6050 board.
int upperLimit = 1023;
int lowerLimit = -1023;

unsigned long lastTime;
unsigned long currentTime;
unsigned int timeLimit = 10;


void setup() {
  
  Serial.begin(9600);
  Joystick.begin();
  Wire.begin();
  mpu6050.begin();
  mpu6050.calcGyroOffsets(false); //This is is not a necessary call. works fine without it.
  lastTime = 0;
  currentTime = 0;
}


void loop() 
{
  mpu6050.update();
   float x=(mpu6050.getAccX());
   float y=(mpu6050.getAccY());

  x-=zeroX;
  y-=zeroY;

 // Map the angles of x, y to joystick position 0 to 1024
 // to reverse the axis, switch the last 2 numbers in the map() function
 if (x < XNUDGE_FORCE && x > (-1* XNUDGE_FORCE)){     // If force is between the XNUDGE_FORCE (-XNUDGE_FORCE<>XNUDGE_FORCE) Then make X zero
    x = 0;
 }
  if (y < YNUDGE_FORCE && y > (-1 * YNUDGE_FORCE)) {  // If force is between the XNUDGE_FORCE (-YNUDGE_FORCE<>YNUDGE_FORCE) Then make Y zero
    y = 0;
 }
   
   x = x * 1000;  // convert accel to usable axis number
   y = y * 1000;  // convert accel to usable axis number

   //Joystick updates only sent ever 10 ms as this is as fast as VPX and USB can process changes
   //Keeps from overloading USB bus
   currentTime = millis();
   if (currentTime - lastTime > timeLimit) { //Send Update
      Joystick.setXAxis(map(x, lowerLimit,upperLimit, 1024,0));
      Joystick.setYAxis(map(y, lowerLimit,upperLimit, 0, 1024));
      lastTime = millis();
   }
   
   if (x != 0) { //Debug Code
        Serial.print ("\n x="); //Debug Code
        Serial.print (x); //Debug Code
   } //Debug Code
   if (y != 0) { //Debug Code
        Serial.print(" y="); //Debug Code
        Serial.print (y); //Debug Code
   } //Debug Code

}
