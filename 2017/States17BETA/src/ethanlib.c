#include "constants.h"
#include "main.h"
// The following are my global variables
Mutex potMutex;
// MOTOR PORTS//
// LIFT//
const char *uptown[3] = {
    "UptownGi:d=16,o=6,b=125:8e7,8p,8e7,8p,8e7,4p,8d#7,8e7,f#7,p,e7,p,d#7,p,8c#"
    "7,8c#7,p,8b,8g#,8b,4p,a,p,g#,p,a,p,b,p,a,p,g#,p,f#,p",
    "Upto:d=8,o=6,b=125:4e,4e,2e,4p,d#,e,f#,e,d#,c#,4c#,b5,g#5,2b5,4p,a5,g#5,"
    "a5,b5,a5,g#5,f#5,4e5,4e,2e,4p,e,d#,e,f#,e,d#,c#,c#,b5,b5,g#5,2b5,p,a5,g#5,"
    "a5,b5",
    "UptonGi:d=4,o=5,b=140:c6,c6,c.6,p,8c6,8b,8c6,8d6,8c6,8b,8a,a,g,g.,p,8f,8e,"
    "8f,8g,8f,8e,8d,c,c6,d.6,p,8c6,8b,8c6,8e6,8d6,8c6,8b,a,g,g.,p,8f,8e,8f,g,"
    "8a,8b,c6,c6,c.6"};
// INIT SENSORS//
Gyro gyro;        // Initializes the variable gyro to type Gyro
Encoder lencoder; // Initializes the variable lencoder (Left encoder) to type
                  // Encoder
Encoder rencoder; // Initializes the variable rencoder (Right encoder) to type
                  // Encoder
TaskHandle motorsSafe;
TaskHandle showTime;
TaskHandle liftToHandle;
int *arr;
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

void hard_reset(void) {
  uint32_t aircr = *((uint32_t *)0xE000ED0C);
  aircr = (aircr & 0xFFFF) | (0x5FA << 16) | 5;
  *((volatile uint32_t *)0xE000ED0C) = aircr;
  asm("DSB");
  while (1)
    ;
}

void driveStop(void) { driveSet(0, 0); }

void uptownPlay(void *parameter) { speakerPlayArray(uptown); }

void playUp(void *parameter) {
  delay(100);
  TaskHandle uptownHandle = taskCreate(uptownPlay, TASK_DEFAULT_STACK_SIZE,
                                       NULL, TASK_PRIORITY_DEFAULT);
  while (!isEnabled()) {
    delay(20);
  }
  taskDelete(uptownHandle);
  taskDelete(NULL);
}

void printButtons(void *parameter) {
  while (true)
    printf("%d", lcdReadButtons(uart1));
}

