#ifndef RERUN_H
#define RERUN_H

#include "lib.h"

#define stopButton joystickGetDigital(1, 7, JOY_UP)
#define DRIVE_POWER 127
#define DRIVE_TO_TOLERANCE 5
#define RERUN_DELAY 10

TaskHandle rerunHandle;

bool goalReached[3];
bool isDriver(void);

typedef struct {
    int(*value)(void);
    int port;
} System;

System leftDrive;
System rightDrive;
System lift;

int liftGet(void);
int leftGet(void);
int rightGet(void);
void liftToRaw(int pos);

void liftToRawTask(void *position);
void leftToTask(void *position);
void rightToTask(void *position);
extern bool rerunEnabled;

void toPos(int lift, int leftDrive, int rightDrive);
void recordP(void *none);
void replayC(void);
void replayF(void);
void rerunInit(void);

#endif
