#include "constants.h"
#include "revision.h"

void auto13() {
  calibrate();
  jerk();
  //smartTurnTo(0, 37);

  driveToEncDep(280, 280, 127, 127, false);

  driveToEncDep(462, 391, 127, 127, false);

  driveToEncDep(765, 765, 127, 127, false);

  liftTo(POTHALF);

  driveToEncDep(1, 1, 100, 100, true);
  ///*
  smartTurnTo(90, 90);

  for (size_t count = 0; count < 2; count++) {

    delay(3250);

    liftToAuto(700, POTTOP + 50);

    timerReset(1);
    while ((digitalRead(isWall) == 1 ||
           digitalRead(isWall2) == 1) &&
               (timer(1) < 4000)) { // Repeats until the buttons on the robot
                                    // are pressed or 4 seconds pass
      delay(1);
      driveStop();
    }
    driveSet(0, 0);

    liftTo(POTHALF);

    timerReset(1);
    while (!isLine() && (timer(1) < 4000)) {
      driveSet(100, 100);
      delay(1);
    }
    driveStop();
  }

  delay(3250);

  liftToAuto(700, POTTOP + 50);

  timerReset(1);
  while ((digitalRead(isWall) == 1 ||
         digitalRead(isWall2) == 1) &&
             (timer(1) < 4000)) { // Repeats until the buttons on the robot
                                  // are pressed or 4 seconds pass
    delay(1);
    driveStop();
  }
  driveSet(0, 0);

  liftTo(POTBOTTOM);
  //*/
}
