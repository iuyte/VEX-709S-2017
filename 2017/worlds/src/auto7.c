#include "lib.h"

void auto7() {
  calibrate();
  driveStop(); // Stops the robot

/////////////////
// BEGIN PART 2
////////////////

  rLiftTo(0, POTBOTTOM);
  timerReset(3);
  driveSet(-100, -100);
  while ((digitalRead(isWall) == 1 || digitalRead(isWall2) == 1) &&
         timer(3) < 675) { // Repeats until the button on the robot is pressed
                           // or 1 seconds pass
    delay(5);
  }
  driveStop();
  check(50);
  calibrate();

  delay(750);

  smartTurn(73, 60); // 67 68 70 72 73 too little

  driveStop();

  delay(250);

  driveInchNoFix(43, 127); // 38
  rLiftTo(0, POTHALF);
  driveInchNoFix(11, 150);
  driveStop();

  turnNoFix(-10, 80);

  rLiftTo(700, POTTOP);
  delay(450);
  timerReset(3);
  driveSet(-127, -127);
  while ((digitalRead(isWall) == 1 || digitalRead(isWall2) == 1) &&
         timer(3) < 1500) { // Repeats until the button on the robot is pressed
                            // or 1 seconds pass
    delay(5);
  }
  driveStop();
  if (analogReadCalibrated(POT) < POTTOP - 52)
    wLift(POTTOP - 50);
  delay(600);
  liftTo(POTBOTTOM);
  timerReset(3);
  driveSet(-127, -127);
  while ((digitalRead(isWall) == 1 || digitalRead(isWall2) == 1) &&
         timer(3) < 1500) { // Repeats until the button on the robot is pressed
                            // or 1 seconds pass
    delay(5);
  }
  driveStop();
  calibrate();

  {
    turnToNoFix(-51.5, 90);
    driveSet(138, -138);
    delay(150);
    driveStop();
  }

  driveSet(127, 127);
  timerReset(4);
  encoderReset(lencoder);
  while ((ultrasonicGet(sonic) > 60 || ultrasonicGet(sonic) == 0) &&
         timer(4) < (MAX_TIME_TO_WALL * sqrt(2) + 5) + 500 && encoderGet(lencoder) < (95 * INCHESMULTIPLIER))
    delay(10);
  driveStop();
  driveInchNoFix(9, 127);
  delay(50);

  liftTo(POTHALF);
  {
    turnToNoFix(-212, 90);
    driveSet(158, -158);
    delay(200);
    driveStop();
  }
  liftSet(LIFTZERO);

  timerReset(3);
  driveSet(-127, -127);
  delay(900);
  driveStop();
  calibrate();

  liftTo(POTTOP);
  delay(700);
  liftTo(POTBOTTOM);
  liftSet(-127);
  delay(50);
  liftSet(0);
  int lastLift = analogReadCalibrated(POT);
  while (true) {
    delay(400);
    if (analogReadCalibrated(POT) > lastLift - 15 && analogReadCalibrated(POT) < lastLift + 15) {
      liftTo(POTHALF);
      turnNoFix(-2.5, 65);
      delay(700);
      liftTo(POTTOP);
      liftTo(POTBOTTOM);
      lastLift = analogReadCalibrated(POT);
    } else  {
      break;
    }
  }

  driveStop();
  calibrate();
}
