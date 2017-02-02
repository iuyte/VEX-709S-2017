#include "main.h"
// The following are my global variables
int liftToTaskPos;
int liftToTaskWait;
// MOTOR PORTS//
// LIFT//
int OLL = 2;      // Outside Left Lift
int TILLBILL = 4; // Top Inside Left Lift and Bottom Inside Left Lift (y-cables
                  // to power expander)
int ORL = 9;      // Outside Right Lift
int TIRLBIRL = 7; // Top Inside Right Lift and Bottom Inside Right Lift
                  // (y-cables to power expander)
int liftZero = 17;
// DRIVE//
int TLD = 3; // Top Left Drive
int BLD = 5; // Back Left Drive
int TRD = 8; // Top Right Drive
int BRD = 6; // Back Right Drive
// DIGITAL INPUT//
int lencPort = 8;  // Left encoder top port is 8 (means that bottom port is 9)
int rencPort = 11; // Right encoder top port is 11(means that bottom port is
                   // 12)
int isWall = 1;    // Wall bumper port is digital 1
int isWall2 = 10;
// DIGITAL OUTPUT//
int hangLockLeft = 3;  // Left Hang Lock Pnuematic
int hangLockRight = 2; // Right Hang Lock Pnuematic
int hangHook = 4;      // Hook for hanging pneumatic
// ANALOG INPUT//
int pot = 4; // Potentiometer on lift in analog port 4
int potTop = 1800;
int potBottom = 25; // when the potentiometer is all the way down, this is it's position
int potHalf = 550; // When the lift is halfway up
int gyroPort = 2;  // Sets the gyroscope port
int isLine = 3;    // Sets the port for the line sensor
int lineLight =
    2000; // Sets the point where a line is white  if it's less than this
// INIT SENSORS//
Gyro gyro;        // Initializes the variable gyro to type Gyro
Encoder lencoder; // Initializes the variable lencoder (Left encoder) to type
                  // Encoder
Encoder rencoder; // Initializes the variable rencoder (Right encoder) to type
                  // Encoder
// Encoders are on a 5/8 gear ratio of actual rotation
// This means that there are 225 degrees in a rotation
// The diameter of the current omni wheels is 4", therefore 4*pi =
// 12.56637061435917295385057353311801153678867759750042328389
// This means that after 225 degrees rotated,
// 12.56637061435917295385057353311801153678867759750042328389 inches are
// traveled
// To interpret this, for every degree rotated, the robot travels forward
// apporoxamately 0.05585053606381854646155810459163560683017190043333521459507
// inches for every degree rotated (This is about 21/376 inches, which is about
// 1/18 of an inch), meaning that because 1/^THAT NUMBER^
// is equal to
// 17.90493109783822527399942337940786572887671014580135048412544009,
// that is the number of degrees necessary to be traveled for 1 inch

float inchesMultiplier =
    17.904931 /*09783822527399942337940786572887671014580135048412544009*/;

// float inchesMultiplier = 17.9049;

void liftSet(int power) {
  motorSet(OLL, 0 - power);
  motorSet(ORL, power);
  motorSet(TIRLBIRL, 0 - power);
  motorSet(TILLBILL, power);
}

void driveSet(int Lpower, int Rpower) {
  motorSet(TLD, Lpower);
  motorSet(BLD, Lpower);
  motorSet(TRD, 0 - Rpower);
  motorSet(BRD, 0 - Rpower);
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

void liftTask(void *parameter) {
  float toPos = potBottom;
  while (isEnabled()) {
    if (joystickGetDigital(1, 6, JOY_UP)) {
      toPos = toPos + 1;
    } else if (joystickGetDigital(1, 6, JOY_DOWN)) {
      toPos = toPos - 1;
    }
    liftTo(toPos);
  }
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
  delay(200);
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
  delay(200);
  driveSet(0, 0);
}
