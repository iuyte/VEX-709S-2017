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
#include "ethanlib.h"
#include "constants.h"

void initializeIO() {
  pinMode(isWall, INPUT);
  pinMode(isWall2, INPUT);
  pinMode(hangHook, OUTPUT);
  pinMode(hangLockLeft, OUTPUT);
  pinMode(hangLockRight, OUTPUT);
}

void initialize() {
  potMutex = mutexCreate();
  speakerInit();
  TaskHandle motorsSafe = taskRunLoop(stopAllPeriodic, 333);
  TaskHandle showTime = taskCreate(lcdDisplayTime, TASK_DEFAULT_STACK_SIZE, NULL, TASK_PRIORITY_DEFAULT);
  gyro=gyroInit(gyroPort, 0);
  lencoder = encoderInit(lencPort, lencPort+1, true);
  rencoder = encoderInit(rencPort, rencPort+1, false);
  encoderReset(lencoder);
  encoderReset(rencoder);
  analogCalibrate(pot);
  analogCalibrate(isLine);
  lcdInit(uart1);
  lcdClear(uart1);
  lcdSetBacklight(uart1, true);
  setTeamName("709s");
}
