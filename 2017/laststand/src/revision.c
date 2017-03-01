#include "constants.h"
#include "revision.h"

void check(int um) {
  checknum = um;
}

float sgn(float num) {
  if (num > 0) {
    return 1;
  } else if (num < 0) {
    return -1;
  } else {
    return 0;
  }
}

void leftTo(void *none) {
  check(1);
  mutexTake(isThisDone, -1);
  if (leftarr[VALUE] == 0)
    leftarr[VALUE] = 1;
  int power = abs((int)leftarr[POWER]);
  long target = (long)leftarr[VALUE];
  if (encoderGet(lencoder) < target) {
    check(2);
    while (encoderGet(lencoder) < (target * ENCO_MULTIPLIER * 1.1)) {
      check(3);
      mutexTake(driveMutex, -1);
      driveSet(power, motorGet(TRD));
      mutexGive(driveMutex);
      delay(20);
    }

    while (encoderGet(lencoder) < (target - ENCO_TOL)) {
      check(4);
      mutexTake(driveMutex, -1);
      driveSet(power * ENCO_MULTIPLIER, motorGet(TRD));
      mutexGive(driveMutex);
      delay(20);
    }

    if (leftarr[FIX]) {
      delay(200);
      check(5);
      while (encoderGet(lencoder) > (target + ENCO_TOL)) {
        mutexTake(driveMutex, -1);
        driveSet(-power * ENCO_MULTIPLIER * ENCO_MULTIPLIER, motorGet(TRD));
        mutexGive(driveMutex);
        delay(20);
      }
    }
    ///////////////////////////////////////////////////////////////////
  } else if (encoderGet(lencoder) > (int)leftarr[VALUE]) {
    check(6);
    while (encoderGet(lencoder) > (target / (ENCO_MULTIPLIER * 1.1))) {
      check(7);
      mutexTake(driveMutex, -1);
      driveSet(-power, motorGet(TRD));
      mutexGive(driveMutex);
      delay(20);
    }

    while (encoderGet(lencoder) > (target + ENCO_TOL)) {
      check(8);
      mutexTake(driveMutex, -1);
      driveSet(-power * ENCO_MULTIPLIER, motorGet(TRD));
      mutexGive(driveMutex);
      delay(20);
    }

    if (leftarr[FIX]) {
      delay(200);
      check(9);
      while (encoderGet(lencoder) < (target - ENCO_TOL)) {
        check(10);
        mutexTake(driveMutex, -1);
        driveSet(power * ENCO_MULTIPLIER * ENCO_MULTIPLIER, motorGet(TRD));
        mutexGive(driveMutex);
        delay(20);
      }
    }
  }

  driveSet(0, motorGet(TRD));
  check(11);
  mutexGive(isThisDone);
  taskSuspend(NULL);
}

void rightTo(void *none) {
  check(12);
  mutexTake(isThatDone, -1);
  if (rightarr[VALUE] == 0)
    rightarr[VALUE] = 1;
  int power = abs((int)rightarr[POWER]);
  long target = (long)rightarr[VALUE];
  if (encoderGet(rencoder) < target) {
    check(13);
    while (encoderGet(rencoder) < (target * ENCO_MULTIPLIER * 1.1)) {
      check(14);
      mutexTake(driveMutex, -1);
      driveSet(motorGet(TLD), power);
      mutexGive(driveMutex);
      delay(20);
    }

    while (encoderGet(rencoder) < (target - ENCO_TOL)) {
      check(15);
      mutexTake(driveMutex, -1);
      driveSet(motorGet(TLD), power * ENCO_MULTIPLIER);
      mutexGive(driveMutex);
      delay(20);
    }

    if (rightarr[FIX]) {
      delay(200);
      check(16);
      while (encoderGet(rencoder) > (target + ENCO_TOL)) {
        check(17);
        mutexTake(driveMutex, -1);
        driveSet(motorGet(TLD), -power * ENCO_MULTIPLIER * ENCO_MULTIPLIER);
        mutexGive(driveMutex);
        delay(20);
      }
    }
    ///////////////////////////////////////////////////////////////////
  } else if (encoderGet(rencoder) > target) {
    check(18);
    while (encoderGet(rencoder) > (target / (ENCO_MULTIPLIER * 1.1))) {
      check(19);
      mutexTake(driveMutex, -1);
      driveSet(motorGet(TLD), -power);
      mutexGive(driveMutex);
      delay(20);
    }

    while (encoderGet(rencoder) > (target + ENCO_TOL)) {
      check(20);
      mutexTake(driveMutex, -1);
      driveSet(motorGet(TLD), power * ENCO_MULTIPLIER);
      mutexGive(driveMutex);
      delay(20);
    }

    if (rightarr[FIX]) {
      delay(200);
      check(21);
      while (encoderGet(rencoder) > (target + ENCO_TOL)) {
        check(22);
        mutexTake(driveMutex, -1);
        driveSet(motorGet(TLD), power * ENCO_MULTIPLIER * ENCO_MULTIPLIER);
        mutexGive(driveMutex);
        delay(20);
      }
    }
  }
  driveSet(motorGet(TLD), 0);
  check(23);
  mutexGive(isThatDone);
  taskSuspend(NULL);
}

