#include "constants.h"
#include "main.h"
// The following are my global variables
int liftToTaskPos;
int liftToTaskWait;
Mutex mutex;
// MOTOR PORTS//
// LIFT//

// INIT SENSORS//
Gyro gyro;        // Initializes the variable gyro to type Gyro
Encoder lencoder; // Initializes the variable lencoder (Left encoder) to type
                  // Encoder
Encoder rencoder; // Initializes the variable rencoder (Right encoder) to type
                  // Encoder

unsigned long startTimes[12] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
int drivemotorList[6] = {TLD, MLD, BLD, TRD, MRD, BRD};
int liftMotorList[6] = {ORL, OLL, TIRLBIRL, TILLBILL};

void timerReset(int number) { startTimes[number] = millis(); }

unsigned long timer(int number) {
  unsigned long value = millis() - startTimes[number];
  return value;
}

void liftSet(int power) {
  motorSet(OLL, 0 - power);
  motorSet(ORL, power);
  motorSet(TIRLBIRL, 0 - power);
  motorSet(TILLBILL, power);
}

void driveSet(int Lpower, int Rpower) {
  motorSet(TLD, Lpower);
  motorSet(MLD, Lpower);
  motorSet(BLD, -Lpower);
  motorSet(TRD, -Rpower);
  motorSet(MRD, -Rpower);
  motorSet(BRD, Rpower);
}

void driveStop() { driveSet(0, 0); }

void lcdDisplayTime(void *parameter) {
  unsigned long tim;
  int min;
  while (true) {
    timerReset(8);
    if (isAutonomous()) {
      while (timer(8) <= 15000 && isAutonomous()) {
        tim = 15000 - timer(8);
        lcdPrint(uart1, 1, "%lu", tim / 1000);
        lcdPrint(uart1, 2, "Battery: %1.3f", (double)powerLevelMain() / 1000);
        delay(10);
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
        lcdPrint(uart1, 1, "%d : %lu", min, tim);
        lcdPrint(uart1, 2, "Batt: %1.3f V", (double)powerLevelMain() / 1000);
        delay(10);
      }
    } else {
      while (isEnabled() == false) {
        // printf("%d", lcdReadButtons(uart1));
        FILE *fd5;
        int opmd2;
        if ((fd5 = fopen("autoM", "r")) == NULL) {
          opmd2 = 0;
        } else {
          opmd2 = fgetc(fd5);
        }
        fclose(fd5);
        lcdPrint(uart1, 1, "Auto: %d", opmd2);
        lcdPrint(uart1, 2, "Batt: %1.3f V", (double)powerLevelMain() / 1000);
        if (lcdReadButtons(uart1) == 4) {
          FILE *fd1;
          int value;
          if ((fd1 = fopen("autoM", "r")) == NULL) {
            fclose(fd1);
            FILE *fd2 = fopen("autoM", "w");
            fputc(1, fd2);
            fclose(fd2);
          } else {
            value = fgetc(fd1);
            fclose(fd1);
            FILE *fd4 = fopen("autoM", "w");
            if (value < NUMBER_OF_AUTON) {
              fputc(value + 1, fd4);
            } else {
              fputc(NUMBER_OF_AUTON, fd4);
            }
            fclose(fd4);
          }
          delay(500);
        } else if (lcdReadButtons(uart1) == 1) {
          FILE *fd1;
          int value;
          if ((fd1 = fopen("autoM", "r")) == NULL) {
            fclose(fd1);
            FILE *fd2 = fopen("autoM", "w");
            fputc(0, fd2);
            fclose(fd2);
          } else {
            value = fgetc(fd1);
            fclose(fd1);
            FILE *fd4 = fopen("autoM", "w");
            if (value > 0) {
              fputc(value - 1, fd4);
            } else {
              fputc(value, fd4);
            }
            fclose(fd4);
          }
          delay(500);
        }
        delay(10);
      }
    }
  }
}

void liftTo(int pos) {
  if (pos > analogReadCalibrated(pot)) {
    liftSet(100);
    while (analogReadCalibrated(pot) < pos - 10)
      delay(1);
  } else if (pos < analogReadCalibrated(pot)) {
    liftSet(-100);
    while (analogReadCalibrated(pot) > pos + 10)
      delay(1);
  }
  liftSet(liftZero);
}

