#include "constants.h"
#include "ethanlib.h"

void auto0() {
  gyroReset(gyro);
  encoderReset(lencoder);
  encoderReset(rencoder);
  jerk();// This is intended to drop and lock the intake
  delay(400);// waits 440 milliseconds for the intake to drop (AND LOCK)
  //GO FORWARD & PICK UP 1-3 STARS (+ intake), BACK UP TO PREVIOUS POSITION
  driveInchNoFix(16, 127); // Drives 24
  arr[0] = 0;
  arr[1] = POTHALF;
  liftToHandle = taskCreate(liftToTask, TASK_DEFAULT_STACK_SIZE, (void *)arr, TASK_PRIORITY_DEFAULT);
  driveInch(13, 127); // 6
  mutexTake(potMutex, -1);
  int j = analogReadCalibrated(POT);
  mutexGive(potMutex);
  while(j < POTHALF / 1.5) {
    mutexTake(potMutex, 5);
    j = analogReadCalibrated(POT);
    mutexGive(potMutex);
    delay(1);
  }
  driveInch(-12, 127);  // Drives 38 inches backward at 63 power
  //TURN BACK OF ROBOT TO WALL< LIFT UP, HIT WALL, DUMP
  //turnNoFix(2, 127);        // Turns 83 degrees to the right at 40 power
  //turn(88, 55);
  smartTurn(90, 100);
  driveSet(-127, -127); // Sets the drive to go backwards at 70 power
  timerReset(0);
  arr[0] = 600; // 300
  arr[1] = POTTOP + 50;
  liftToHandle = taskCreate(liftToTask, TASK_DEFAULT_STACK_SIZE, (void *)arr, TASK_PRIORITY_DEFAULT);
  TaskHandle gyroResetIn = taskCreate(gyroResetAfter, TASK_DEFAULT_STACK_SIZE, (void *)1000, TASK_PRIORITY_DEFAULT);
  while (digitalRead(isWall) == 1 && digitalRead(isWall2) == 1 &&
         timer(0) < 4000) { // Repeats until the button on the robot is pressed
                            // or 4 seconds pass
    delay(1);
  }
  driveSet(0, 0); // Stops the robot

}
