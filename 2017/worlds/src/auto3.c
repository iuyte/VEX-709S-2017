#include "lib.h"

void auto3() {
calibrate();
  jerk();     // This is intended to drop and lock the intake
  // GO FORWARD & PICK UP 1-3 STARS (+ intake), BACK UP TO PREVIOUS POSITION

  /////////////BEGIN PART 0 (count from 0 in CS)/////////////

  driveInchNoFix(38, 127); // Drives 40

  rLiftTo(0, POTHALF);

  driveInch(14, 127); // 6

  int j = analogReadCalibrated(POT);
  while (j < POTHALF / 1.5) {
    j = analogReadCalibrated(POT);
    delay(1);
  }

  driveInch(-51, 127); // -43

  // TURN BACK OF ROBOT TO WALL, LIFT UP, HIT WALL, DUMP
  turnNoFix(77, 100);
  driveSet(-100, 100);
  delay(50);
  driveStop();
  driveSet(50, -50);
  while (rGyros() < 85) delay(20);
  delay(50);

  rLiftTo(735, POTTOP); // 650 550 600 675

  checknum = 3;

  timerReset(1);
  driveSet(-157, -157);
  while ((digitalRead(isWall) == 1 || digitalRead(isWall2) == 1) &&
         timer(1) < MAX_TIME_TO_WALL) { // Repeats until the button on the robot
                                        // is pressed
                                        // or 4 seconds pass
    delay(5);
    checknum = 4;
  }

  checknum = 5;

  driveStop(); // Stops the robot

  checknum = 6;

  while (analogReadCalibrated(POT) < POTTOP - 50)
    delay(20);
  checknum = 7;

  delay(600);

  rLiftTo(0, POTHALF);
  wLift(POTHALF * 1.5);
  driveSet(-65, -65);
  while ((digitalRead(isWall) == 1 || digitalRead(isWall2) == 1) &&
         timer(1) < 500) { // Repeats until the button on the robot
                                        // is pressed
                                        // or 4 seconds pass
    delay(5);
    checknum = 4;
  }
  calibrate();
  rLiftTo(0, POTBOTTOM);

  delay(300);

  turnNoFix(-53, 90);
  driveSet(100, -100);
  delay(40);
  driveStop();
  turnNoFix(-10, 55);
  driveSet(100, -100);
  delay(40);
  driveStop();

  driveInchNoFix(36, 127);
  rLiftTo(0, POTHALF + 200);
  driveInchNoFix(12, 127);

  wLift(POTHALF);
  driveInchNoFix(-12, 127);
  rLiftTo(0, POTTOP);
  driveInch(-30, 157);

  wLift(POTTOP);
  delay(650);
  liftTo(POTHALF);
  liftTo(POTTOP);

};
