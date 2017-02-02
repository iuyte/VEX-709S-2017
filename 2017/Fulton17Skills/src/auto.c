#include "FunctionVars.h"

void autonomous() {
  driveInch(-4, 127);   // Drives 4 inches backward at 90 power
  driveInch(4, 127);    // Drives 4 inches forward at 90 power
  driveInch(24.5, 127); // Drives 25.5/28.5 inches forward at 63 power
  liftToTaskPos = potHalf;
  liftToTaskWait = 0;
  TaskHandle liftToooTaskk = taskCreate(liftToTask, TASK_DEFAULT_STACK_SIZE, NULL,
                                      TASK_PRIORITY_DEFAULT);
  driveInch(4.5, 127);
  delay(400);
  driveInch(-26, 127); // Drives 24/27 inches backward at 63 power
  turn(79, 100);       // Turns 83 degrees to the right at 40 power
  delay(6000);
  driveSet(-110, -127); // Sets the drive to go backwards at 70 power
  unsigned long startTime =
      millis(); // Sets a variable to the current time, stays at this
  unsigned long totalTime =
      millis() - startTime; // Sets a variable to the time since startTime
  while (digitalRead(isWall) == 1 &&
         totalTime < 4000) { // Repeats until the button on the robot is pressed
                             // or 4 seconds pass
    totalTime =
        millis() - startTime; // Sets a variable to the time since startTime
    delay(1);
    if (totalTime == 1000) {
      liftToTaskPos = potTop; // Brings the lift to the top
      TaskHandle lliftToTassssssskk = taskCreate(liftToTask, TASK_DEFAULT_STACK_SIZE,
                                           NULL, TASK_PRIORITY_DEFAULT);
    }
  }
  driveSet(0, 0); // Stops the robot
  ///*
  liftToTaskPos = potHalf; // Takes the lift down to halfway
  TaskHandle lliiftToTaskk = taskCreate(liftToTask, TASK_DEFAULT_STACK_SIZE, NULL, TASK_PRIORITY_DEFAULT);
  driveInch(36, 100);
  delay(6000);
  driveSet(-100, -100);
  startTime = millis();
  totalTime = millis() - startTime;
  bool blah = true;
  while (digitalRead(isWall) == 1 && digitalRead(isWall2) == 1) { // Repeats until the button on the robot is pressed or 4 seconds pass
    totalTime = millis() - startTime; // Sets a variable to the time since startTime
    if (totalTime > 1000 && blah) {
      liftToTaskPos = potTop; // Brings the lift to the top
      TaskHandle lliftToTaskk = taskCreate(liftToTask, TASK_DEFAULT_STACK_SIZE, NULL, TASK_PRIORITY_DEFAULT);
      blah=false;
    }
  }
    driveSet(0, 0);
    /*
    delay(1000);
    liftToTaskPos = potHalf; // Takes the lift down to halfway
    TaskHandle lliiftToTasskk = taskCreate(liftToTask, TASK_DEFAULT_STACK_SIZE, NULL, TASK_PRIORITY_DEFAULT);
    driveInch(36, 100);
    delay(4000);
    driveSet(-100, -100);
    startTime = millis();
    totalTime = millis() - startTime;
    while (digitalRead(isWall) == 1 && totalTime < 6000) { // Repeats until the button on the robot is pressed or 4 seconds pass
      totalTime = millis() - startTime; // Sets a variable to the time since startTime
      delay(1);
      if (totalTime == 800) {
        liftToTaskPos = potTop; // Brings the lift to the top
        TaskHandle lliftToTaaaaaskk = taskCreate(liftToTask, TASK_DEFAULT_STACK_SIZE, NULL, TASK_PRIORITY_DEFAULT);
      }
    }
      driveSet(0, 0);
      */
  delay(5000);                // Waits half of a second
  liftToTaskPos = potBottom; // Takes the lift down to the bottom
  TaskHandle lliiftTooTaskk = taskCreate(liftToTask, TASK_DEFAULT_STACK_SIZE,
                                        NULL, TASK_PRIORITY_DEFAULT);
  turn(-60, 100);     // Turns left 43/61/55 degrees at 40 power
  driveInch(29, 127); // Drives 34 inches forward
  liftToTaskPos = potHalf;
  TaskHandle lliftTToTaskk = taskCreate(liftToTask, TASK_DEFAULT_STACK_SIZE, NULL, TASK_PRIORITY_DEFAULT);
  driveInch(3.5, 127);
  driveSet(-127, -127);
  startTime = millis();             // Sets a starting time variable
  totalTime = millis() - startTime; // gets the time since startTime is set
  while (digitalRead(isWall) == 1 &&
         totalTime < 3000) { // Repeats until the bumper sensor on the robot is
                             // pressed or 3 seconds pass
    totalTime = millis() - startTime; // gets the time since startTime is set
  }
  driveSet(0, 0); // Stops the drive
  driveSet(127, -127);
  delay(250);
  liftToTaskPos = potTop; // Brings the lift to the top
  TaskHandle lliftTToTaaskk = taskCreate(liftToTask, TASK_DEFAULT_STACK_SIZE,
                                         NULL, TASK_PRIORITY_DEFAULT);
  driveSet(0, 0);
  gyroShutdown(gyro);
}
