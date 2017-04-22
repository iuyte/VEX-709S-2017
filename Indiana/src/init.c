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
#include "revision.h"
#include "constants.h"


void initializeIO() {
  pinMode(isWall, INPUT);
  pinMode(isWall2, INPUT);
}

void initialize() {
  if (USE_JINX) {
    initJINX(stdout);
    delay(100);
    taskCreate(JINXRun, TASK_DEFAULT_STACK_SIZE, NULL, (TASK_PRIORITY_DEFAULT));
    delay(100);
  }

  for (size_t j = 1; j < 11; j++) {
    motorMutexes[j] = mutexCreate();
  }

  potMutex = mutexCreate();
  timerMutex = mutexCreate();
  driveMutex = mutexCreate();
  isThisDone = mutexCreate();
  isThatDone = mutexCreate();
  leftarr = (int *)malloc(sizeof(int) * 3);
  rightarr = (int *)malloc(sizeof(int) * 3);
  passThis = (int *)malloc(sizeof(int) * 2);
  leftarr[FIX] = rightarr[FIX] = false;
  //motorsSafe = taskRunLoop(stopAllPeriodic, 100);
  motorsHandle = taskCreate(motorSlewTask, TASK_DEFAULT_STACK_SIZE, NULL, TASK_PRIORITY_DEFAULT + 1);
  gyro=gyroInit(GYROPORT, 198);
  gyra=gyroInit(GYRAPORT, 197);
  lencoder = encoderInit(LENCPORT);
  rencoder = encoderInit(RENCPORT);
  sonic = ultrasonicInit(sonicECHO, sonicPING);
  encoderReset(lencoder);
  encoderReset(rencoder);
  gyroReset(gyro);
  analogCalibrate(POT);
  lcdInit(uart1);
  lcdClear(uart1);
  lcdSetBacklight(uart1, true);
  showTime = taskCreate(lcdDisplayTime, TASK_DEFAULT_STACK_SIZE, NULL, TASK_PRIORITY_DEFAULT - 1);
  setTeamName("709s");
}
