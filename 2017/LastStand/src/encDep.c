#include "constants.h"
#include "encDep.h"

void check(int checkNum) {
  if (USE_JINX) {
    char outPuut[50];
    sprintf(outPuut, "Check #_%d", checkNum);
    printf("JINX_%s\r\n", outPuut);
  } else
    printf("CHECK %d", checkNum);
}

void leftTo(void *none) {
  check(1);
  mutexTake(isThisDone, -1);
  if (leftarr[VALUE] == 0) leftarr[VALUE] = 1;
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
  if (rightarr[VALUE] == 0) rightarr[VALUE] = 1;
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
  mutexTake(driveMutex, -1);
  driveStop();
  mutexGive(driveMutex);
  mutexGive(isThisDone);
  mutexGive(isThatDone);
}

void liftToAuto(long wait, int position) {
  mutexTake(potMutex, -1);
  mutexGive(potMutex);
  arr[0] = wait;
  arr[1] = position;
  TaskHandle liftHandle = taskCreate(liftToTask, TASK_DEFAULT_STACK_SIZE,
                                     (void *)arr, TASK_PRIORITY_DEFAULT);
  taskGetState(liftHandle);
}
