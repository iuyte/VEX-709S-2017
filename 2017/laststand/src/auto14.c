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

  driveSet(-160, -160);

  liftToAuto(600, POTTOP);

  while ((digitalRead(isWall) == 1 || digitalRead(isWall2)) == 1 &&
         timer(1) < 4000) { // Repeats until the button on the robot is pressed
                            // or 4 seconds pass
    delay(1);
  }

  driveStop(); // Stops the robot

  while (analogReadCalibrated(POT) < POTTOP - 50) delay(20);

  delay(600);

  liftTo(POTHALF);

  driveSet(100, 100);
  while (!isLine()) delay(20);
  driveStop();

  delay(3250);

  liftToAuto(600, POTTOP);

  driveSet(-160, -160);
  while ((digitalRead(isWall) == 1 || digitalRead(isWall2)) == 1 &&
         timer(1) < 4000) { // Repeats until the button on the robot is pressed
                            // or 4 seconds pass
    delay(1);
  }

  driveStop(); // Stops the robot


  while (analogReadCalibrated(POT) < POTTOP - 50) delay(20);

  delay(600);

  liftTo(POTHALF);

  driveSet(100, 100);
  while (!isLine()) delay(20);
  driveStop();

  delay(3250);

  liftToAuto(600, POTTOP);

  driveSet(-160, -160);
  while ((digitalRead(isWall) == 1 || digitalRead(isWall2)) == 1 &&
         timer(1) < 4000) { // Repeats until the button on the robot is pressed
                            // or 4 seconds pass
    delay(1);
  }

  driveStop(); // Stops the robot

  driveStop();
  calibrate();
}
