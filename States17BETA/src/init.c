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
  pinMode(HANGHOOK, OUTPUT);
  pinMode(HANGLOCKLEFT, OUTPUT);
  pinMode(HANGLOCKRIGHT, OUTPUT);
}

void initialize() {
  potMutex = mutexCreate();
  timerMutex = mutexCreate();
  //speakerInit();
  //TaskHandle lcdButtonsHandle = taskCreate(printButtons, TASK_DEFAULT_STACK_SIZE, NULL, TASK_PRIORITY_DEFAULT - 1);
  motorsSafe = taskRunLoop(stopAllPeriodic, 100);
  showTime = taskCreate(lcdDisplayTime, TASK_DEFAULT_STACK_SIZE, NULL, TASK_PRIORITY_DEFAULT - 1);
  gyro=gyroInit(GYROPORT, 0);
  lencoder = encoderInit(LENCPORT, true);
  rencoder = encoderInit(RENCPORT, false);
  encoderReset(lencoder);
  encoderReset(rencoder);
  gyroReset(gyro);
  analogCalibrate(POT);
  lcdInit(uart1);
  lcdClear(uart1);
  lcdSetBacklight(uart1, true);
  setTeamName("709s");
}
