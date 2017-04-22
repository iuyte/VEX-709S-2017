#include "revision.h"
#include "constants.h"

void auto15() {
  calibrate();
  jerk();     // This is intended to drop and lock the intake
  delay(400); // waits 440 milliseconds for the intake to drop (AND LOCK)
  // GO FORWARD & PICK UP 1-3 STARS (+ intake), BACK UP TO PREVIOUS POSITION

  /////////////BEGIN PART 0 (count from 0 in CS)/////////////

  driveInchNoFix(20, 127); // Drives 24

  rLiftTo(0, POTHALF);

  driveInch(14, 127); // 6

  int j = analogReadCalibrated(POT);
  while (j < POTHALF / 1.5) {
    j = analogReadCalibrated(POT);
    delay(1);
  }

  driveInch(-32, 127); // -43

  // TURN BACK OF ROBOT TO WALL, LIFT UP, HIT WALL, DUMP
  rTurn(83, 2, 90, true, true); 

  checknum = 1;


  rLiftTo(600, POTTOP);

  checknum = 2;
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
  calibrate();
}
