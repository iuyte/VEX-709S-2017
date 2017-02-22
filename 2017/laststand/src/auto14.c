#include "revision.h"
#include "constants.h"

void auto14() {
  calibrate();
  jerk();// This is intended to drop and lock the intake
  delay(400);// waits 440 milliseconds for the intake to drop (AND LOCK)
  //smartTurnTo(0, 50);
  //GO FORWARD & PICK UP 1-3 STARS (+ intake), BACK UP TO PREVIOUS POSITION

  driveInchNoFix(30, 127); // Drives 24

  /*
  arr[0] = 0;
  arr[1] = POTHALF;
  liftToHandle = taskCreate(liftToTask, TASK_DEFAULT_STACK_SIZE, (void *)arr, TASK_PRIORITY_DEFAULT);
  */
  liftToAuto(0, POTHALF);

  driveInch(14, 127); // 6

  int j = analogReadCalibrated(POT);
  while(j < POTHALF / 1.5) {
    j = analogReadCalibrated(POT);
    delay(1);
  }

  driveInch(-38, 127);  // -43

  //TURN BACK OF ROBOT TO WALL< LIFT UP, HIT WALL, DUMP
  smartTurn(90, 100);

  delay(3250);

  driveSet(-160, -160); // Sets the drive to go backwards at 127 power

  mutexGive(timerMutex);
  timerReset(1);

  /*
  arr[0] = 600; // 300
  arr[1] = POTTOP;
  liftToHandle = taskCreate(liftToTask, TASK_DEFAULT_STACK_SIZE, (void *)arr, TASK_PRIORITY_DEFAULT);
  */

  liftToAuto(600, POTTOP);

  //TaskHandle gyroResetIn = taskCreate(gyroResetAfter, TASK_DEFAULT_STACK_SIZE, (void *)1000, TASK_PRIORITY_DEFAULT);

  while ((digitalRead(isWall) == 1 || digitalRead(isWall2)) == 1 &&
         timer(1) < 4000) { // Repeats until the button on the robot is pressed
                            // or 4 seconds pass
    delay(1);
  }

  driveStop(); // Stops the robot

  timerReset(1);

  while (!mutexTake(potMutex, 5) && timer(1) < 1500) delay(1);

  delay(600);

  //LIFT TO BOTTOM, TURN & DRIVE FORWARD (GET CUBE) LIFT UP

  /*
  arr[0] = 0;
  arr[1] = POTBOTTOM; // Takes the lift down to the bottom
  liftToHandle = taskCreate(liftToTask, TASK_DEFAULT_STACK_SIZE,
                                       (void *)arr, TASK_PRIORITY_DEFAULT);
  */

  liftTo(POTBOTTOM);

  /*
  delay(1500);
  liftSet(-52);
  delay(250);
  liftSet(0);
  */

  smartTurnTo(-39, 127); // -50

  driveInchNoFix(18, 127); // 25

  /*
  arr[0] = 600;
  arr[1] = POTHALF + 200;
  liftToHandle = taskCreate(liftToTask, TASK_DEFAULT_STACK_SIZE,
                                      (void *)arr, TASK_PRIORITY_DEFAULT);

  */

  liftToAuto(600, POTHALF);

  driveInch(10, 127);

  //
  delay(250);
  turnNoFix(20, 70);
  driveSet(-127, -127);
  timerReset(1);
  arr[0] = 0;
  arr[1] = POTTOP; // Brings the lift to the top
  liftToHandle = taskCreate(liftToTask, TASK_DEFAULT_STACK_SIZE,
                                      (void *)arr, TASK_PRIORITY_DEFAULT);
  while (digitalRead(isWall) == 1 && digitalRead(isWall2) == 1 &&
         timer(1) < 2000) { // Repeats until the bumper sensor on the robot is
                            // pressed or 3 seconds pass
    delay(1);
  }
  driveStop();
  gyroReset(gyro);
  encoderReset(lencoder);
  encoderReset(rencoder);
}
