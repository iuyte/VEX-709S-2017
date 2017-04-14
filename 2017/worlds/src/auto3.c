#include "lib.h"

void auto3() {
  calibrate();
  jerk();     // This is intended to drop and lock the intake
  // GO FORWARD & PICK UP 1-3 STARS (+ preload), BACK UP TO PREVIOUS POSITION

  driveInchNoFix(20, 127); // Drives 38 inches forward at 127 power

  rLiftTo(0, POTHALF);

  driveInch(13, 90); // 6 14

  wLift(POTHALF * .75);

  driveInch(-26, 127); // -43

  // TURN BACK OF ROBOT TO WALL, LIFT UP, HIT WALL, DUMP
  smartTurnTo(90, 80);

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

  wLift(POTTOP);
  checknum = 7;

  delay(600);

  rLiftTo(0, POTHALF);
  wLift(POTHALF * 1.5);
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

  smartTurnTo(-53, 80);

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
