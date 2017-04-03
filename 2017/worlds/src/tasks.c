#include "lib.h"

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
      driveSet(power, getMotor(TRD));
      mutexGive(driveMutex);
      delay(20);
    }

    while (encoderGet(lencoder) < (target - ENCO_TOL)) {
      check(4);
      mutexTake(driveMutex, -1);
      driveSet(power * ENCO_MULTIPLIER, getMotor(TRD));
      mutexGive(driveMutex);
      delay(20);
    }

    if (leftarr[FIX]) {
      delay(200);
      check(5);
      while (encoderGet(lencoder) > (target + ENCO_TOL)) {
        mutexTake(driveMutex, -1);
        driveSet(-power * ENCO_MULTIPLIER * ENCO_MULTIPLIER, getMotor(TRD));
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
      driveSet(-power, getMotor(TRD));
      mutexGive(driveMutex);
      delay(20);
    }

    while (encoderGet(lencoder) > (target + ENCO_TOL)) {
      check(8);
      mutexTake(driveMutex, -1);
      driveSet(-power * ENCO_MULTIPLIER, getMotor(TRD));
      mutexGive(driveMutex);
      delay(20);
    }

    if (leftarr[FIX]) {
      delay(200);
      check(9);
      while (encoderGet(lencoder) < (target - ENCO_TOL)) {
        check(10);
        mutexTake(driveMutex, -1);
        driveSet(power * ENCO_MULTIPLIER * ENCO_MULTIPLIER, getMotor(TRD));
        mutexGive(driveMutex);
        delay(20);
      }
    }
  }

  driveSet(0, getMotor(TRD));
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
      driveSet(getMotor(TLD), power);
      mutexGive(driveMutex);
      delay(20);
    }

    while (encoderGet(rencoder) < (target - ENCO_TOL)) {
      check(15);
      mutexTake(driveMutex, -1);
      driveSet(getMotor(TLD), power * ENCO_MULTIPLIER);
      mutexGive(driveMutex);
      delay(20);
    }

    if (rightarr[FIX]) {
      delay(200);
      check(16);
      while (encoderGet(rencoder) > (target + ENCO_TOL)) {
        check(17);
        mutexTake(driveMutex, -1);
        driveSet(getMotor(TLD), -power * ENCO_MULTIPLIER * ENCO_MULTIPLIER);
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
      driveSet(getMotor(TLD), -power);
      mutexGive(driveMutex);
      delay(20);
    }

    while (encoderGet(rencoder) > (target + ENCO_TOL)) {
      check(20);
      mutexTake(driveMutex, -1);
      driveSet(getMotor(TLD), power * ENCO_MULTIPLIER);
      mutexGive(driveMutex);
      delay(20);
    }

    if (rightarr[FIX]) {
      delay(200);
      check(21);
      while (encoderGet(rencoder) > (target + ENCO_TOL)) {
        check(22);
        mutexTake(driveMutex, -1);
        driveSet(getMotor(TLD), power * ENCO_MULTIPLIER * ENCO_MULTIPLIER);
        mutexGive(driveMutex);
        delay(20);
      }
    }
  }
  driveSet(getMotor(TLD), 0);
  check(23);
  mutexGive(isThatDone);
  taskSuspend(NULL);
}

