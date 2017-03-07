#include "lib.h"

void initMotors(void) {
  motorManagerInit();

  int driveRate = MOTOR_DEFAULT_SLEW_RATE * 0.66;

  blrsMotorInit(OLL, true, MOTOR_DEFAULT_SLEW_RATE, trueSpeed);
  blrsMotorInit(ORL, false, MOTOR_DEFAULT_SLEW_RATE, trueSpeed);
  blrsMotorInit(TILLBILL, false, MOTOR_DEFAULT_SLEW_RATE, trueSpeed);
  blrsMotorInit(TIRLBIRL, true, MOTOR_DEFAULT_SLEW_RATE, trueSpeed);

  blrsMotorInit(TRD, true, driveRate, trueSpeed);
  blrsMotorInit(MRD, true, driveRate, trueSpeed);
  blrsMotorInit(BRD, false, driveRate, trueSpeed);
  blrsMotorInit(TLD, false, driveRate, trueSpeed);
  blrsMotorInit(MLD, false, driveRate, trueSpeed);
  blrsMotorInit(BLD, true, driveRate, trueSpeed);
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

int getMotor(int motorPort) {
  return blrsMotorGet(motorPort);
}

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

int truerSpeed(int speed) {return trueSpeed(speed);}
