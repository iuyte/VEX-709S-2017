#include "main.h"
#include "constants.h"

int liftGoal = 0;
int liftTop = 1000;
int liftBottom = 5;
int liftToWait = 0;
unsigned long startTime;

void timerReset() {
  startTime = millis();
}

unsigned long timer() {
  unsigned long value = millis() - startTime;
  return value;
}

void hard_reset() {
    uint32_t aircr = *((uint32_t *)0xE000ED0C);
    aircr = (aircr & 0xFFFF) | (0x5FA << 16) | 5;
    *((volatile uint32_t *)0xE000ED0C) = aircr;
    asm("DSB");
    while (1);
}

void leftUntilLine(void *parameter) {
  while (abs(analogReadCalibratedHR(leftLine)) < whiteLine) {
    motorSet(leftDrive, 127);
  }
  motorSet(leftDrive, -63);
  delay(250);
  motorSet(leftDrive, 0);
  taskDelete(NULL);
}

void rightUntilLine(void *parameter) {
  while (abs(analogReadCalibratedHR(rightLine)) < whiteLine) {
    motorSet(rightDrive, 127);
  }
  motorSet(leftDrive, -63);
  delay(250);
  motorSet(leftDrive, 0);
  taskDelete(NULL);
}

void liftSet(int power) {
  motorSet(llift, power);
  motorSet(rlift, -power);
}

void driveSet(int left, int right) {
  motorSet(leftDrive, left);
  motorSet(rightDrive, right);
}

void liftTo(void *parameter) {
  encoderReset(encoder);
  if (liftGoal > encoderGet(encoder) + 10) {
    liftSet(127);
    while (encoderGet(encoder) < liftGoal) {
      delay(1);
    }
  } else if (liftGoal < encoderGet(encoder) - 10) {
    liftSet(-127);
    while (abs(encoderGet(encoder)) < abs(liftGoal)) {
      delay(1);
    }
  }
  liftSet(0);
  taskDelete(NULL);
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
  delay(200);
  driveSet(0, 0);
}
