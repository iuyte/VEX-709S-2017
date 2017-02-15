#include "ethanlib.h"
#include "constants.h"

void auto2() {
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
}
