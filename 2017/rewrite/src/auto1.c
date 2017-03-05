#include "lib.h"

void auto1() {
  calibrate();

  timerReset(3);
  driveSet(-100, -100);
  while ((digitalRead(isWall) == 1 || digitalRead(isWall2) == 1) &&
         timer(3) < 675) { // Repeats until the button on the robot is pressed
                            // or 1 seconds pass
    delay(5);
  }
  driveStop();
  check(50);
  calibrate();

  driveSet(80, -80);

  while (SONICGET > 80 || SONICGET == 0) {
    delay(10);
  }
  calibrate();

  delay(250);

  rTurn(15, 3, 55, true, false);

  driveInchNoFix(32, 127);
  rLiftTo(0, POTHALF);
  driveInchNoFix(8, 127);
  driveStop();

  turnNoFix(-15, 80);

  rLiftTo(400, POTTOP);
  delay(450);
  timerReset(3);
  driveSet(-127, -127);
  while ((digitalRead(isWall) == 1 || digitalRead(isWall2) == 1) &&
         timer(3) < 1500) { // Repeats until the button on the robot is pressed
                            // or 1 seconds pass
    delay(5);
  }
  driveStop();

  liftTo(POTHALF);
  driveInchNoFix(.5, 127);
  turnNoFix(20, 60);
  rLiftTo(0, POTTOP);
  delay(200);

  timerReset(3);
  driveSet(-127, -127);
  while ((digitalRead(isWall) == 1 || digitalRead(isWall2) == 1) &&
         timer(3) < 750) { // Repeats until the button on the robot is pressed
                            // or 1 seconds pass
    delay(5);
  }
  driveStop();


  driveStop();
  calibrate();
}
