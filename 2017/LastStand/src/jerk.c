#include "constants.h"
#include "revision.h"
#include "autonomous.h"

void jerk(void) {
    float targetPosition = 6 * INCHESMULTIPLIER;
    gyroReset(gyro);
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
      printValues();
    }
    driveStop();
    //
    targetPosition = 0;
    leftAt = -100;
    rightAt = -100;
    while (abs(encoderGet(lencoder)) > 1 &&
           abs(encoderGet(rencoder)) > 1) {
      if (abs(encoderGet(lencoder)) <= 1) {
        leftAt = 0;
      }
      if (abs(encoderGet(rencoder)) <= 1) {
        rightAt = 0;
      }
      driveSet(leftAt, rightAt);
      delay(5);
      printValues();
    }
    driveStop();
    delay(250);
    leftAt = power / ENCO_CORRECTION;
    rightAt = power / ENCO_CORRECTION;
    while (abs(encoderGet(lencoder)) > 1 &&
           abs(encoderGet(rencoder)) > 1) {
      if (abs(encoderGet(lencoder)) <= 1) {
        leftAt = 0;
      }
      if (abs(encoderGet(rencoder)) <= 1) {
        rightAt = 0;
      }
      driveSet(leftAt, rightAt);
      delay(5);
      printValues();
    }
  driveStop();
  TURN_TOLERANCE = 1;
  //smartTurn(gyroGet(gyro) * -1, 37);
  TURN_TOLERANCE = 4;
}

void revisedJerk() {
  calibrate();

  float targetPosition = 6 * INCHESMULTIPLIER;

  //driveToEncDep(-targetPosition, -targetPosition, 127, 127, false);

  int power = 160;
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
    printValues();
  }

  while (abs(encoderGet(lencoder)) > 1 &&
         abs(encoderGet(rencoder)) > 1) {
    if (abs(encoderGet(lencoder)) <= 1) {
      leftAt = 0;
    }
    if (abs(encoderGet(rencoder)) <= 1) {
      rightAt = 0;
    }
    driveSet(leftAt, rightAt);
    delay(5);
    printValues();
  }
  driveStop();
  delay(250);
  leftAt = power / ENCO_CORRECTION;
  rightAt = power / ENCO_CORRECTION;
  while (abs(encoderGet(lencoder)) > 1 &&
         abs(encoderGet(rencoder)) > 1) {
    if (abs(encoderGet(lencoder)) <= 1) {
      leftAt = 0;
    }
    if (abs(encoderGet(rencoder)) <= 1) {
      rightAt = 0;
    }
    driveSet(leftAt, rightAt);
    delay(5);
    printValues();
  }
driveStop();
TURN_TOLERANCE = 1;
turnNoFix(gyroGet(gyro) * -0.5, 37);
TURN_TOLERANCE = 4;
}
