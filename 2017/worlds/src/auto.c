#include "lib.h"
#include "defaultAuton.h"

void encoderValues() {
  int startL = encoderGet(lencoder);
  int startR = encoderGet(lencoder);
  while (isAutonomous()) {
    if (encoderGet(lencoder) < startL - 5 ||
        encoderGet(lencoder) > startL + 5 ||
        encoderGet(rencoder) < startR - 5 ||
        encoderGet(rencoder) > startL + 5) {
      printf("%d | %d\n", encoderGet(lencoder), encoderGet(rencoder));
    }
    startL = encoderGet(lencoder);
    startR = encoderGet(lencoder);
    delay(100);
  }
}

void autonomous() {
  FILE *chooser;
  int op2;
  if ((chooser = fopen("autoM", "r")) == NULL) {
    op2 = DEFAULT_AUTON;
  } else {
    op2 = fgetc(chooser);
  }
  fclose(chooser);
  switch (op2) {
  case 0:
    auto0();
    break;
  case 1:
    auto1();
    break;
  case 2:
    auto2();
    break;
  case 3:
    auto3();
    break;
  case 4:
    auto4();
    break;
  case 5:
    break;
  case 6:
    auto6();
  case 7:
    auto7();
    break;
  }
  while (isAutonomous())
    delay(50);
}
