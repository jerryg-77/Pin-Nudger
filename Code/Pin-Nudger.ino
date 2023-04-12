// Testing USB joystick function using Arduino Leonardo
// using MPU6050 as analog x, y and z axis input
// Code written by Pin Chung  
// Sep 22, 2020

#include <Joystick.h>
#include <MPU6050_tockn.h>
#include <Wire.h>

MPU6050 mpu6050(Wire);
//Joystick_ Joystick;

Joystick_ Joystick(JOYSTICK_DEFAULT_REPORT_ID, 
  JOYSTICK_TYPE_GAMEPAD, 0, 0,
  true, true, false, 
  false, false, false,
  false, false, 
  false, false, false);

#define ResetPin 13
#define Joystick1Pin 9

int lastButtonState=0;
int currentButtonState;

float zeroX=0;
float zeroY=0;
float zeroZ=0;

const float XNUDGE_FORCE = .4;
const float YNUDGE_FORCE = .4;

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

void ResetMPU6050(float x, float y, float z){
  //Center the Joystick at current position
  zeroX=x;
  zeroY=y;

}

void loop() 
{
  mpu6050.update();
   float x=(mpu6050.getAccX());
   float y=(mpu6050.getAccY());

  
  x-=zeroX;
  y-=zeroY;
  
/*   if (x>upperLimit) x=upperLimit;
   else if (x<lowerLimit) x=lowerLimit;
   if (y>upperLimit) y=upperLimit;
   else if (y<lowerLimit) y=lowerLimit;
*/


 // Map the angles of x, y and z to joystick position 0 to 1024
 // to reverse the axis, switch the last 2 numbers in the map() function
 if (x < XNUDGE_FORCE && x > (-1* XNUDGE_FORCE)){
    x = 0;
 }
  if (y < YNUDGE_FORCE && y > (-1 * YNUDGE_FORCE)) {
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