void lcdDisplayTime(void *parameter) {
  unsigned long tim;
  int min;
  while (true) {
    timerReset(8);
    if (isAutonomous()) {
      // TaskHandle upplayHandle = taskCreate(uptownPlay,
      // TASK_DEFAULT_STACK_SIZE, NULL, TASK_PRIORITY_DEFAULT);
      while (timer(8) <= 15000 && isAutonomous()) {
        tim = 15000 - timer(8);
        lcdPrint(uart1, 1, "%lu", tim / 1000);
        lcdPrint(uart1, 2, "Battery: %1.3f", (double)powerLevelMain() / 1000);
        delay(20);
      }
      // taskDelete(upplayHandle);
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
        delay(20);
      }
    } else {
      while (isEnabled() == false) {
        // printf("%d", lcdReadButtons(uart1));
        FILE *fd5;
        int opmd2;
        if ((fd5 = fopen("autoM", "r")) == NULL) {
          opmd2 = DEFAULT_AUTON;
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
              fputc(0, fd4);
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
              fputc(NUMBER_OF_AUTON, fd4);
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
  if (pos > analogReadCalibrated(POT)) {
    liftSet(100);
    while (analogReadCalibrated(POT) < pos - 10)
      delay(1);
  } else if (pos < analogReadCalibrated(POT)) {
    liftSet(-100);
    while (analogReadCalibrated(POT) > pos + 10)
      delay(1);
  }
  liftSet(LIFTZERO);
}

void liftToTask(void *parameters[2]) {
  long unsigned int ms = (long unsigned int)parameters[0];
  delay(ms);
  long liftToTaskPos = (unsigned long)parameters[1];
  int k = analogReadCalibrated(POT);
  if (liftToTaskPos > k) {
    liftSet(100);
    while (k < liftToTaskPos - 10) {
      mutexTake(potMutex, 5);
      k = analogReadCalibrated(POT);
      mutexGive(potMutex);
      delay(1);
    }
  } else if (liftToTaskPos < k) {
    liftSet(-100);
    while (k > liftToTaskPos + 10) {
      mutexTake(potMutex, 5);
      k = analogReadCalibrated(POT);
      mutexGive(potMutex);
      delay(1);
    }
  }
  liftSet(LIFTZERO);
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
    driveStop();
    delay(250);
    leftAt = power / -ENCO_CORRECTION;
    rightAt = power / -ENCO_CORRECTION;
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
    // driveSet(0 - power / 2, 0 - power / 2);
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
    // driveSet(power / 2, power / 2);
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
  // delay(100);
  driveStop();
}

void driveToAbs(float targetPosition, int power) {
  power = abs(power);
  long avg = (encoderGet(lencoder) + encoderGet(rencoder)) / 2;
  if (targetPosition > avg) {
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
    delay(250);
    leftAt = power / -ENCO_CORRECTION;
    rightAt = power / -ENCO_CORRECTION;
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
    // driveSet(0 - power / 2, 0 - power / 2);
  } else if (targetPosition < avg) {
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
    // driveSet(power / 2, power / 2);
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
  // delay(100);
  driveStop();
}

void driveToNoFix(float targetPosition, int power) {
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
    driveStop();
    // driveSet(0 - power / 2, 0 - power / 2);
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
    // driveSet(power / 2, power / 2);
    driveStop();
  }
  // delay(100);
  driveStop();
}

void driveInch(float inches, int power) {
  inches = inches * INCHESMULTIPLIER;
  driveTo(inches, power);
}

void driveInchNoFix(float inches, int power) {
  inches = inches * INCHESMULTIPLIER;
  driveToNoFix(inches, power);
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
    driveStop();
    delay(250);
    // driveSet(0 - power / 2, power / 2);
    while (gyroGet(gyro) - gyroZero > degrees) {
      driveSet(power * TURN_CORRECTION, power / TURN_CORRECTION);
      delay(5);
    }

  } else if (degrees < 0) {
    while (gyroGet(gyro) - gyroZero > degrees) {
      driveSet(-power, power);
      delay(5);
    }
    driveStop();
    delay(250);
    // driveSet(power / 2, 0 - power / 2);
    while (gyroGet(gyro) - gyroZero < degrees) {
      driveSet(power * TURN_CORRECTION, power / -TURN_CORRECTION);
      delay(5);
    }
  }
  driveStop();
}

void turnNoFix(float degrees, int power) {
  int gyroZero = gyroGet(gyro);
  if (degrees > 0) {
    while (gyroGet(gyro) - gyroZero < degrees) {
      driveSet(power, -power);
      delay(5);
    }
  } else if (degrees < 0) {
    while (gyroGet(gyro) - gyroZero > degrees) {
      driveSet(-power, power);
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
    driveStop();
    delay(250);
    // driveSet(0 - power / 2, power / 2);
    while (gyroGet(gyro) > degrees) {
      driveSet(power * TURN_CORRECTION, power / TURN_CORRECTION);
      delay(5);
    }
  } else if (degrees < gyroGet(gyro)) {
    while (gyroGet(gyro) > degrees) {
      driveSet(-power, power);
      delay(5);
    }
    driveStop();
    delay(250);
    // driveSet(power / 2, 0 - power / 2);
    while (gyroGet(gyro) < degrees) {
      driveSet(power * TURN_CORRECTION, power / -TURN_CORRECTION);
      delay(5);
    }
  }
  driveStop();
}

void smartTurn(float degrees, int power) {
  int gyroZero = gyroGet(gyro);
  turnNoFix(degrees * SMART_TURN_MULT, power);
  driveStop();
  delay(250);
  // driveSet(0 - power / 2, power / 2);
  if (degrees > 0) {
    while (gyroGet(gyro) - gyroZero > degrees + TURN_TOLERANCE) {
      driveSet(power * -TURN_CORRECTION, power * TURN_CORRECTION);
      delay(5);
    }
  } else if (degrees < 0) {
    while (gyroGet(gyro) - gyroZero < degrees - TURN_TOLERANCE) {
      driveSet(power * TURN_CORRECTION, power * -TURN_CORRECTION);
      delay(5);
    }
  }
  driveStop();
}

void smartTurnTo(float degrees, int power) {
  turnNoFix(degrees * SMART_TURN_MULT, power);
  driveStop();
  delay(250);
  // driveSet(0 - power / 2, power / 2);
  if (degrees > gyroGet(gyro)) {
    while (gyroGet(gyro) > degrees + TURN_TOLERANCE) {
      driveSet(power * -TURN_CORRECTION, power * TURN_CORRECTION);
      delay(5);
    }
  } else if (degrees < gyroGet(gyro)) {
    while (gyroGet(gyro) < degrees - TURN_TOLERANCE) {
      driveSet(power * TURN_CORRECTION, power * -TURN_CORRECTION);
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

void driveInchAbs(float inches, int power) {
  inches = inches * INCHESMULTIPLIER;
  driveToAbs(inches, power);
}

bool isLine(void) {
  bool re = false;
  if (analogReadCalibrated(LINE) >= LINELIGHT) {
    re = true;
  }
  return re;
}
