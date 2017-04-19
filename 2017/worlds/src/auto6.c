#include "lib.h"

void auto6() {
  calibrate();
  jerk();     // This is intended to drop and lock the intake
  // GO FORWARD & PICK UP 1-3 STARS (+ preload), BACK UP TO PREVIOUS POSITION

  /////////////BEGIN PART 0 (count from 0 in CS)/////////////

  driveInchNoFix(38, 127); // Drives 40

  rLiftTo(0, POTHALF);

  driveInch(14, 127); // 6

  int j = analogReadCalibrated(POT);
  while (j < POTHALF / 1.5) {
    j = analogReadCalibrated(POT);
    delay(1);
  }

  driveInch(-51, 127); // -43

  // TURN BACK OF ROBOT TO WALL, LIFT UP, HIT WALL, DUMP
  {
    turnToNoFix(85, 80);
    driveSet(-158, 158);
    delay(240);
    driveStop();
  }

  checknum = 1;

  delay(4500);

  checknum = 2;

  rLiftTo(800, POTTOP); // 650 550 600 675 735

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
  checknum = 10;

  while (analogReadCalibrated(POT) < POTTOP - 50)
    delay(20);

  delay(600);

  liftTo(POTHALF + 600);

  driveSet(100, 100);
  checknum = 100;
  delay(800);
  check(150);
  while (!isLine())
    delay(20);
  delay(100);
  while (!isLine())
    delay(20);
  driveStop();
  checknum = 155;

  turnNoFix(-1, 40); // 6

  driveSet(100, -100);
  delay(50);
  driveStop();

  check(160);

  delay(1250);

  rLiftTo(470, POTTOP); // 550

  driveSet(-158, -158);
  timerReset(1);
  while ((digitalRead(isWall) == 1 || digitalRead(isWall2) == 1) &&
         timer(1) < MAX_TIME_TO_WALL) { // Repeats until the button on the robot
                                        // is pressed
                                        // or 4 seconds pass
    delay(1);
  }

  driveStop(); // Stops the robot

  /////////////BEGIN PART 1 (count from 0 in CS)/////////////

  check(20);

  mutexTake(potMutex, -1);
  mutexGive(potMutex);
  delay(600);
  liftTo(POTHALF + 100);
  check(21);

  driveSet(-158, -158);
  timerReset(1);
  while ((digitalRead(isWall) == 1 || digitalRead(isWall2) == 1) &&
         timer(1) < 1300) { // Repeats until the button on the robot is pressed
                            // or 4 seconds pass
    delay(1);
  }
  driveStop();
  check(215);

  calibrate();

  if (timer(1) < 1300) {
    driveInchNoFix(2, 127);

    driveSet(55, -55);
    delay(75);
    driveStop();

    driveSet(-158, -158);
    timerReset(1);
    while ((digitalRead(isWall) == 1 || digitalRead(isWall2) == 1) &&
           timer(1) <
               1300) { // Repeats until the button on the robot is pressed
                       // or 4 seconds pass
      delay(1);
    }
    driveStop();
  }

  timerReset(4);
  while (rGyros() > -73) {
    driveSet(-50, 50);
    delay(10);
  }
  driveSet(50, -50);
  delay(85);
  driveStop();

  check(22);

  liftTo(POTBOTTOM);

  check(23);

  delay(500);
  driveInch(28, 127);

  int startp = -86;
  int startEnc = encoderGet(lencoder);
  long power = 110;
  long lpower = power;
  long rpower = power;
  int tolerance = 2;
  float changer;
  while (encoderGet(lencoder) - startEnc < 45 * INCHESMULTIPLIER) { // 45
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
  startp = rGyros() - 8; // 7
  startEnc = encoderGet(lencoder);
  timerReset(3);
  while (encoderGet(lencoder) - startEnc < 15 * INCHESMULTIPLIER &&
         timer(3) < (MAX_TIME_TO_WALL / 3) + 750) { // 23 25
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

  driveInch(6, 127);

  turnNoFix(2, 70);

  check(24);

  mutexGive(potMutex);
  rLiftTo(0, POTHALF + 10);
  delay(300);
  driveInch(-5, 100);

  int stp = analogReadCalibrated(POT) + 10;
  while (analogReadCalibrated(POT) < POTHALF) {
    delay(75);
    if (analogReadCalibrated(POT) <= stp) {
      int jj = getMotor(ORL);
      liftSet(0);
      driveInchNoFix(-0.5, 80);
      liftSet(jj);
      delay(75);
    }
  }
  check(25);

  smartTurn(90, 65);

  TaskHandle dumpHandle = taskCreate(quickDump, TASK_DEFAULT_STACK_SIZE, NULL,
                                     TASK_PRIORITY_DEFAULT);
  delay(100);
  while (!mutexTake(isThisDone, 5))
    delay(20);
  taskDelete(dumpHandle);
  mutexGive(isThisDone);
  delay(600);
  mutexGive(potMutex);
  liftTo(POTBOTTOM); // + 10
  check(40);

  mutexGive(potMutex);

  driveSet(-158, -158);
  timerReset(1);
  while ((digitalRead(isWall) == 1 || digitalRead(isWall2) == 1) &&
         timer(1) < 2000) { // Repeats until the button on the robot is pressed
                            // or 4 seconds pass
    delay(1);
  }
  driveStop();

  smartTurnTo(-3, 65); // -3, 40 45 too low

  startp = rGyros() + 2;
  power = 110;
  lpower = power;
  rpower = power;
  tolerance = 2;
  timerReset(4);
  while ((ultrasonicGet(sonic) > 20 || ultrasonicGet(sonic) == 0) &&
         timer(4) < MAX_TIME_TO_WALL - 1500) {
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

  long prev = timer(4);
  if (prev > MAX_TIME_TO_WALL)
    driveInch(-5, 127);

  rLiftTo(0, POTHALF + 50);

  if (prev < MAX_TIME_TO_WALL - 500)
    driveInch(3, 127);
  check(41);

  stp = analogReadCalibrated(POT);
  while (analogReadCalibrated(POT) < POTHALF) {
    delay(75);
    if (analogReadCalibrated(POT) <= stp) {
      int jj = getMotor(ORL);
      liftSet(0);
      driveInchNoFix(-0.75, 90);
      liftSet(jj);
      delay(75);
    }
  }

  driveInchNoFix(-4, 127);
  turnNoFix(-2, 70);
  check(42);

  while (analogReadCalibrated(POT) < POTHALF)
    delay(20);
  check(425);

  delay(600);

  driveSet(-158, -158);
  timerReset(1);
  while ((digitalRead(isWall) == 1 || digitalRead(isWall2) == 1) &&
         timer(1) <
             MAX_TIME_TO_WALL -
                 1700) { // Repeats until the button on the robot is pressed
                         // or 4 seconds pass
    delay(1);
  }

  check(43);

  fastDumpFromWall();

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
  while (analogReadCalibrated(POT) < POTTOP - 52)
    delay(50);
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
      liftTo(POTTOP);
      delay(800);
      liftTo(POTBOTTOM);
      lastLift = analogReadCalibrated(POT);
    } else  {
      break;
    }
  }

  driveStop();
  calibrate();
  while (isAutonomous())
    delay(50);
}