void liftToTask(void *parameters[2]) {
  long unsigned int ms = (long unsigned int)parameters[0];
  delay(ms);
  long liftToTaskPos = (unsigned long)parameters[1];
  if (liftToTaskPos > analogReadCalibrated(pot)) {
    liftSet(100);
    while (analogReadCalibrated(pot) < liftToTaskPos - 10)
      delay(1);
  } else if (liftToTaskPos < analogReadCalibrated(pot)) {
    liftSet(-100);
    while (analogReadCalibrated(pot) > liftToTaskPos + 10)
      delay(1);
  }
  liftSet(liftZero);
  taskDelete(NULL);
}

void driveTo(float targetPosition, int power) {
  encoderReset(rencoder);
  encoderReset(lencoder);
  power = abs(power);
  if (targetPosition > 0) {
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
    leftAt = power / -2;
    rightAt = power / -2;
    while (abs(encoderGet(lencoder)) > targetPosition &&
           abs(encoderGet(rencoder)) > targetPosition) {
      if (abs(encoderGet(lencoder)) <= targetPosition) {
        leftAt = 0;
      }
      if (abs(encoderGet(rencoder)) <= targetPosition) {
        rightAt = 0;
      }
      driveSet(leftAt, rightAt);
      delay(5);
    }
    //driveSet(0 - power / 2, 0 - power / 2);
  } else if (targetPosition < 0) {
    int leftAt = 0 - power;
    int rightAt = 0 - power;
    while (abs(encoderGet(lencoder)) < abs(targetPosition) &&
           abs(encoderGet(rencoder)) < abs(targetPosition)) {
      if (abs(encoderGet(lencoder)) >= abs(targetPosition)) {
        leftAt = 0;
      }
      if (abs(encoderGet(rencoder)) >= abs(targetPosition)) {
        rightAt = 0;
      }
      driveSet(leftAt, rightAt);
      delay(5);
    }
    //driveSet(power / 2, power / 2);
    leftAt = power / 2;
    rightAt = power / 2;
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
  delay(100);
  driveStop();
}

void driveInch(float inches, int power) {
  inches = inches * inchesMultiplier;
  driveTo(inches, power);
}

void stopAllPeriodic() {
  if (isEnabled() == false) {
    motorStopAll();
  }
}

void turn(float degrees, int power) {
  int gyroZero = gyroGet(gyro);
  if (degrees > 0) {
    while (gyroGet(gyro) - gyroZero < degrees) {
      driveSet(power, -power);
      delay(5);
    }
    delay(250);
    //driveSet(0 - power / 2, power / 2);
    while (gyroGet(gyro) - gyroZero > degrees) {
      driveSet(power / -2, power / 2);
      delay(5);
    }

  } else if (degrees < 0) {
    while (gyroGet(gyro) - gyroZero > degrees) {
      driveSet(-power, power);
      delay(5);
    }
    delay(250);
    //driveSet(power / 2, 0 - power / 2);
    while (gyroGet(gyro) - gyroZero < degrees) {
      driveSet(power / 2, power / -2);
      delay(5);
    }
  }
  driveStop();
}

void turnTo(float degrees, int power) {
  if (degrees > gyroGet(gyro)) {
    while (gyroGet(gyro) < degrees) {
      driveSet(power, -power);
      delay(5);
    }
    delay(250);
    //driveSet(0 - power / 2, power / 2);
    while (gyroGet(gyro) > degrees) {
      driveSet(power / -2, power / 2);
      delay(5);
    }
  } else if (degrees < gyroGet(gyro)) {
    while (gyroGet(gyro) > degrees) {
      driveSet(-power, power);
      delay(5);
    }
    delay(250);
    //driveSet(power / 2, 0 - power / 2);
    while (gyroGet(gyro) < degrees) {
      driveSet(power / 2, power / -2);
      delay(5);
    }
  }
  driveStop();
}

void stopDriveAfter(void *milliseconds) {
  timerReset(10);
  unsigned long *ms = (unsigned long *)milliseconds;
  while ((unsigned long *)timer(10) < ms)
    delay(1);
  driveSet(0, 0);
  taskDelete(NULL);
}

void gyroResetAfter(void *milliseconds) {
  timerReset(11);
  unsigned long *ms = (unsigned long *)milliseconds;
  while ((unsigned long *)timer(11) < ms)
    delay(1);
  gyroReset(gyro);
  taskDelete(NULL);
}
