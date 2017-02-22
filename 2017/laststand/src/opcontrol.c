/** @file opcontrol.c
 * @brief File for operator control code
 *
 * This file should contain the user operatorControl() function and any
 * functions related to it.
 *
 * Any copyright is dedicated to the Public Domain.
 * http://creativecommons.org/publicdomain/zero/1.0/
 *
 * PROS contains FreeRTOS (http://www.freertos.org) whose source code may be
 * obtained from http://sourceforge.net/projects/freertos/files/ or on request.
 */

#include "ethanlib.h"
#include "constants.h"

void operatorControl() {

  bool HANGHOOKSet = true;
  /*
  for (size_t i = 0; i < 6; i++) {
    motorSet(driveMotorList[i], 100);
    delay(500);
    motorSet(driveMotorList[i], 0);
    delay(1000);
  }
  driveSet(100, 100);
  delay(1000);
  driveSet(0, 0);
  //*/
  //driveSet(100, 100);
  //TaskHandle stopTask = taskCreate(stopDriveAfter, TASK_DEFAULT_STACK_SIZE, (void*)1000, TASK_PRIORITY_DEFAULT + 1);
  while (isEnabled()) {
    if (joystickGetDigital(1, 6, JOY_UP) && analogReadCalibrated(POT) < POTTOP) {
      liftSet(127);
    } else if (joystickGetDigital(1, 6, JOY_DOWN) && analogReadCalibrated(POT) > POTBOTTOM) {
      liftSet(-127);
    } else {
      liftSet(LIFTZERO);
    }
    /*
    if (analogReadCalibrated(POT) < POTBOTTOM) {
      liftSet(50);
    }
    */

    if (joystickGetDigital(1, 8, JOY_RIGHT)) {
      arr[0] = 0;
      arr[1] = POTTOP;
      liftToHandle = taskCreate(liftToTask, TASK_DEFAULT_STACK_SIZE,
                                          (void *)arr, TASK_PRIORITY_DEFAULT);
      driveInch(5, 127);
      timerReset(0);
      driveSet(-127, -127);
      while ((digitalRead(isWall) == 1 || digitalRead(isWall2) == 1) &&
             timer(0) < 1000) { // Repeats until the either bumper sensor on the robot is
                                // pressed or 1 second passes
        delay(5);
      }
      driveStop();
      mutexTake(potMutex, -1);
      int j = analogReadCalibrated(POT);
      mutexGive(potMutex);
      while(j < POTTOP) {
        mutexTake(potMutex, 5);
        j = analogReadCalibrated(POT);
        mutexGive(potMutex);
        delay(1);
      }
    }

    driveSet(joystickGetAnalog(1, 3), joystickGetAnalog(1, 2));
    // if (lcdReadButtons(uart1) == 7) taskSuspend(NULL);
    delay(20);
	}
}
