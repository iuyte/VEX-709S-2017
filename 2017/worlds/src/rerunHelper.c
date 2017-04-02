#include "rerun.h"

int liftGet(void) {return analogReadCalibrated(POT);}
int leftGet(void) {return encoderGet(lencoder);}
int rightGet(void) {return encoderGet(rencoder);}

void liftToRaw(int pos) {
  mutexTake(potMutex, -1);
  if (pos > analogReadCalibrated(POT)) {
    liftSet(127);
    while (analogReadCalibrated(POT) < pos - 10)
      delay(1);
  } else if (pos < analogReadCalibrated(POT)) {
    liftSet(-127);
    while (analogReadCalibrated(POT) > pos + 10)
      delay(1);
  }
  liftSet(LIFTZERO);
  mutexGive(potMutex);
}

void liftToRawTask(void *position) {
  int pos = (int)position;
  mutexTake(potMutex, -1);
  if (pos > analogReadCalibrated(POT)) {
    liftSet(127);
  } else if (pos < analogReadCalibrated(POT)) {
    liftSet(-127);
  } else {
    liftSet(LIFTZERO);
  }
  mutexGive(potMutex);
  goalReached[0] = true;
}

void leftToTask(void *position) {
  int targetPosition = (int)position;
  long avg = encoderGet(lencoder);
  int power = abs(DRIVE_POWER);
  int leftAt;
  if (targetPosition > avg + DRIVE_TO_TOLERANCE) {
    leftAt = power;

  } else if (targetPosition < avg - DRIVE_TO_TOLERANCE) {
    leftAt = -power;
  } else {
    leftAt = 0;
  }
  driveSet(leftAt, getMotor(rightDrive.port));
  goalReached[1] = true;
}

void rightToTask(void *position) {
  int targetPosition = (int)position;
  long avg = encoderGet(rencoder);
  int power = abs(DRIVE_POWER);
  int rightAt;
  if (targetPosition > avg + DRIVE_TO_TOLERANCE) {
    rightAt = power;

  } else if (targetPosition < avg - DRIVE_TO_TOLERANCE) {
    rightAt = -power;
  } else {
    rightAt = 0;
  }
  driveSet(getMotor(leftDrive.port), rightAt);
  goalReached[2] = true;
}
