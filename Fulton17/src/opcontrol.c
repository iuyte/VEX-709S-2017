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

#include "FunctionVars.h"

void operatorControl() {
  bool hangHookSet = true;
  bool prev = joystickGetDigital(1, 7, JOY_DOWN);
  //TaskHandle taskOfLift = taskCreate(liftTask, TASK_DEFAULT_STACK_SIZE, NULL, TASK_PRIORITY_DEFAULT);
  while (isEnabled()) {
    prev = joystickGetDigital(1, 7, JOY_DOWN);
    ///*
    if (joystickGetDigital(1, 6, JOY_UP) && analogReadCalibrated(pot) < potTop) {
      liftSet(100);
    } else if (joystickGetDigital(1, 6, JOY_DOWN) && analogReadCalibrated(pot) > 1) {
      liftSet(-100);
    } else {
      liftSet(liftZero);
    }
    //*/
    if (joystickGetDigital(1, 5, JOY_UP)) {
      digitalWrite(hangLockLeft, true);
    } else if (joystickGetDigital(1, 5, JOY_DOWN)) {
      digitalWrite(hangLockLeft, false);
      delay(75);
    }
    if (joystickGetDigital(1, 7, JOY_DOWN) && joystickGetDigital(1, 7, JOY_DOWN) != prev) {
      if (hangHookSet == true) {
        hangHookSet = false;
      } else {
        hangHookSet = true;
      }
      digitalWrite(hangHook, hangHookSet);
      delay(75);
    }
    driveSet(joystickGetAnalog(1, 3), joystickGetAnalog(1, 2));
		//lcdPrint(uart2, 1, "%d", analogReadCalibrated(pot));
    //lcdPrint(uart2, 2, "%d", gyroGet(gyro));
    //delay(100);
	}
}
