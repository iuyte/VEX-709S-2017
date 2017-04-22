#include "lib.h"

void auto4() {
  int driveMotorList[6] = {TLD, TRD, MLD, MRD, BLD, BRD};
  for (size_t i = 0; i < 6; i++) {
    motorRek(driveMotorList[i], 127);
    delay(500);
    motorRek(driveMotorList[i], 0);
  }

  for (size_t i = 0; i < 6; i++) {
    motorRek(driveMotorList[i], -127);
    delay(500);
    motorRek(driveMotorList[i], 0);
  }

  delay(2000);
  int liftMotorList[4] = {OLL, ORL, TILLBILL, TIRLBIRL};
  for (size_t i = 0; i < 4; i++) {
    motorRek(liftMotorList[i], 127);
    delay(500);
    motorRek(liftMotorList[i], 0);
  }
};
