#include "constants.h"
#include "ethanlib.h"
#include "autonomous.h"

void autonomous() {
  arr = (int *)malloc(sizeof(int) * 2);
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
    auto8();
    break;
  case 9:
    auto9();
    break;
  case 10:
    auto10();
  }
}
