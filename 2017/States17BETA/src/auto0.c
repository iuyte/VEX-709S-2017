#include "ethanlib.h"
#include "constants.h"

void auto0() {
  gyroReset(gyro);
  encoderReset(lencoder);
  encoderReset(rencoder);
  jerk();// This is intended to drop and lock the intake
  delay(400);// waits 440 milliseconds for the intake to drop (AND LOCK)
  //GO FORWARD & PICK UP 1-3 STARS (+ intake), BACK UP TO PREVIOUS POSITION
  driveInchNoFix(44, 127); // Drives 28 inches forward at 63 power
  arr[0] = 0;
  arr[1] = potHalf;
  TaskHandle liftToTask1 = taskCreate(liftToTask, TASK_DEFAULT_STACK_SIZE, (void *)arr, TASK_PRIORITY_DEFAULT);
  driveInch(6, 127);
  mutexTake(potMutex, 5);
  int j = analogReadCalibrated(pot);
  mutexGive(potMutex);
  while(j < potHalf / 1.5) {
    mutexTake(potMutex, 5);
    j = analogReadCalibrated(pot);
    mutexGive(potMutex);
    delay(1);
  }
  driveInch(-38, 127);  // Drives 24/27 inches backward at 63 power
  //TURN BACK OF ROBOT TO WALL< LIFT UP, HIT WALL, DUMP
  turn(90, 70);        // Turns 83 degrees to the right at 40 power
  driveSet(-127, -127); // Sets the drive to go backwards at 70 power
  timerReset(0);
  arr[0] = 600;
  arr[1] = potTop + 50;
  TaskHandle liftToTask2 = taskCreate(liftToTask, TASK_DEFAULT_STACK_SIZE, (void *)arr, TASK_PRIORITY_DEFAULT);
  TaskHandle gyroResetIn = taskCreate(gyroResetAfter, TASK_DEFAULT_STACK_SIZE, (void *)1000, TASK_PRIORITY_DEFAULT);
  while (digitalRead(isWall) == 1 && digitalRead(isWall2) == 1 &&
         timer(0) < 4000) { // Repeats until the button on the robot is pressed
                            // or 4 seconds pass
    delay(1);
  }
  driveSet(0, 0); // Stops the robot
  delay(500);     // Waits half of a second
  //LIFT TO BOTTOM, TURN & DRIVE FORWARD (GET CUBE) LIFT UP
  arr[0] = 0;
  arr[1] = potBottom; // Takes the lift down to the bottom
  TaskHandle liiftToTask3 = taskCreate(liftToTask, TASK_DEFAULT_STACK_SIZE,
                                       (void *)arr, TASK_PRIORITY_DEFAULT);
  delay(1500);
  liftSet(-50);
  delay(250);
  liftSet(0);
  turnTo(-60, 70); // Turns left 43/61/55 degrees at 40 power
  driveInchNoFix(36, 127);
  arr[0] = 500;
  arr[1] = potHalf + 200;
  TaskHandle liftToTask4 = taskCreate(liftToTask, TASK_DEFAULT_STACK_SIZE,
                                      (void *)arr, TASK_PRIORITY_DEFAULT);
  driveInch(6, 127);
  //
  delay(250);
  driveInch(-12, 127);
  turn(-15, 70);
  driveSet(-127, -127);
  timerReset(0);
  while (digitalRead(isWall) == 1 && digitalRead(isWall2) == 1 &&
         timer(0) < 2000) { // Repeats until the bumper sensor on the robot is
                            // pressed or 3 seconds pass
    delay(1);
  }
  driveStop();
  driveInch(12, 127);
  turn(90, 70);
  delay(333);
  arr[1] = potTop; // Brings the lift to the top
  TaskHandle liftToTask5 = taskCreate(liftToTask, TASK_DEFAULT_STACK_SIZE,
                                      (void *)arr, TASK_PRIORITY_DEFAULT);
  timerReset(0);
  while (digitalRead(isWall) == 1 && digitalRead(isWall2) == 1 &&
         timer(0) < 2000) { // Repeats until the bumper sensor on the robot is
                            // pressed or 3 seconds pass
    delay(1);
  }
  driveStop();
  gyroReset(gyro);
  encoderReset(lencoder);
  encoderReset(rencoder);
}
