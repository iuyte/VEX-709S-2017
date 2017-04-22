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


void initializeIO() {
  pinMode(isWall, INPUT);
  pinMode(isWall2, INPUT);
}

void initialize() {
  initMotors();
  initVars();
  initSensors();
  showTime = taskCreate(lcdDisplayTime, TASK_DEFAULT_STACK_SIZE, NULL, TASK_PRIORITY_DEFAULT - 1);
  setTeamName("709s");
}
