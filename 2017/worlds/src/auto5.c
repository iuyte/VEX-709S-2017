#include "lib.h"

void auto5() {
  calibrate();
  jerk();// This is intended to drop and lock the intake
  delay(400);// waits 440 milliseconds for the intake to drop (AND LOCK)
  //GO FORWARD & PICK UP 1-3 STARS (+ intake), BACK UP TO PREVIOUS POSITION
  driveInchNoFix(14, 127); // Drives 24
  rLiftTo(0, POTHALF);
  driveInch(13, 127); // 6
  wLift(POTHALF / 1.5);
  driveInch(-12, 127);  // Drives 38 inches backward at 63 power
  //TURN BACK OF ROBOT TO WALL< LIFT UP, HIT WALL, DUMP
  //turnNoFix(2, 127);        // Turns 83 degrees to the right at 40 power
  //turn(88, 55);
  smartTurn(90, 100);
  driveSet(-127, -127); // Sets the drive to go backwards at 70 power
  timerReset(0);
  rLiftTo(600, POTTOP + 50);
  while (digitalRead(isWall) == 1 && digitalRead(isWall2) == 1 &&
         timer(0) < 4000) { // Repeats until the button on the robot is pressed
                            // or 4 seconds pass
    delay(1);
  }
  driveSet(0, 0); // Stops he robot
  gyroReset(gyro);
  gyroReset(gyra);
  wLift(POTTOP);
  delay(600);
  //LIFT TO BOTTOM, TURN & DRIVE FORWARD (GET CUBE) LIFT UP
  rLiftTo(0, POTBOTTOM);
  delay(1500);
  liftSet(-52);
  delay(250);
  liftSet(0);
  smartTurnTo(-39, 127); // -50
  driveInchNoFix(18, 127); // 25
  rLiftTo(600, POTHALF + 200);
  driveInch(10, 127);
  //
  delay(250);
  turnNoFix(20, 70);
  driveSet(-127, -127);
  timerReset(0);
  rLiftTo(0, POTTOP);
  while (digitalRead(isWall) == 1 && digitalRead(isWall2) == 1 &&
         timer(0) < 2000) { // Repeats until the bumper sensor on the robot is
                            // pressed or 3 seconds pass
    delay(1);
  }
  driveStop();
  calibrate();
}
