#include "MyJinx.h"
#include "constants.h"
#include "motorSlew.h"

// The following are my global variables
float TURN_TOLERANCE = 3;
long powerTolerance[2] = {2, 127};
int blinker = 1;
int checknum = 0;
int lcdMode = 1;
Mutex potMutex;
Mutex timerMutex;
Mutex driveMutex;
Mutex isThisDone;
Mutex isThatDone;
bool thisDoneYet = true;
bool thatDoneYet = false;
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
Gyro gyra;
Encoder lencoder; // Initializes the variable lencoder (Left encoder) to type
                  // Encoder
Encoder rencoder; // Initializes the variable rencoder (Right encoder) to type
                  // Encoder
Ultrasonic sonic;
TaskHandle motorsSafe;
TaskHandle showTime;
TaskHandle liftToHandle;
TaskHandle calibrateHandle;
TaskHandle AutonHandle;
TaskHandle leftToHandle;
TaskHandle rightToHandle;

int *arr;
int drivemotorList[6] = {TLD, MLD, BLD, TRD, MRD, BRD};
int liftMotorList[6] = {ORL, OLL, TIRLBIRL, TILLBILL};

unsigned long startTimes[12] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

void timerReset(int number) {
  startTimes[number] = millis();
}

unsigned long timer(int number) {
  unsigned long value = millis() - startTimes[number];
  return value;
}

int rGyros() { return (gyroGet(gyro) + gyroGet(gyra)) / 2; }

void scalemotorRek(int motorPort, int motorPower) {
  motorRek(motorPort, motorPower * .8);
}

void liftSet(int power) {
  motorRek(OLL, -power * LIFT_CAP);
  motorRek(ORL, power * LIFT_CAP);
  motorRek(TIRLBIRL, -power * LIFT_CAP);
  motorRek(TILLBILL, power * LIFT_CAP);
}

int getMotor(int motorPort) {
  return motorReq[motorPort];
}

void driveSet(int Lpower, int Rpower) {
  mutexTake(driveMutex, -1);
  motorRek(TLD, Lpower * DRIVE_CAP);
  motorRek(MLD, Lpower * DRIVE_CAP);
  motorRek(BLD, -Lpower * DRIVE_CAP);
  motorRek(TRD, -Rpower * DRIVE_CAP);
  motorRek(MRD, -Rpower * DRIVE_CAP);
  motorRek(BRD, Rpower * DRIVE_CAP);
  mutexGive(driveMutex);
}

void driveSetBack(int Lpower, int Rpower) {
  mutexTake(driveMutex, -1);
  motorRek(MLD, Lpower * DRIVE_CAP);
  motorRek(BLD, -Lpower * DRIVE_CAP);
  motorRek(MRD, -Rpower * DRIVE_CAP);
  motorRek(BRD, Rpower * DRIVE_CAP);
  mutexGive(driveMutex);
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

void calibrate(void) {
  encoderReset(lencoder);
  encoderReset(rencoder);
  gyroReset(gyro);
  gyroReset(gyra);
}

void calibrateTask(void *parameter) { calibrate(); }

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

void printValues(void) {
  if (!USE_JINX) {
    mutexTake(potMutex, -1);
    mutexTake(driveMutex, -1);
    if (timer(0) > 400) {
      printf("\n | SONC %d | LINE %d | LINE2 %d | LENC %d | RENC %d | LIFT %d | GYRO %d | LDRIV %d | RDRIV %d | TIM %lu | CHECK %d | BUT1 %d | BUT2 %d | TURNCO %f | \n",
             ultrasonicGet(sonic), analogRead(LINE), analogRead(LINE2), encoderGet(lencoder),
             encoderGet(rencoder), analogReadCalibrated(POT), rGyros(),
             getMotor(TLD), getMotor(TRD), timer(1), checknum, digitalRead(isWall), digitalRead(isWall2), (float)TURN_CORRECTION);
      mutexGive(potMutex);
      mutexGive(driveMutex);
    } else {
      if (timer(0) > 400) {
        mutexTake(potMutex, -1);
        mutexTake(driveMutex, -1);
        printf("JINX_LINE %d\r\n", analogReadCalibrated(LINE));
        printf("JINX_LIFT POTENTIOMETER_%d\r\n", analogReadCalibrated(POT));
        printf("JINX_LEFT ENCODER_%d\r\n", encoderGet(lencoder));
        printf("JINX_RIGHT ENCODER_%d\r\n", encoderGet(rencoder));
        printf("JINX_LEFT DRIVE POWER_%d\r\n", getMotor(TLD));
        printf("JINX_RIGHT DRIVE POWER_%d\r\n", getMotor(TRD));
        printf("JINX_GYRO_%d\r\n", rGyros());
        mutexGive(potMutex);
        mutexGive(driveMutex);
        timerReset(timer(0));
      }
    }
  }
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
          lcdPrint(uart1, 1, "Auto: %d", opmd2);
          lcdPrint(uart1, 2, "Batt: %1.3f V", (double)powerLevelMain() / 1000);
        } else {
          lcdPrint(uart1, 1, "Gyro %d | US %d", rGyros(), SONICGET);
          lcdPrint(uart1, 2, "L: %d | R: %d", encoderGet(lencoder), encoderGet(rencoder));
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
          } else {
            lcdMode = 1;
          }
          delay(200);
        }
        delay(20);
      }
    }
  }
}

