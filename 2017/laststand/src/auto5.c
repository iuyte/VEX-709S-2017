#include "ethanlib.h"
#include "constants.h"

void auto5() {
  for (size_t k = 0; k < 6; k++) {
    int mot = driveMotorList[k];
    motorSet(mot, 100);
    wait(500);
    motorStop(mot);
  }
  delay(1000);
  for (size_t k = 0; k < 6; k++) {
    int mot = liftMotorList[k];
    motorSet(mot, 100);
    wait(500);
    motorStop(mot);
    wait(500);
  }
  motorStopAll();
}
