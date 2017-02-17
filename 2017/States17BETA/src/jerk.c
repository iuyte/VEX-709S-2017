#include "constants.h"
#include "ethanlib.h"

void jerk(void) {
  int jer = 1;
  if (jer == 0) {
    driveInchNoFix(4, 100);
    driveInch(-4, 100);
  } else if (jer == 1) {
    float targetPosition = 6 * INCHESMULTIPLIER; // 4
    encoderReset(rencoder);
    encoderReset(lencoder);
    int power = 100;
    int leftAt = power;
    int rightAt = power;
    while (abs(encoderGet(lencoder)) < targetPosition &&
           abs(encoderGet(rencoder)) < targetPosition) {
      if (abs(encoderGet(lencoder)) >= targetPosition) {
        leftAt = 0;
      }
      if (abs(encoderGet(rencoder)) >= targetPosition) {
        rightAt = 0;
      }
      driveSet(leftAt, rightAt);
      delay(5);
    }
    driveStop();
    //
    targetPosition = 0;
    leftAt = -100;
    rightAt = -100;
    while (abs(encoderGet(lencoder)) > 0 &&
           abs(encoderGet(rencoder)) > 0) {
      if (abs(encoderGet(lencoder)) <= 0) {
        leftAt = 0;
      }
      if (abs(encoderGet(rencoder)) <= 0) {
        rightAt = 0;
      }
      driveSet(leftAt, rightAt);
      delay(5);
    }
    driveStop();
    delay(250);
    leftAt = power / ENCO_CORRECTION;
    rightAt = power / ENCO_CORRECTION;
    while (abs(encoderGet(lencoder)) > abs(targetPosition) &&
           abs(encoderGet(rencoder)) > abs(targetPosition)) {
      if (abs(encoderGet(lencoder)) <= abs(targetPosition)) {
        leftAt = 0;
      }
      if (abs(encoderGet(rencoder)) <= abs(targetPosition)) {
        rightAt = 0;
      }
      driveSet(leftAt, rightAt);
      delay(5);
    }
  }
  driveStop();
}
