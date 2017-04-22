#pragma once
#include "ethanlib.h"
#include "motorSlew.h"
#include "constants.h"
#define ENCO_MULTIPLIER .4 // 65
#define ENCO_TOL 10
#define VALUE 0
#define POWER 1
#define TOLERANCE 0
#define FIX 2

int *leftarr;
int *rightarr;
int *passThis;
TaskHandle rDriveHandle;

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
void rLiftTo(long wait, int position);

/*
 *A revised and more successful version
 *of jerk()
 */
void revisedJerk();

/*
 *Returns the truespeed value for motors.
 *Based on Jess and Jordan's data (Jordan's funtion)
 */
short trueSpeed(short speed);

/*
 *Returns the sign of a number.
 *ex: sgn(-5) = -1 and sgn(5) = 1
 *If the number is 0, it returns 0
 */
float sgn(float num);

/*
 *The revised version of all my turn functions.
 *NOTE: uses both sensors and will not stop
 *until it is in the range of the specified value
 */
void rTurn(int degrees, int tolerance, int power, bool isAbsolute, bool useAll6);

/*
 *A task that dumps quickly
 */
void quickDump(void *none);

/*
 *Dumps quickly and effectively from the wall
 */
void fastDumpFromWall();

/*
 *A task that drives straight until it is taskDeleted.
 *Use rDriveSet instead
 */
void driveStraightUntil(void *none);

/*
 *A function that starts the robot driving straight
 *Use rDriveStop() to stop it
 */
void rDriveSet(int power, int tolerance);

/*
 *Used to stop rDriveSet.
 *WARNING: only use if rDriveSet was used previously
 *without being rDriveStop-ed,
 *or undefined behavior might occur
 */
 void rDriveStop(void);

 /*
 struct {
   bool done(void) {
     if (millis() >= ms) return true;
     else return false;
   };
   unsigned long ms;
   void set(unsigned long milliseconds) {
     ms = millis() + milliseconds;
   };
 } alarm;
 //*/
