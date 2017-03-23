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
  liftToRaw((int)position);
  goalReached[0] = true;
}

void leftToTask(void *position) {
  int targetPosition = (int)position;
  long avg = encoderGet(lencoder);
  int power = abs(DRIVE_POWER);
  if (targetPosition > avg + DRIVE_TO_TOLERANCE) {
    int leftAt = power;
    while (encoderGet(lencoder) < targetPosition) {
      driveSet(leftAt, getMotor(rightDrive.port));
      delay(5);
    }
  } else if (targetPosition < avg - DRIVE_TO_TOLERANCE) {
    int leftAt = -power;
    while (encoderGet(lencoder) > targetPosition) {
      driveSet(leftAt, getMotor(rightDrive.port));
      delay(5);
    }
  }
  driveSet(0, getMotor(rightDrive.port));
  goalReached[1] = true;
}

void rightToTask(void *position) {
  int targetPosition = (int)position;
  long avg = encoderGet(rencoder);
  int power = abs(DRIVE_POWER);
  if (targetPosition > avg + DRIVE_TO_TOLERANCE) {
    int rightAt = power;
    while (encoderGet(rencoder) < targetPosition) {
      driveSet(getMotor(leftDrive.port), rightAt);
      delay(5);
    }
  } else if (targetPosition < avg - DRIVE_TO_TOLERANCE) {
    int rightAt = -power;
    while (encoderGet(rencoder) > targetPosition) {
      driveSet(getMotor(leftDrive.port), rightAt);
      delay(5);
    }
  }
  driveSet(getMotor(leftDrive.port), 0);
  goalReached[2] = true;
}
