#include "FunctionVars.h"
#include "constants.h"

void autonomous() {
  gyroReset(gyro);
  encoderReset(lencoder);
  encoderReset(rencoder);
  driveInch(-4, 100);//Drives 4 inches backward at 90 power
  driveInch(4, 100);//Drives 4 inches forward at 90 power
  delay(400);
  driveInch(28, 127);//Drives 25.5/28.5 inches forward at 63 power
  liftToTaskPos = potHalf;
  liftToTaskWait = 0;
  TaskHandle liftToTask1 = taskCreate(liftToTask, TASK_DEFAULT_STACK_SIZE, NULL, TASK_PRIORITY_DEFAULT);
  driveInch(5, 127);
  delay(400);
  driveInch(-30, 127);//Drives 24/27 inches backward at 63 power
  turn(77, 100);//Turns 83 degrees to the right at 40 power
  driveSet(-110, -127);//Sets the drive to go backwards at 70 power
  timerReset(0);
  liftToTaskWait = 1000;
  liftToTaskPos = potTop + 50;
  TaskHandle liftToTask2 = taskCreate(liftToTask, TASK_DEFAULT_STACK_SIZE, NULL, TASK_PRIORITY_DEFAULT);
  while (digitalRead(isWall) == 1 && timer(0) < 4000) {//Repeats until the button on the robot is pressed or 4 seconds pass
    delay(1);
  }
  liftToTaskWait = 0;
  driveSet(0, 0);//Stops the robot
  delay(500);//Waits half of a second
  liftToTaskPos = potBottom;//Takes the lift down to the bottom
  TaskHandle liiftToTask3 = taskCreate(liftToTask, TASK_DEFAULT_STACK_SIZE, NULL, TASK_PRIORITY_DEFAULT);
  delay(1500);
  liftSet(-50);
  delay(250);
  liftSet(0);
  turn(-53, 100);//Turns left 43/61/55 degrees at 40 power
  driveInch(34, 127);
  liftToTaskPos = potHalf + 200;
  TaskHandle liftToTask4 = taskCreate(liftToTask, TASK_DEFAULT_STACK_SIZE, NULL, TASK_PRIORITY_DEFAULT);
  driveInch(6, 127);
  delay(250);
  driveSet(-127, -127);
  timerReset(0);
  while (digitalRead(isWall) == 1 && digitalRead(isWall2) == 1 && timer(0) < 2000) {//Repeats until the bumper sensor on the robot is pressed or 3 seconds pass
    delay(1);
  }
  driveSet(0, 0);//Stops the drive
  driveSet(127, 127);
  delay(333);
  driveSet(127, -127);
  delay(333);
  liftToTaskPos = potTop;//Brings the lift to the top
  TaskHandle liftToTask5 = taskCreate(liftToTask, TASK_DEFAULT_STACK_SIZE, NULL, TASK_PRIORITY_DEFAULT);
  driveSet(0, 0);
  gyroReset(gyro);
  encoderReset(lencoder);
  encoderReset(rencoder);
}
