#include "revision.h"

void auto14() {
  calibrate();
  jerk();     // This is intended to drop and lock the intake
  delay(400); // waits 440 milliseconds for the intake to drop (AND LOCK)
  // GO FORWARD & PICK UP 1-3 STARS (+ intake), BACK UP TO PREVIOUS POSITION

  /////////////BEGIN PART 0 (count from 0 in CS)/////////////

  driveInchNoFix(40, 127); // Drives 24

  rLiftTo(0, POTHALF);

  driveInch(14, 127); // 6

  int j = analogReadCalibrated(POT);
  while (j < POTHALF / 1.5) {
    j = analogReadCalibrated(POT);
    delay(1);
  }

  driveInch(-52, 127); // -43

  // TURN BACK OF ROBOT TO WALL, LIFT UP, HIT WALL, DUMP
  rTurn(90, 2, 90, true, true);

  checknum = 1;

  delay(5500);

  checknum = 2;

  rLiftTo(650, POTTOP); // 400

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

  while (analogReadCalibrated(POT) < POTTOP - 50)
    delay(20);
  checknum = 7;

  delay(600);

  liftTo((POTHALF + 200));
  checknum = 70;

  driveSet(100, 100);
  checknum = 8;
  delay(500);
  check(80);
  while (!isLine())
    delay(20);
  delay(100);
  while (!isLine())
    delay(20);
  driveStop();
  checknum = 85;

  delay(3250);

  checknum = 86;
  rLiftTo(580, POTTOP); // 200 500 600

  checknum = 9;
  timerReset(1);
  driveSet(-158, -158);
  while ((digitalRead(isWall) == 1 || digitalRead(isWall2) == 1) &&
         timer(1) < MAX_TIME_TO_WALL) { // Repeats until the button on the robot
                                        // is pressed
                                        // or 4 seconds pass
    delay(1);
  }

  driveStop(); // Stops the robot
  checknum = 10;

  while (analogReadCalibrated(POT) < POTTOP - 50)
    delay(20);

  delay(600);

  liftTo(POTHALF + 200);

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

  turnNoFix(-6, 40); // -3

  driveSet(100, -100);
  delay(50);
  driveStop();

  check(160);

  delay(3250);

  rLiftTo(550, POTTOP); // 200 350 600

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

  gyroReset(gyro);
  gyroReset(gyra);

  // rTurn(-90, 3, 50, true, true);
  // smartTurnTo(-90, 60);
  timerReset(4);
  while (rGyros() > -75) { // 73 77
    driveSet(-50, 50);
    delay(10);
  }
  driveSet(50, -50);
  delay(60);
  driveStop();

  check(22);

  liftTo(POTBOTTOM); //*/ rLiftTo(0, POTBOTTOM);
  // driveInchNoFix(-4, 127); // 12
  check(23);

  delay(500);
  driveInch(28, 127); // 22

  int startp = -86; // rGyros(); // 7 2
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
  startp = rGyros() - 7; // 4 6
  startEnc = encoderGet(lencoder);
  timerReset(3);
  while (encoderGet(lencoder) - startEnc < 23 * INCHESMULTIPLIER && timer(3) < (MAX_TIME_TO_WALL / 2.5) + 500) { // 25
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

  driveInch(4, 127); // 16 12 8 too far

  check(24);
  rLiftTo(0, POTHALF);
  delay(300);
  driveInch(-6, 100);

  while (analogReadCalibrated(POT) < POTHALF)
    delay(20);
  check(25);

  smartTurnTo(90, 60);

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
  //rLiftTo(0, POTBOTTOM);
  driveSet(-158, -158);
  timerReset(1);
  while ((digitalRead(isWall) == 1 || digitalRead(isWall2) == 1) &&
         timer(1) < 2000) { // Repeats until the button on the robot is pressed
                            // or 4 seconds pass
    delay(1);
  }
  driveStop();

  smartTurnTo(-5, 40); // -3

  startp = rGyros();
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

  while (analogReadCalibrated(POT) < (POTHALF * .75))
    delay(20);
  driveInch(-4, 127);
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

  ////////////////// BEGIN \\\\\\\\\\\\\\\\\\\
\\\\\\\\\\\\\\\\\ PART 2 ////////////////////

  rLiftTo(0, POTHALF - 50);
  timerReset(3);
  driveSet(-100, -100);
  while ((digitalRead(isWall) == 1 || digitalRead(isWall2) == 1) &&
         timer(3) < 1000) { // Repeats until the button on the robot is pressed
                            // or 1 seconds pass
    delay(5);
  }
  driveStop();
  check(50);
  calibrate();

  driveInch(3, 127);

  turnNoFix(77, 70);

  unsigned int count = 5;
  for (size_t newCount = 0; newCount < count; newCount++) {
    driveInch(16, 127);
    liftTo((POTHALF + 200));
    liftTo(POTHALF - 100);
  }

  driveStop();
  calibrate();
}
