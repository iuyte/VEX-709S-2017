#include "lib.h"

void auto1() {
  calibrate();
  jerk();

  {
    turnToNoFix(-40, 73);
  }

  driveInchNoFix(34, 140);

  rLiftTo(0, POTHALF);
  wLift(POTHALF * .7);

  turnNoFix(85, 90);

  rLiftTo(100, POTTOP);
  driveInchNoFix(-38, 127);
  calibrate();


}
