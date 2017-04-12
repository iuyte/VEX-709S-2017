#include "lib.h"
#include "functions.h"

void auto2() {
  calibrate();
  jerk();

  {
    int startp = 0;
    int startEnc = encoderGet(lencoder);
    long power = 110;
    long lpower = power;
    long rpower = power;
    int tolerance = 2;
    int goal = 12;
    float changer;
    while (encoderGet(lencoder) - startEnc < goal * INCHESMULTIPLIER) { // 45
      changer = (abs(rGyros() - startp) - tolerance) * 15;
      if (rGyros() - startp > tolerance) {
        lpower = power - changer;
        rpower = power + changer;
      } else if (rGyros() - startp < -tolerance) {
        lpower = power + changer;
        rpower = power - changer;
      }
      driveSet(lpower, rpower);
      delay(50);
    }
  }
  {
    int startp = 10;
    int startEnc = encoderGet(lencoder);
    long power = 110;
    long lpower = power;
    long rpower = power;
    int tolerance = 2;
    int goal = 17;
    float changer;
    while (encoderGet(lencoder) - startEnc < goal * INCHESMULTIPLIER) { // 45
      changer = (abs(rGyros() - startp) - tolerance) * 15;
      if (rGyros() - startp > tolerance) {
        lpower = power - changer;
        rpower = power + changer;
      } else if (rGyros() - startp < -tolerance) {
        lpower = power + changer;
        rpower = power - changer;
      }
      driveSet(lpower, rpower);
      delay(50);
    }
  }
  rLiftTo(0, POTHALF);
  {
    int startp = rGyros();
    int startEnc = encoderGet(lencoder);
    long power = 110;
    long lpower = power;
    long rpower = power;
    int tolerance = 2;
    int goal = 8;
    float changer;
    while (encoderGet(lencoder) - startEnc < goal * INCHESMULTIPLIER) { // 45
      changer = (abs(rGyros() - startp) - tolerance) * 20;
      if (rGyros() - startp > tolerance) {
        lpower = power - changer;
        rpower = power + changer;
      } else if (rGyros() - startp < -tolerance) {
        lpower = power + changer;
        rpower = power - changer;
      }
      driveSet(lpower, rpower);
      delay(50);
    }
  }
  driveStop();
  wLift(POTHALF * 0.6);
  turnNoFix(70, 100);
  driveInch(-18, 127);
  turnNoFix(10, 100);
  delay(50);
  rLiftTo(150, POTTOP);
  driveSet(-127, -127);
  while ((digitalRead(isWall) == 1 || digitalRead(isWall2) == 1) &&
         timer(3) < 750) { // Repeats until the button on the robot is pressed
                            // or 1 seconds pass
    delay(5);
  }
  driveStop();
}
