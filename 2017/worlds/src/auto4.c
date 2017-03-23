#include "lib.h"

void auto4() {
  int driveMotorList[6] = {TLD, TRD, MLD, MRD, BLD, BRD};
  for (size_t i = 0; i < 6; i++) {
    motorRek(driveMotorList[i], 127);
    delay(500);
    motorRek(driveMotorList[i], 0);
  }
};