void driveToEncDep(int lpos, int rpos, int lpower, int rpower,
                   bool correctionEnabled) {
  leftarr[FIX] = rightarr[FIX] = correctionEnabled;
  leftarr[VALUE] = lpos;
  rightarr[VALUE] = rpos;
  leftarr[POWER] = lpower;
  rightarr[POWER] = rpower;
  leftToHandle =
      taskCreate(leftTo, TASK_DEFAULT_STACK_SIZE, NULL, TASK_PRIORITY_DEFAULT);
  rightToHandle =
      taskCreate(rightTo, TASK_DEFAULT_STACK_SIZE, NULL, TASK_PRIORITY_DEFAULT);
  delay(20);
  mutexTake(isThisDone, -1);
  mutexTake(isThatDone, -1);
  taskDelete(leftToHandle);
  taskDelete(rightToHandle);
  driveStop();
  mutexGive(isThisDone);
  mutexGive(isThatDone);
}

void rLiftTo(long wait, int position) {
  mutexTake(potMutex, -1);
  mutexGive(potMutex);
  int *passThis = (int *)malloc(sizeof(int) * 2);
  passThis[0] = wait;
  passThis[1] = position;
  TaskHandle liftHandle = taskCreate(liftToTask, TASK_DEFAULT_STACK_SIZE,
                                     (void *)passThis, TASK_PRIORITY_DEFAULT);
  taskGetState(liftHandle);
}

short trueSpeed(short speed) {
  if (speed == 0) {
    return 0;
  }
  short x = abs(speed);

  if (x > 127) {
    x = 127;
  }
  return (
      sgn(speed) *
      ((((0.000001115136722 * x - 0.0001834554708) * x + 0.01010261354) * x +
        0.01924469053) *
           x +
       11.46841392));
}

void rTurn(int degrees, int tolerance, int power, bool isAbsolute, bool useAll6) {
  driveStop();
  if (!isAbsolute) {
    gyroReset(gyro);
    gyroReset(gyra);
  }
  while (rGyros() + tolerance < degrees ||
           rGyros() - tolerance > degrees) {
    if (degrees > rGyros() + tolerance) {
      while ((degrees / 2) > rGyros()) {
        if (useAll6) {
          driveSet(power, -power);
        } else {
          driveSetBack(power, -power);
        }
      }
      while ((degrees / 1.33) > rGyros()) {
        if (useAll6) {
          driveSet(power * .7, -power * .7);
        } else {
          driveSetBack(power * .7, -power * .7);
        }
      }
      while ((degrees / 1.5) > rGyros() + tolerance) {
        if (useAll6) {
          driveSet(power * .5, -power * .5);
        } else {
          driveSetBack(power * .5, -power * .5);
        }
      }
      while (degrees > rGyros() + tolerance) {
        if (useAll6) {
          driveSet(power * .35, -power * .35);
        } else {
          driveSetBack(power * .35, -power * .35);
        }
      }

    } else if (degrees < rGyros() - tolerance) {
      while ((degrees * 2) < rGyros()) {
        if (useAll6) {
          driveSet(-power, power);
        } else {
          driveSetBack(-power, power);
        }
      }
      while ((degrees * 1.33) < rGyros()) {
        if (useAll6) {
          driveSet(-power * .7, power * .7);
        } else {
          driveSetBack(-power * .7, power * .7);
        }
      }
      while ((degrees * 1.5) < rGyros() - tolerance) {
        if (useAll6) {
          driveSet(-power * .5, power * .5);
        } else {
          driveSetBack(-power * .5, power * .5);
        }
      }
      while (degrees < rGyros() - tolerance) {
        if (useAll6) {
          driveSet(-power * .35, power * .35);
        } else {
          driveSetBack(-power * .35, power * .35);
        }
      }
    }
    delay(150);
  }
  driveStop();
}

void quickDump(void *none) {
  mutexGive(isThisDone);
  mutexTake(isThisDone, -1);
  rLiftTo(100, POTTOP);
  driveInchNoFix(5, 127);
  timerReset(6);
  driveSet(-127, -127);
  while ((digitalRead(isWall) == 1 || digitalRead(isWall2) == 1) &&
         timer(6) <
             1300) { // Repeats until the either bumper sensor on the robot is
                     // pressed or 1 second passes
    delay(5);
  }
  driveStop();
  int j = analogReadCalibrated(POT);
  while (j < POTTOP) {
    j = analogReadCalibrated(POT);
    delay(20);
  }
  mutexGive(isThisDone);
  while (1) delay(500);
}

void fastDumpFromWall() {
  TaskHandle dumpHandle = taskCreate(quickDump, TASK_DEFAULT_STACK_SIZE, NULL, TASK_PRIORITY_DEFAULT);
  delay(100);
  while (!mutexTake(isThisDone, 5)) delay (20);
  taskDelete(dumpHandle);
  mutexGive(isThisDone);
}

void driveStraightUntil(void *none) {
  int startp = rGyros();
  long power = (long)passThis[0];
  long lpower = power;
  long rpower = power;
  int tolerance = (int)passThis[1];
  float changer;
  while (1) {
    changer = (abs(rGyros() - startp) - tolerance) * 15;
      if (rGyros() - startp > tolerance) {
        lpower = power - changer;
        rpower = power + changer;
      } else if (rGyros() - startp < -tolerance) {
        lpower = power + changer;
        rpower = power - changer;
      }
      driveSet(lpower, rpower);
      delay(50);
  }
  driveStop();
}

void rDriveSet(int power, int tolerance) {
  passThis[0] = power;
  passThis[1] = tolerance;
  rDriveHandle = taskCreate(liftToTask, TASK_DEFAULT_STACK_SIZE,
                                     NULL, TASK_PRIORITY_DEFAULT);
  taskGetState(rDriveHandle);
}

void rDriveStop(void) {
  taskDelete(rDriveHandle);
  driveStop();
}
