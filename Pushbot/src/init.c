/** @file init.c
 * @brief File for initialization code
 *
 * This file should contain the user initialize() function and any functions
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

void initializeIO() {}

void initialize() {
  //gyro = gyroInit(GYRO_PORT, 0);
  encoder = encoderInit(encoderPort, encoderPort + 1, false);
  analogCalibrate(leftLine);
  analogCalibrate(rightLine);
  speakerInit();
  lcdInit(uart1);
  lcdClear(uart1);
  lcdSetBacklight(uart1, true);
  setTeamName("Team 709s");
  TaskHandle periodicStop = taskRunLoop(stopAllPeriodic, 50);
}
