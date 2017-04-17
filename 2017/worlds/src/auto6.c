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
    delay(260);
    driveStop();
  }

  checknum = 1;

  delay(4500);

  checknum = 2;

  rLiftTo(735, POTTOP); // 650 550 600 675

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

  turnNoFix(-6, 40); // 6

  driveSet(100, -100);
  delay(50);
  driveStop();

  check(160);

  delay(1250);

  rLiftTo(400, POTTOP); // 550

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
  while (rGyros() > -75) {
    driveSet(-50, 50);
    delay(10);
  }
  driveSet(50, -50);
  delay(60);
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
  while (encoderGet(lencoder) - startEnc < 25 * INCHESMULTIPLIER &&
         timer(3) < (MAX_TIME_TO_WALL / 3) + 750) { // 23
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

  driveInch(8, 127);

  turnNoFix(3, 60);

  check(24);

  rLiftTo(0, POTHALF);
  delay(300);
  driveInch(-5, 100);

  int stp = analogReadCalibrated(POT);
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

  smartTurnTo(0, 60);

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

  smartTurnTo(-8, 50); // -3, 40 45 too low

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

  turnNoFix(4, 60);

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
  check(42);

  while (analogReadCalibrated(POT) < POTHALF)
    delay(20);
  check(425);

  delay(600);

  turnNoFix(2, 60);

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

  smartTurn(73, 60); // 67 68 70 72 too little

  driveStop();

  delay(250);

  driveInchNoFix(43, 127); // 38
  rLiftTo(0, POTHALF);
  driveInchNoFix(11, 150);
  driveStop();

  turnNoFix(-10, 80);

  rLiftTo(400, POTTOP);
  delay(450);
  timerReset(3);
  driveSet(-127, -127);
  while ((digitalRead(isWall) == 1 || digitalRead(isWall2) == 1) &&
         timer(3) < 1500) { // Repeats until the button on the robot is pressed
                            // or 1 seconds pass
    delay(5);
  }
  driveStop();

  liftTo(POTHALF);
  driveInchNoFix(.5, 127);
  turnNoFix(-30, 60); // 20
  rLiftTo(0, POTTOP);
  delay(200);

  timerReset(3);
  driveSet(-127, -127);
  while ((digitalRead(isWall) == 1 || digitalRead(isWall2) == 1) &&
         timer(3) < 750) { // Repeats until the button on the robot is pressed
                           // or 1 seconds pass
    delay(5);
  }
  driveStop();

  rliftTo(0, POTHALF);
  driveInchNoFix(6, 127);
  driveInchNoFix(-6, 127);

  liftTo(POTHALF);

  driveInchNoFix(.5, 127);
  turnNoFix(-30, 60); // 20
  rLiftTo(0, POTTOP);
  delay(200);

  timerReset(3);
  driveSet(-127, -127);
  while ((digitalRead(isWall) == 1 || digitalRead(isWall2) == 1) &&
         timer(3) < 750) { // Repeats until the button on the robot is pressed
                           // or 1 seconds pass
    delay(5);
  }
  driveStop();
  calibrate();

  {
    turnToNoFix(-40, 80);
    driveSet(138, -138);
    delay(260);
    driveStop();
  }
  driveInch(30, 127);

  liftSet(30);
  {
    turnToNoFix(-126, 80);
    driveSet(158, -158);
    delay(260);
    driveStop();
  }
  liftSet(LIFTZERO);

  timerReset(3);
  driveSet(-127, -127);
  while ((digitalRead(isWall) == 1 || digitalRead(isWall2) == 1) &&
         timer(3) < 750) { // Repeats until the button on the robot is pressed
                           // or 1 seconds pass
    delay(5);
  }
  driveStop();
  calibrate();

  driveInch(12, 110);
  {
    turnToNoFix(-40, 80);
    driveSet(138, -138);
    delay(260);
    driveStop();
  }

  liftTo(POTTOP);
  delay(400);
  liftTo(POTBOTTOM);


  driveStop();
  calibrate();
}
