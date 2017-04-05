#ifndef TASKS_H
#define TASKS_H
#include "lib.h"
#include "extras.h"

TaskHandle rDriveHandle;
TaskHandle motorsSafe;
TaskHandle showTime;
TaskHandle liftToHandle;
TaskHandle leftToHandle;
TaskHandle rightToHandle;

/**
 *Moves the left side of the drive to
 *the desired position at the desired power
 */
void leftTo(void *none);
/**
 *Moves the right side of the drive to
 *the desired position at the desired power
 */
void rightTo(void *none);
/*
 *A task that dumps quickly
 */
void quickDump(void *none);
void liftToTask(void *parameter);



void stopAllPeriodic(void);
void lcdDisplayTime(void *none);

#endif
