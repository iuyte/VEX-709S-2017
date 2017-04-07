#include "lib.h"
#include "tasks.h"

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

void rLiftTo(long wait, int position) {
  mutexTake(potMutex, -1);
  mutexGive(potMutex);
  int set;
  if (vars[0] == 0) {set = 0;}
  else if (vars[2] == 0) {set = 2;}
  else if (vars[4] == 0) {set = 4;}
  else {set = 0;}
  vars[set] = wait;
  vars[set + 1] = position;
  TaskHandle liftHandle = taskCreate(liftToTask, TASK_DEFAULT_STACK_SIZE, (void *)set, TASK_PRIORITY_DEFAULT);
  delay(1);
  vars[set] = 0;
  vars[(set + 1)] = 0;
  taskGetState(liftHandle);
}

int trueSpeed(int speed) {
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

void fastDumpFromWall() {
  TaskHandle dumpHandle = taskCreate(quickDump, TASK_DEFAULT_STACK_SIZE, NULL, TASK_PRIORITY_DEFAULT);
  delay(100);
  while (!mutexTake(isThisDone, 5)) delay (20);
  taskDelete(dumpHandle);
  mutexGive(isThisDone);
}

void timerReset(int number) {
  startTimes[number] = millis();
}

unsigned long timer(int number) {
  unsigned long value = millis() - startTimes[number];
  return value;
}

int rGyros() { return (gyroGet(gyro) + gyroGet(gyra)) / 2; }

void calibrate(void) {
  encoderReset(lencoder);
  encoderReset(rencoder);
  gyroReset(gyro);
  gyroReset(gyra);
}

void printValues(void) {
  if (!USE_JINX) {
    mutexTake(potMutex, -1);
    if (timer(0) > 400) {
      printf("\n | %d | %d | SONC %d | LINE %d | LINE2 %d | LENC %d | RENC %d | LIFT %d | GYRO %d | LDRIV %d | RDRIV %d | TIM %lu | CHECK %d | BUT1 %d | BUT2 %d | TURNCO %f | \n",
             joystickGetAnalog(1, ACCEL_X), joystickGetAnalog(1, ACCEL_Y),
             ultrasonicGet(sonic), analogRead(LINE), analogRead(LINE2), encoderGet(lencoder),
             encoderGet(rencoder), analogReadCalibrated(POT), rGyros(),
             getMotor(TLD), getMotor(TRD), timer(1), checknum, digitalRead(isWall), digitalRead(isWall2), (float)TURN_CORRECTION);
      mutexGive(potMutex);
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
