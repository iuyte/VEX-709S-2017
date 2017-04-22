#include "lib.h"

void auto3() {
  calibrate();
  jerk();     // This is intended to drop and lock the intake
  // GO FORWARD & PICK UP 1-3 STARS (+ preload), BACK UP TO PREVIOUS POSITION

  // driveInchNoFix(27, 127); // Drives 38 inches forward at 127 power

  int startp = rGyros() - 3;
  int startEnc = encoderGet(lencoder);
  long power = 127;
  long lpower = power;
  long rpower = power;
  int tolerance = 2;
  float changer;
  while (encoderGet(lencoder) - startEnc < 27 * INCHESMULTIPLIER) { // 45
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

  rLiftTo(0, POTHALF);

  driveInchNoFix(16, 90); // 6 14

  wLift(POTHALF * .6);

  driveInchNoFix(-38, 127); // -43
  {
    driveSet(100, 100);
    delay(50);
    driveStop();
  }
  driveInchNoFix(-11, 70);

  // TURN BACK OF ROBOT TO WALL, LIFT UP, HIT WALL, DUMP

  {
    turnToNoFix(85, 77);
    driveSet(-158, 158);
    delay(250);
    driveStop();
  }

  rLiftTo(900, POTTOP); // 650 550 600 675 735

  checknum = 3;

  timerReset(1);
  driveSet(-157, -157);
  while ((digitalRead(isWall) == 1 || digitalRead(isWall2) == 1) &&
         timer(1) < MAX_TIME_TO_WALL) { // Repeats until the button on the robot
                                        // is pressed
                                        // or 4 seconds pass
    delay(5);
    checknum = 4;
  }

  checknum = 5;

  driveStop(); // Stops the robot

  checknum = 6;

  checknum = 7;

  delay(600);

  liftTo(POTHALF * 1.6);
  rLiftTo(0, POTHALF);
  driveSet(-65, -65);
  while ((digitalRead(isWall) == 1 && digitalRead(isWall2) == 1) &&
         timer(1) < 750) { // Repeats until the button on the robot
                                        // is pressed
                                        // or 4 seconds pass
    delay(5);
    checknum = 4;
  }
  calibrate();
  rLiftTo(0, POTBOTTOM);

  delay(300);

  {
    turnToNoFix(-34, 85);
    /*
    driveSet(158, -158);
    delay(125);
    driveStop();
    */
  }

  liftSet(-127);
  delay(50);
  liftSet(-10);

  driveInchNoFix(32, 157);
  rLiftTo(0, POTHALF);
  driveInchNoFix(8, 127);

  wLift(POTHALF);
  driveInchNoFix(-12, 157);
  turnNoFix(15, 110);
  driveInchNoFix(-5, 157);
  rLiftTo(0, POTTOP);
  driveInchNoFix(-30, 157);

  wLift(POTTOP);
  delay(650);
  liftTo(POTHALF);
  liftTo(POTTOP);

};
