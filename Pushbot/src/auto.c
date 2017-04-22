/** @file auto.c
 * @brief File for autonomous code
 *
 * This file should contain the user autonomous() function and any functions
 * related to it.
 *
 * Any copyright is dedicated to the Public Domain.
 * http://creativecommons.org/publicdomain/zero/1.0/
 *
 * PROS contains FreeRTOS (http://www.freertos.org) whose source code may be
 * obtained from http://sourceforge.net/projects/freertos/files/ or on request.
 */

#include "lib.h"
#include "constants.h"

void autonomous() {
  if (analogRead(chooser) < chooserPosOne) {
    while (abs(analogReadCalibrated(rightLine)) < whiteLine ||
           abs(analogReadCalibrated(leftLine)) < whiteLine) {
      if (abs(analogReadCalibratedHR(rightLine)) > whiteLine) {
        motorSet(rightDrive, -10);
      } else {
        motorSet(rightDrive, 127);
      }
      if (abs(analogReadCalibratedHR(leftLine)) > whiteLine) {
        motorSet(leftDrive, -10);
      } else {
        motorSet(leftDrive, 127);
      }
      delay(10);
    }
    motorSet(leftDrive, 0);
    motorSet(rightDrive, 0);
  } else if (analogRead(chooser) > chooserPosThree) {
    TaskHandle LeftUntilLine = taskCreate(leftUntilLine, TASK_DEFAULT_STACK_SIZE, NULL, TASK_PRIORITY_DEFAULT);
    TaskHandle RightUntilLine = taskCreate(rightUntilLine, TASK_DEFAULT_STACK_SIZE, NULL, TASK_PRIORITY_DEFAULT);
  } else {
  }
}
