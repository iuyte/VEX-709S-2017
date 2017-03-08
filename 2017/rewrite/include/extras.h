#ifndef EXTRAS_H
#define EXTRAS_H

#include "lib.h"
#include "motors.h"

Gyro gyro;
Gyro gyra;
Ultrasonic sonic;
Encoder rencoder;
Encoder lencoder;

Mutex potMutex;
Mutex timerMutex;
Mutex driveMutex;
Mutex isThisDone;
Mutex isThatDone;

unsigned long startTimes[21];
long vars[7];
float TURN_TOLERANCE;
int blinker;
int checknum;
int *leftarr;
int *rightarr;
int *passThis;
int lcdMode;
int accelX;
int accelY;
int prevX;
int prevY;
typedef struct { int a; int b; } Foo;

void initVars(void);
void initSensors(void);

#endif
