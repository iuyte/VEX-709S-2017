#include "FunctionVars.h"

void autonomous() {
  driveInch(-4, 127);//Drives 4 inches backward at 90 power
  driveInch(4, 127);//Drives 4 inches forward at 90 power
  driveInch(27, 127);//Drives 25.5/28.5 inches forward at 63 power
  liftToTaskPos = potHalf;
  liftToTaskWait = 0;
  TaskHandle liftToTaskk = taskCreate(liftToTask, TASK_DEFAULT_STACK_SIZE, NULL, TASK_PRIORITY_DEFAULT);
  driveInch(4, 127);
  delay(400);
  driveInch(-28, 127);//Drives 24/27 inches backward at 63 power
  turn(77, 100);//Turns 83 degrees to the right at 40 power
  driveSet(-110, -127);//Sets the drive to go backwards at 70 power
  timerReset(0);
  while (digitalRead(isWall) == 1 && timer(0) < 4000) {//Repeats until the button on the robot is pressed or 4 seconds pass
    delay(1);
    if (timer(0) == 1000) {
      liftToTaskPos = potTop;//Brings the lift to the top
      TaskHandle lliftToTaskk = taskCreate(liftToTask, TASK_DEFAULT_STACK_SIZE, NULL, TASK_PRIORITY_DEFAULT);
    }
  }
  driveSet(0, 0);//Stops the robot
  delay(75);//Waits half of a second
  liftToTaskPos = potBottom;//Takes the lift down to the bottom
  TaskHandle lliiftToTaskk = taskCreate(liftToTask, TASK_DEFAULT_STACK_SIZE, NULL, TASK_PRIORITY_DEFAULT);
  turn(-56, 100);//Turns left 43/61/55 degrees at 40 power
  driveInch(30, 127);
  liftToTaskPos = potHalf;
  TaskHandle lliftTToTaskk = taskCreate(liftToTask, TASK_DEFAULT_STACK_SIZE, NULL, TASK_PRIORITY_DEFAULT);
  driveInch(6, 127);
  driveSet(-127, -127);
  timerReset(0);
  while (digitalRead(isWall) == 1 && digitalRead(isWall2) == 1 && timer(0) < 2000) {//Repeats until the bumper sensor on the robot is pressed or 3 seconds pass
    delay(1);
  }
  driveSet(0, 0);//Stops the drive
  driveSet(127, 127);
  delay(200);
  driveSet(127, -127);
  delay(250);
  liftToTaskPos = potTop;//Brings the lift to the top
  TaskHandle lliftTToTaaskk = taskCreate(liftToTask, TASK_DEFAULT_STACK_SIZE, NULL, TASK_PRIORITY_DEFAULT);
  driveSet(0, 0);
  gyroShutdown(gyro);
  encoderShutdown(lencoder);
  encoderShutdown(rencoder);
}
