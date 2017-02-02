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
#include "constants.h"

void operatorControl() {
  while (isEnabled()) {
    driveSet(joystickGetAnalog(1, 3), joystickGetAnalog(1, 2));
    lcdPrint(uart1, 1, "Line: %d", analogReadCalibrated(leftLine));
    lcdPrint(uart1, 2, "Enc: %d", encoderGet(encoder));
    delay(75);
  }
  motorStopAll();
}
