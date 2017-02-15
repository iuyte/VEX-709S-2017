#include "ethanlib.h"
#include "constants.h"

void auto4() {
  for (size_t k = 0; k < 6; k++) {
    int mot = driveMotorList[k];
    motorSet(mot, 127);
    wait(500);
    motorStop(mot);
  }
}
