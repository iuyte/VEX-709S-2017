#pragma once
#include "ethanlib.h"
#include "main.h"
#include <Math.h>
#define ENCO_MULTIPLIER .4 // 65
#define ENCO_TOL 10
#define VALUE 0
#define POWER 1
#define FIX 2

int *leftarr;
int *rightarr;

/*
 *Prints CHECK [checkNum] to the
  *debug stream
 */
void check(int checkNum);
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

/**
 *Moves each side of the drive independently with a
 *amount of customization
 *@param lpos the target position for the left side
 *@param rpos the target position for the right side
 *@param lpower the amount of power for left side
 *@param rpower the amount of power for the right side
 *@param correctionEnabled whether or not the robot
 *should correct it's position if it goes too far
 */
void driveToEncDep(int lpos, int rpos, int lpower, int rpower, bool correctionEnabled);

/*
 *Brings the lift to a
 *certain position (after a delay) by
 *starting a task
 */
void liftToAuto(long wait, int position);

/*
 *A revised and more successful version
 *of jerk()
 */
void revisedJerk();
