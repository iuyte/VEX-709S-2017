#include "main.h"

Gyro gyro;
Ultrasonic fsonar;
int leftLine=1;
int rightLine=2;


void initializeIO() {
  pinMode(1, 0);
  digitalWrite(1, 0);
  pinMode(2, 0);
  digitalWrite(2, 0);
  pinMode(3, 0);
  digitalWrite(3, 0);
}


 void initialize() {
   analogCalibrate(leftLine);
   analogCalibrate(rightLine);
   fsonar = ultrasonicInit(5, 4);
   gyro = gyroInit(4,196);
   analogCalibrate(8);
   lcdInit(uart1);
   lcdSetBacklight(uart1, true);
   lcdClear(uart1);
   setTeamName("RoboEagles");
 }
