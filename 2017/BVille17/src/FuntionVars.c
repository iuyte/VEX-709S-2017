#include "main.h"
#include "constants.h"
// The following are my global variables
int liftToTaskPos;
int liftToTaskWait;
// MOTOR PORTS//
// LIFT//

// INIT SENSORS//
Gyro gyro;        // Initializes the variable gyro to type Gyro
Encoder lencoder; // Initializes the variable lencoder (Left encoder) to type
                  // Encoder
Encoder rencoder; // Initializes the variable rencoder (Right encoder) to type
                  // Encoder

unsigned long startTimes[9] = {0, 0, 0, 0, 0, 0, 0, 0, 0};
int motorList[6] = {TLD, MLD, BLD, TRD, MRD, BRD};//, ORL, OLL, TIRLBIRL, TILLBILL};

void timerReset(int number) {
  startTimes[number] = millis();
}

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

void lcdDisplayTime(void *parameter) {
  unsigned long tim;
  int min;
  while (true) {
    timerReset(8);
    if (isAutonomous()) {
      while (timer(8) <= 15000) {
          tim = 15000 - timer(8);
        lcdPrint(uart1, 1, "%lu", tim / 1000);
        lcdPrint(uart1, 2, "Battery: %1.3f", (double)powerLevelMain() / 1000);
        delay(10);
      }
    } else if (isEnabled()) {
      while (timer(8) <= 105000) {
        min = 0;
        tim = 105000 - timer(8);
        tim = tim / 1000;
        if (tim > 60) {
          min = 1;
          tim = tim - 60;
        }
        lcdPrint(uart1, 1, "%d : %lu", min,  tim);
        lcdPrint(uart1, 2, "Batt: %1.3f V", (double)powerLevelMain() / 1000);
        delay(10);
      }
    } else {
      while (isEnabled() == false) {
        lcdSetText(uart1, 1, "  Opmode: Stop  ");
        lcdPrint(uart1, 2, "Batt: %1.3f V", (double)powerLevelMain() / 1000);
        delay(10);
      }
    }
  }
}

void liftTo(int pos) {
  if (pos > analogReadCalibrated(pot)) {
    liftSet(100);
    while (analogReadCalibrated(pot) < pos - 10)
      ;
  } else if (pos < analogReadCalibrated(pot)) {
    liftSet(-100);
    while (analogReadCalibrated(pot) > pos + 10)
      ;
  }
  liftSet(liftZero);
}

void liftToTask(void *parameter) {
  delay(liftToTaskWait);
  if (liftToTaskPos > analogReadCalibrated(pot)) {
    liftSet(100);
    while (analogReadCalibrated(pot) < liftToTaskPos - 10)
      ;
  } else if (liftToTaskPos < analogReadCalibrated(pot)) {
    liftSet(-100);
    while (analogReadCalibrated(pot) > liftToTaskPos + 10)
      ;
  }
  liftSet(liftZero);
  taskDelete(NULL);
}

void driveTo(float targetPosition, int power) {
  encoderReset(rencoder);
  encoderReset(lencoder);
  if (targetPosition > 0) {
    int leftAt = power;
    int rightAt = power;
    while (encoderGet(lencoder) < targetPosition &&
           encoderGet(rencoder) < targetPosition) {
      if (encoderGet(lencoder) >= targetPosition) {
        leftAt = 0;
      }
      if (encoderGet(rencoder) >= targetPosition) {
        rightAt = 0;
      }
      driveSet(leftAt, rightAt);
    }
    driveSet(0 - power / 2, 0 - power / 2);
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
    }
    driveSet(power / 2, power / 2);
  }
  delay(100);
  driveSet(0, 0);
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
  gyroReset(gyro);
  if (degrees > 0) {
    while (gyroGet(gyro) < degrees) {
      driveSet(power, -power);
    }
    driveSet(0 - power / 2, power / 2);
  } else if (degrees < 0) {
    while (gyroGet(gyro) > degrees) {
      driveSet(-power, power);
    }
    driveSet(power / 2, 0 - power / 2);
  }
  delay(150);
  driveSet(0, 0);
}
