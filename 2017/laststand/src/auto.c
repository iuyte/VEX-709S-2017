#include "autonomous.h"
#include "constants.h"
#include "ethanlib.h"

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
    auto5();
    break;
  case 6:
    auto6();
    break;
  case 7:
    auto7();
    break;
  case 8:
    jerk();
    break;
  case 9:
    auto9();
    break;
  case 10:
    auto10();
  case 11:
    encoderValues();
    break;
  case 12:
    while (isAutonomous())
      delay(50);
    break;
  case 13:
    auto13();
    break;
  case 14:
    auto14();
    break;
  case 15:
    //auto15();
    break;
  case 16:
    //auto16();
    break;
  }
  while (isAutonomous()) delay(50);
}
