#include "lib.h"

void initVars(void) {
  TURN_TOLERANCE = 3;
  blinker = 1;
  checknum = 0;
  lcdMode = 1;
  for (size_t i = 1; i < 22; i++) {
    startTimes[i] = 0;
  };
  if (USE_JINX) {
    initJINX(stdout);
    delay(100);
    taskCreate(JINXRun, TASK_DEFAULT_STACK_SIZE, NULL, (TASK_PRIORITY_DEFAULT));
    delay(100);
  };
  potMutex = mutexCreate();
  driveMutex = mutexCreate();
  isThisDone = mutexCreate();
  isThatDone = mutexCreate();
  leftarr = (int *)malloc(sizeof(int) * 3);
  rightarr = (int *)malloc(sizeof(int) * 3);
  passThis = (int *)malloc(sizeof(int) * 2);
  leftarr[FIX] = rightarr[FIX] = false;
}

void initSensors(void) {
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
}
