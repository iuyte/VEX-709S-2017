#include "lib.h"

void auto5() {
  calibrate();
  jerk();     // This is intended to drop and lock the intake
  // GO FORWARD & PICK UP 1-3 STARS (+ preload), BACK UP TO PREVIOUS POSITION

  driveInchNoFix(34, 127); // Drives 38 inches forward at 127 power

  rLiftTo(0, POTHALF);

  driveInchNoFix(16, 90); // 6 14

  wLift(POTHALF * .75);

  driveInch(-55, 127); // -43

  // TURN BACK OF ROBOT TO WALL, LIFT UP, HIT WALL, DUMP

  {
    turnToNoFix(87, 80);
    driveSet(-158, 158);
    delay(250);
    driveStop();
  }

  rLiftTo(900, POTTOP); // 650 550 600 675 735

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

  delay(250);
  checknum = 7;

  delay(600);

  liftTo(POTHALF * 1.5);
  rLiftTo(0, POTHALF);
  driveSet(-65, -65);
  while ((digitalRead(isWall) == 1 || digitalRead(isWall2) == 1) &&
         timer(1) < 750) { // Repeats until the button on the robot
                                        // is pressed
                                        // or 4 seconds pass
    delay(5);
    checknum = 4;
  }
  calibrate();
  rLiftTo(0, POTBOTTOM);

  delay(300);

  {
    turnToNoFix(-62, 110);
    driveSet(158, -158);
    delay(175);
    driveStop();
  }

  liftSet(-127);
  delay(50);
  liftSet(-10);

  driveInchNoFix(34, 127);
  rLiftTo(0, POTHALF);
  driveInchNoFix(14, 127);

  wLift(POTHALF);
  driveInchNoFix(-12, 127);
  turnNoFix(15, 110);
  driveInchNoFix(-5, 157);
  rLiftTo(0, POTTOP);
  driveInchNoFix(-30, 157);

  wLift(POTTOP);
  delay(650);
  liftTo(POTHALF);
  liftTo(POTTOP);

};
