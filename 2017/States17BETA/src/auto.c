#include "FunctionVars.h"
#include "constants.h"

void autonomous() {
  gyroReset(gyro);
  useIdeals[LIFT] = true;
  useIdeals[LEFT_DRIVE] = true;
  useIdeals[RIGHT_DRIVE] = true;
  encoderReset(lencoder);
  encoderReset(rencoder);
  driveInch(-4, 100);//Drives 4 inches backward
  driveInch(4, 100);//Drives 4 inches forward
  // The previous code is intended to drop the intake
  delay(400);
  driveInch(28, 127);//Drives 28 inches forward at 63 power
  systems[LIFT] = potHalf;
  driveInch(5, 127);
  delay(400);
  driveInch(-30, 127);//Drives 24/27 inches backward at 63 power
  turn(77, 100);//Turns 83 degrees to the right at 40 power
  driveSet(-110, -127);//Sets the drive to go backwards at 70 power
  useIdeals[LEFT_DRIVE] = false;
  useIdeals[RIGHT_DRIVE] = false;
  TaskHandle gyroResetIn = taskCreate(gyroResetAfter, TASK_DEFAULT_STACK_SIZE, (void*)1000, TASK_PRIORITY_DEFAULT) ;
  while (digitalRead(isWall) == 1 && digitalRead(isWall2) == 1 && timer(0) < 4000) {//Repeats until the button on the robot is pressed or 4 seconds pass
    delay(1);
  }
  driveSet(0, 0);//Stops the robot
  delay(500);//Waits half of a second
  systems[LIFT] = potBottom;//Takes the lift down to the bottom
  delay(1500);
  useIdeals[LIFT] = false;
  liftSet(-50);
  delay(250);
  liftSet(0);
  useIdeals[LIFT] = true;
  turnTo(-53, 100);//Turns left 43/61/55 degrees at 40 power
  driveInch(34, 127);
  systems[LIFT] = potHalf + 200;
  driveInch(6, 127);
  delay(250);
  driveInch(-12, 127);
  turn(-15, 100);
  driveSet(-127, -127);
  timerReset(0);
  while (digitalRead(isWall) == 1 && digitalRead(isWall2) == 1 && timer(0) < 2000) {//Repeats until the bumper sensor on the robot is pressed or 3 seconds pass
    delay(1);
  }
  driveStop();
  driveInch(12, 127);
  turn(77, 100);
  delay(333);
  systems[LIFT] = potTop;
  timerReset(0);
  while (digitalRead(isWall) == 1 && digitalRead(isWall2) == 1 && timer(0) < 2000) {//Repeats until the bumper sensor on the robot is pressed or 3 seconds pass
    delay(1);
  }
  driveStop();
  gyroReset(gyro);
  encoderReset(lencoder);
  encoderReset(rencoder);
}
