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

#include "lib.h"

void operatorControl() {
  while (isEnabled()) {
    if (joystickGetDigital(1, 6, JOY_UP) &&
        analogReadCalibrated(POT) < POTTOP) {
      liftSet(127);
    } else if (joystickGetDigital(1, 6, JOY_DOWN) &&
               analogReadCalibrated(POT) > POTBOTTOM) {
      liftSet(-127);
    } else {
      liftSet(LIFTZERO);
    }
    /*
    if (analogReadCalibrated(POT) < POTBOTTOM) {
      liftSet(50);
    }
    */

    if (joystickGetDigital(1, 8, JOY_RIGHT) && false) {
      TaskHandle dumpHandle = taskCreate(quickDump, TASK_DEFAULT_STACK_SIZE, NULL, TASK_PRIORITY_DEFAULT);
      delay(100);
      while (!joystickGetDigital(1, 8, JOY_RIGHT) && !mutexTake(isThisDone, 5)) delay (20);
      taskDelete(dumpHandle);
    }

    driveSet(joystickGetAnalog(1, 3), joystickGetAnalog(1, 2));

    if (lcdReadButtons(uart1) == 2) {
      if (lcdMode == 1) {
        lcdMode = 2;
      } else {
        lcdMode = 1;
      }
      delay(200);
    } else if (lcdReadButtons(uart1) == 5) {
      calibrate();
    }
    delay(20);
  }
}