void quickDump(void *none) {
  mutexGive(isThisDone);
  mutexTake(isThisDone, -1);
  rLiftTo(250, POTTOP); // 100
  driveInchNoFix(5, 127);
  timerReset(6);
  driveSet(-127, -127);
  while ((digitalRead(isWall) == 1 || digitalRead(isWall2) == 1) &&
         timer(6) <
             1750) { // Repeats until the either bumper sensor on the robot is
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

void lcdDisplayTime(void *parameter) {
  unsigned long tim;
  int min;
  while (true) {
    timerReset(8);
    if (isAutonomous()) {
      while (timer(8) <= 15000 && isAutonomous()) {
        tim = 15000 - timer(8);
        if (lcdMode == 1) {
          lcdPrint(uart1, 1, "%lu", tim / 1000);
          lcdPrint(uart1, 2, "Battery: %1.3f", (double)powerLevelMain() / 1000);
        } else {
          lcdPrint(uart1, 1, "Gyro %d | US %d", rGyros(), SONICGET);
          lcdPrint(uart1, 2, "L: %d | R: %d", encoderGet(lencoder), encoderGet(rencoder));
        }
        printValues();
        delay(20);
      }
    } else if (isEnabled()) {
      while (timer(8) <= 105000 && isEnabled() && isAutonomous() == false) {
        min = 0;
        tim = 105000 - timer(8);
        tim = tim / 1000;
        if (tim > 60) {
          min = 1;
          tim = tim - 60;
        }
        if (lcdMode == 1) {
          lcdPrint(uart1, 1, "%d : %lu", min, tim);
          lcdPrint(uart1, 2, "Batt: %1.3f V", (double)powerLevelMain() / 1000);
        } else {
          lcdPrint(uart1, 1, "Gyro %d | US %d", rGyros(), SONICGET);
          lcdPrint(uart1, 2, "L: %d | R: %d", encoderGet(lencoder), encoderGet(rencoder));
        }
        printValues();
        delay(20);
      }
    } else {
      while (!isEnabled()) {
        printValues();
        FILE *fd5;
        int opmd2;
        if ((fd5 = fopen("autoM", "r")) == NULL) {
          opmd2 = DEFAULT_AUTON;
        } else {
          opmd2 = fgetc(fd5);
        }
        fclose(fd5);
        if (lcdMode == 1) {
          lcdPrint(uart1, 1, "%d Auto: %d", (digitalRead(isWall) && digitalRead(isWall2)), opmd2);
          lcdPrint(uart1, 2, "Batt: %1.3f V", (double)powerLevelMain() / 1000);
        } else if (lcdMode == 2) {
          lcdPrint(uart1, 1, "Gyro %d | US %d", rGyros(), SONICGET);
          lcdPrint(uart1, 2, "L: %d | R: %d", encoderGet(lencoder), encoderGet(rencoder));
        } else if (lcdMode == 3) {
          lcdPrint(uart1, 1, "Recording rerun is %s", (rerunEnabled == true) ? "ON" : "OFF");
        }
        if (lcdReadButtons(uart1) == 4 && lcdMode == 1) {
          FILE *fd1;
          int value;
          if ((fd1 = fopen("autoM", "r")) == NULL) {
            fclose(fd1);
            FILE *fd2 = fopen("autoM", "w");
            value = DEFAULT_AUTON + 1;
            if (value > NUMBER_OF_AUTON)
              value = 0;
            fputc(value, fd2);
            fclose(fd2);
          } else {
            value = fgetc(fd1);
            fclose(fd1);
            FILE *fd4 = fopen("autoM", "w");
            if (value < NUMBER_OF_AUTON) {
              fputc(value + 1, fd4);
            } else {
              fputc(0, fd4);
            }
            fclose(fd4);
          }
          delay(500);
        } else if (lcdReadButtons(uart1) == 1 && lcdMode == 1) {
          FILE *fd1;
          int value;
          if ((fd1 = fopen("autoM", "r")) == NULL) {
            fclose(fd1);
            FILE *fd2 = fopen("autoM", "w");
            fputc(DEFAULT_AUTON - 1, fd2);
            fclose(fd2);
          } else {
            value = fgetc(fd1);
            fclose(fd1);
            FILE *fd4 = fopen("autoM", "w");
            if (value > 0) {
              fputc(value - 1, fd4);
            } else {
              fputc(NUMBER_OF_AUTON, fd4);
            }
            fclose(fd4);
          }
          delay(500);
        } else if (lcdReadButtons(uart1) == 5 && lcdMode == 2) {
          lcdSetText(uart1, 1, "Calibrating");
          lcdSetText(uart1, 2, "Please Wait...");
          calibrate();
          wait(1024);
        } else if (lcdReadButtons(uart1) == 2) {
          if (lcdMode == 1) {
            lcdMode = 2;
          } else if (lcdMode == 2) {
            lcdMode = 3;
          } else {
            lcdMode = 1;
          }
          delay(200);
        } else if (lcdReadButtons(uart1) == 1 && lcdMode == 3) {
          rerunEnabled = true;
          delay(200);
        } else if (lcdReadButtons(uart1) == 4 && lcdMode == 3) {
          rerunEnabled = false;
          delay(200);
        }
        delay(20);
      }
    }
  }
}

void liftToTask(void *parameter) {
  int set = (int)parameter;

  long unsigned int ms = vars[set];
  long liftToTaskPos = vars[(set + 1)];

  delay(ms);
  liftTo(liftToTaskPos);
  free((void *)parameter);
  taskDelete(NULL);
}

void stopAllPeriodic() {
  if (isEnabled() == false) {
    motorStopAll();
  }
}