void liftTo(int pos) {
  mutexTake(potMutex, -1);
  int startp = analogReadCalibrated(POT);
  if (pos > analogReadCalibrated(POT)) {
    liftSet(127);
    while (analogReadCalibrated(POT) < pos - 10)
      delay(1);
  } else if (pos < analogReadCalibrated(POT)) {
    liftSet(-127);
    while (analogReadCalibrated(POT) > pos + 10)
      delay(1);
  }
  ///*
  delay(100);
  if (pos > startp) {
    liftSet(127);
    while (analogReadCalibrated(POT) < pos - 10)
      delay(1);
  } else if (pos < startp) {
    liftSet(-127);
    while (analogReadCalibrated(POT) > pos + 10)
      delay(1);
  }
  //*/
  liftSet(LIFTZERO);
  mutexGive(potMutex);
}

void liftToTask(void *parameters[2]) {
  long unsigned int ms = (long unsigned int)parameters[0];
  //mutexTake(potMutex, -1);
  //int k = analogReadCalibrated(POT);
  delay(ms);
  long liftToTaskPos = (unsigned long)parameters[1];
  /*
  if (liftToTaskPos > k) {
    liftSet(127);
    while (k < liftToTaskPos - 10) {
      k = analogReadCalibrated(POT);
      delay(1);
    }
  } else if (liftToTaskPos < k) {
    liftSet(-127);
    while (k > liftToTaskPos + 10) {
      k = analogReadCalibrated(POT);
      delay(1);
    }
  }

  liftSet(LIFTZERO);
  mutexGive(potMutex);
  */
  liftTo(liftToTaskPos);
  taskDelete(NULL);
}

void driveTo(float targetPosition, int power) {
  encoderReset(rencoder);
  encoderReset(lencoder);
  power = abs(power);
  if (targetPosition == 0) targetPosition = 1;
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
  int gyroZero = rGyros();
  if (degrees > 0) {
    while (rGyros() - gyroZero < degrees) {
      driveSet(power, -power);
      delay(5);
    }
    driveStop();
    delay(250);
    // driveSet(0 - power / 2, power / 2);
    while (rGyros() - gyroZero > degrees) {
      driveSet(power * TURN_CORRECTION, power / TURN_CORRECTION);
      delay(5);
    }

  } else if (degrees < 0) {
    while (rGyros() - gyroZero > degrees) {
      driveSet(-power, power);
      delay(5);
    }
    driveStop();
    delay(250);
    // driveSet(power / 2, 0 - power / 2);
    while (rGyros() - gyroZero < degrees) {
      driveSet(power * TURN_CORRECTION, power / -TURN_CORRECTION);
      delay(5);
    }
  }
  driveStop();
}

void turnNoFix(float degrees, int power) {
  int gyroZero = rGyros();
  if (degrees > 0) {
    while (rGyros() - gyroZero < degrees) {
      driveSet(power, -power);
      delay(5);
    }
  } else if (degrees < 0) {
    while (rGyros() - gyroZero > degrees) {
      driveSet(-power, power);
      delay(5);
    }
  }
  driveStop();
}

void turnTo(float degrees, int power) {
  if (degrees > rGyros()) {
    while (rGyros() < degrees) {
      driveSet(power, -power);
      delay(5);
    }
    driveStop();
    delay(250);
    // driveSet(0 - power / 2, power / 2);
    while (rGyros() > degrees) {
      driveSet(power * TURN_CORRECTION, power / TURN_CORRECTION);
      delay(5);
    }
  } else if (degrees < rGyros()) {
    while (rGyros() > degrees) {
      driveSet(-power, power);
      delay(5);
    }
    driveStop();
    delay(250);
    // driveSet(power / 2, 0 - power / 2);
    while (rGyros() < degrees) {
      driveSet(power * TURN_CORRECTION, power / -TURN_CORRECTION);
      delay(5);
    }
  }
  driveStop();
}

void smartTurn(float degrees, int power) {
  int gyroZero = rGyros();
  turnNoFix(degrees * SMART_TURN_MULT, power);
  driveStop();
  delay(250);
  int diff = sqrt(pow((abs(degrees - rGyros())), 3));
  if (degrees > 0) {
    while (rGyros() - gyroZero > degrees + TURN_TOLERANCE) {
      driveSet(-diff, diff);
      delay(5);
    }
  } else if (degrees < 0) {
    while (rGyros() - gyroZero < degrees - TURN_TOLERANCE) {
      driveSet(diff, -diff);
      delay(5);
    }
  }
  driveStop();
}

void smartTurnTo(float degrees, int power) {
  turnNoFix(degrees * SMART_TURN_MULT, power);
  driveStop();
  delay(250);
  int diff = sqrt(pow((abs(degrees - rGyros())), 3));
  if (degrees > rGyros()) {
    while (rGyros() > degrees + TURN_TOLERANCE) {
      driveSet(-diff, diff);
      delay(5);
    }
  } else if (degrees < rGyros()) {
    while (rGyros() < degrees - TURN_TOLERANCE) {
      driveSet(diff, -diff);
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
  if (analogRead(LINE) <= 2775 || analogRead(LINE2) <= 2460) {
    re = true;
  }
  return re;
}
