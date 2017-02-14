#include "constants.h"
#include "ethanlib.h"

void auto5() {
  jerk();
}

void auto4() {
  for (size_t k = 0; k < 6; k++) {
    int mot = driveMotorList[k];
    motorSet(mot, 127);
    wait(500);
    motorStop(mot);
  }
}

void auto3() {
  gyroReset(gyro);
  encoderReset(lencoder);
  encoderReset(rencoder);
  jerk();
  // The previous code is intended to drop the intake
  delay(400);
  driveInch(28, 127); // Drives 28 inches forward at 63 power
  arr[0] = 0;
  arr[1] = potHalf;
  TaskHandle liftToTask1 = taskCreate(liftToTask, TASK_DEFAULT_STACK_SIZE,
                                      (void *)arr, TASK_PRIORITY_DEFAULT);
  driveInch(5, 127);
  delay(400);
  driveInch(-30, 127);  // Drives 24/27 inches backward at 63 power
  turn(77, 100);        // Turns 83 degrees to the right at 40 power
  driveSet(-110, -127); // Sets the drive to go backwards at 70 power
  timerReset(0);
  arr[0] = 1000;
  arr[1] = potTop + 50;
  TaskHandle liftToTask2 = taskCreate(liftToTask, TASK_DEFAULT_STACK_SIZE,
                                      (void *)arr, TASK_PRIORITY_DEFAULT);
  TaskHandle gyroResetIn = taskCreate(gyroResetAfter, TASK_DEFAULT_STACK_SIZE,
                                      (void *)1000, TASK_PRIORITY_DEFAULT);
  while (digitalRead(isWall) == 1 && digitalRead(isWall2) == 1 &&
         timer(0) < 4000) { // Repeats until the button on the robot is pressed
                            // or 4 seconds pass
    delay(1);
  }
  driveSet(0, 0); // Stops the robot
  delay(500);     // Waits half of a second
  arr[0] = 0;
  arr[1] = potBottom; // Takes the lift down to the bottom
  TaskHandle liiftToTask3 = taskCreate(liftToTask, TASK_DEFAULT_STACK_SIZE,
                                       (void *)arr, TASK_PRIORITY_DEFAULT);
  delay(1500);
  liftSet(-50);
  delay(250);
  liftSet(0);
  turnTo(-53, 100); // Turns left 43/61/55 degrees at 40 power
  driveInch(34, 127);
  arr[1] = potHalf + 200;
  TaskHandle liftToTask4 = taskCreate(liftToTask, TASK_DEFAULT_STACK_SIZE,
                                      (void *)arr, TASK_PRIORITY_DEFAULT);
  driveInch(6, 127);
  delay(250);
  driveInch(-12, 127);
  turn(-15, 100);
  driveSet(-127, -127);
  timerReset(0);
  while (digitalRead(isWall) == 1 && digitalRead(isWall2) == 1 &&
         timer(0) < 2000) { // Repeats until the bumper sensor on the robot is
                            // pressed or 3 seconds pass
    delay(1);
  }
  driveStop();
  driveInch(12, 127);
  turn(77, 100);
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

void auto1() {
  gyroReset(gyro);
  encoderReset(lencoder);
  encoderReset(rencoder);
  jerk();// This is intended to drop and lock the intake
  delay(400);// waits 440 milliseconds for the intake to drop (AND LOCK)
  //GO FORWARD & PICK UP 1-3 STARS (+ intake), BACK UP TO PREVIOUS POSITION
  driveInchNoFix(22.5, 127); // Drives 32 inches forward at 63 power
  arr[0] = 0;
  arr[1] = potHalf;
  liftToHandle = taskCreate(liftToTask, TASK_DEFAULT_STACK_SIZE, (void *)arr, TASK_PRIORITY_DEFAULT);
  driveInch(12, 127); // 6
  mutexTake(potMutex, -1);
  int j = analogReadCalibrated(pot);
  mutexGive(potMutex);
  while(j < potHalf / 1.5) {
    mutexTake(potMutex, 5);
    j = analogReadCalibrated(pot);
    mutexGive(potMutex);
    delay(1);
  }
  driveInch(-30.5, 127);  // Drives 38 inches backward at 63 power
  //TURN BACK OF ROBOT TO WALL< LIFT UP, HIT WALL, DUMP
  turnNoFix(6, 127);        // Turns 83 degrees to the right at 40 power
  turn(86, 55);
  driveSet(-127, -127); // Sets the drive to go backwards at 70 power
  timerReset(0);
  arr[0] = 300; // 600
  arr[1] = potTop + 50;
  liftToHandle = taskCreate(liftToTask, TASK_DEFAULT_STACK_SIZE, (void *)arr, TASK_PRIORITY_DEFAULT);
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
  driveInchNoFix(25, 127); // 30
  arr[0] = 600;
  arr[1] = potHalf + 200;
  liftToHandle = taskCreate(liftToTask, TASK_DEFAULT_STACK_SIZE,
                                      (void *)arr, TASK_PRIORITY_DEFAULT);
  driveInch(6, 127);
  //
  delay(250);
  driveInch(-2, 127);
  turnNoFix(20, 70);
  driveSet(-127, -127);
  timerReset(0);
  arr[0] = 0;
  arr[1] = potTop; // Brings the lift to the top
  liftToHandle = taskCreate(liftToTask, TASK_DEFAULT_STACK_SIZE,
                                      (void *)arr, TASK_PRIORITY_DEFAULT);
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

///////////////////////////////////////////////////////////
///////////////DEFAULT AUTONOMOUS//////////////////
////////////////////////////////////////////////////////
////THIS IS A MARKER SO I CAN FIND THIS PART/////
//////////OF MY CODE WITH GREATER EASE//////////
/////////////////////////////////////////////////////
////////////////////////////////////////////////////
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


void autonomous() {
  arr = (int *)malloc(sizeof(int) * 2);
  FILE *chooser;
  int op2;
  if ((chooser = fopen("autoM", "r")) == NULL) {
    op2 = DEFAULT_AUTON;
  } else {
    op2 = fgetc(chooser);
  }
  fclose(chooser);
  switch (op2) {
  case 0:
    auto0();
    break;
  case 1:
    auto1();
    break;
  case 2:
    auto2();
    break;
  case 3:
    auto3();
    break;
  case 4:
    auto4();
    break;
  case 5:
    auto5();
    break;
  }
}
