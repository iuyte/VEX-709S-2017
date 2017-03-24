#include "lib.h"

void initMotors(void) {
  motorManagerInit();

  int driveRate = 10;

  blrsMotorInit(OLL, true, MOTOR_DEFAULT_SLEW_RATE, typeofSpeed);
  blrsMotorInit(ORL, false, MOTOR_DEFAULT_SLEW_RATE, typeofSpeed);
  blrsMotorInit(TILLBILL, false, MOTOR_DEFAULT_SLEW_RATE, typeofSpeed);
  blrsMotorInit(TIRLBIRL, true, MOTOR_DEFAULT_SLEW_RATE, typeofSpeed);

  blrsMotorInit(TRD, true, driveRate, typeofSpeed);
  blrsMotorInit(MRD, true, driveRate, typeofSpeed);
  blrsMotorInit(BRD, false, driveRate, fakeSpeed);
  blrsMotorInit(TLD, false, driveRate, typeofSpeed);
  blrsMotorInit(MLD, false, driveRate, typeofSpeed);
  blrsMotorInit(BLD, true, driveRate, fakeSpeed);
}

void motorRek(int motorPort, int power) {
  blrsMotorSet(motorPort, power, false);
}

void motorSetImmediate(unsigned int port, int power) {
  blrsMotorSet(port, power, true);
}

void liftSet(int power) {
  power *= LIFT_CAP;
  motorSetImmediate(OLL, power);
  motorSetImmediate(ORL, power);
  motorSetImmediate(TIRLBIRL, power);
  motorSetImmediate(TILLBILL, power);
}

int getMotor(int motorPort) { return blrsMotorGet(motorPort); }

void driveSet(int Lpower, int Rpower) {
  Lpower *= DRIVE_CAP;
  Rpower *= DRIVE_CAP;
  motorRek(TLD, Lpower);
  motorRek(MLD, Lpower);
  motorRek(BLD, Lpower);
  motorRek(TRD, Rpower);
  motorRek(MRD, Rpower);
  motorRek(BRD, Rpower);
}

void driveSetBack(int Lpower, int Rpower) {
  Lpower *= DRIVE_CAP;
  Rpower *= DRIVE_CAP;
  motorRek(MLD, Lpower);
  motorRek(BLD, Lpower);
  motorRek(MRD, Rpower);
  motorRek(BRD, Rpower);
}

void driveStop(void) { driveSet(0, 0); }

int truerSpeed(int speed) { return trueSpeed(speed); }

int fakeSpeed(int speed) { return speed; }

int gudSpeed(int speed) { return (sgn(speed) * TrueSpeed[abs(speed)]); }

void accelDrive() {
  prevX = accelX;
  prevY = accelY;
  accelX = 0 - joystickGetAnalog(1, ACCEL_X);
  accelY = 0 - joystickGetAnalog(1, ACCEL_Y);
  int threshold = 20;
  int multiplier = 1.1;

  if (abs(accelX) < threshold) accelX = 0;
  if (abs(accelY) < threshold) accelY = 0;

  accelX *= multiplier;
  accelY *= (multiplier * 1.25);

  driveSet(accelX - accelY, accelX + accelY);

}
