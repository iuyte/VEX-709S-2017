#pragma once
#include "main.h"

int motorSlew[11];
int motorReq[11];

TaskHandle motorsHandle;
Mutex motorMutexes[11];

void motorSlewTask(void *none);
/*
 *Set the desired motor to the designated power
 *NOTE: runs through the motorSlewTask
 */
void motorRek(int motorPort, int power);
/*
 *Gets the power the motor was last set to
 */
int getMotor(int motorPort);

typedef struct {
  unsigned char port;
  int power;
  char inverted;
} Motor;
