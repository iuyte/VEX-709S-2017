#include "ethanlib.h"
#include "constants.h"

void auto0() {
  useIdeals[LIFT] = true;
  useIdeals[DRIVE] = true;
  systems[LEFT_DRIVE] = 24 * inchesMultiplier;
  systems[RIGHT_DRIVE] = 24 * inchesMultiplier;
  systems[LIFT] = potHalf;
}

void auto1() {
  gyroReset(gyro);
  useIdeals[LIFT] = true;
  useIdeals[DRIVE] = true;
  encoderReset(lencoder);
  encoderReset(rencoder);
  driveInch(-4, 100);//Drives 4 inches backward
  driveInch(4, 100);//Drives 4 inches forward
  // The previous code is intended to drop the intake
  delay(400);
  driveInch(28, 127);//Drives 28 inches forward at 63 power
  systems[LIFT] = potHalf;
  driveInch(5, 127);
  delay(400);
  driveInch(-30, 127);//Drives 24/27 inches backward at 63 power
  turn(77, 100);//Turns 83 degrees to the right at 40 power
  useIdeals[DRIVE] = false;
  TaskHandle gyroResetIn = taskCreate(gyroResetAfter, TASK_DEFAULT_STACK_SIZE, (void*)1000, TASK_PRIORITY_DEFAULT) ;
  while (digitalRead(isWall) == 1 && digitalRead(isWall2) == 1 && timer(0) < 4000) {//Repeats until the button on the robot is pressed or 4 seconds pass
    systems[LEFT_DRIVE] = encoderGet(lencoder);
    systems[RIGHT_DRIVE] = encoderGet(rencoder);
    delay(5);
  }
  driveStop();
  useIdeals[DRIVE] = true;
  delay(500);//Waits half of a second
  systems[LIFT] = potBottom;//Takes the lift down to the bottom
  delay(1500);
  useIdeals[LIFT] = false;
  liftSet(-50);
  delay(250);
  liftSet(0);
  useIdeals[LIFT] = true;
  turnTo(-53, 100);//Turns left 43/61/55 degrees at 40 power
  driveInch(34, 127);
  systems[LIFT] = potHalf + 200;
  driveInch(6, 127);
  delay(250);
  driveInch(-12, 127);
  turn(-15, 100);
  driveSet(-127, -127);
  useIdeals[DRIVE] = false;
  timerReset(0);
  while (digitalRead(isWall) == 1 && digitalRead(isWall2) == 1 && timer(0) < 2000) {//Repeats until the bumper sensor on the robot is pressed or 3 seconds pass
    delay(1);
  }
  driveStop();
  useIdeals[DRIVE] = true;
  driveInch(12, 127);
  turn(77, 100);
  delay(333);
  systems[LIFT] = potTop;
  useIdeals[DRIVE] = false;
  timerReset(0);
  while (digitalRead(isWall) == 1 && digitalRead(isWall2) == 1 && timer(0) < 2000) {//Repeats until the bumper sensor on the robot is pressed or 3 seconds pass
    delay(1);
  }
  driveStop();
  useIdeals[DRIVE] = true;
  gyroReset(gyro);
  encoderReset(lencoder);
  encoderReset(rencoder);
}

void autonomous() {
  FILE *chooser;
  bool op2;
  if ((chooser = fopen("autoM", "r")) == NULL) {
    op2 = false;
  } else if (fgetc(chooser)){
    op2 = true;
  } else {
    op2 = false;
  }
  fclose(chooser);
  if (op2) {
    auto1();
  } else {
    auto0();
  }
}
