// USB joystick function using Arduino Leonardo for nudging
// using MPU6050 as analog x, y for pinball nudging
// Code contributions include Code written by Pin Chung  
// Code Adapted by Jerry Gilley

// Joystick.h library from MHeironimus https://github.com/MHeironimus/ArduinoJoystickLibrary
// MPU6050_tockn.h library from tockn https://github.com/Tockn/MPU6050_tockn

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

float zeroX=0;  //Start X at Zero
float zeroY=0;  //Start Y at Zero

const float XNUDGE_FORCE = .4;  //Define force on X axis to trigger above and below
const float YNUDGE_FORCE = .4;  //Define force on Y axis to trigger above and below

//upper and lower limit on the angle of the MPU6050 board range.
int upperLimit = 45;
int lowerLimit = -45;

void setup() {

  
  Serial.begin(9600);
  Joystick.begin();

  Wire.begin();
  mpu6050.begin();
  mpu6050.calcGyroOffsets(false); //This is is not a necessary call. works fine without it.

}


void loop() 
{
  mpu6050.update();
   float x=(mpu6050.getAccX());
   float y=(mpu6050.getAccY());
  
  x-=zeroX;
  y-=zeroY;

 // Map the angles of x, y and z to joystick position 0 to 1024
 // to reverse the axis, switch the last 2 numbers in the map() function
 if (x < XNUDGE_FORCE && x > (-1* XNUDGE_FORCE)){     // If force is between the XNUDGE_FORCE (-XNUDGE_FORCE<>XNUDGE_FORCE) Then make X zero
    x = 0;
 }
  if (y < YNUDGE_FORCE && y > (-1 * YNUDGE_FORCE)) {  // If force is between the XNUDGE_FORCE (-YNUDGE_FORCE<>YNUDGE_FORCE) Then make Y zero
    y = 0;
 }
   x = x *1000;
   y = y *1000;
   Joystick.setXAxis(map(x, lowerLimit,upperLimit, 1024,0));
   Joystick.setYAxis(map(y, lowerLimit,upperLimit, 0, 1024));
   if (x != 0) {
        Serial.print ("\n x=");
        Serial.print (x);
   }
   if (y != 0) {
        Serial.print(" y=");
        Serial.print (y);
   }


}
