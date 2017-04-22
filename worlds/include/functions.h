#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include "lib.h"

void liftTo(int pos);
void driveTo(float targetPosition, int power);
void driveToNoFix(float targetPosition, int power);
/*
 *Drives a specific number of inches
 */
void driveInch(float inches, int power);
/*
 *Drives to the specified amount of inches,
 *but does not correct if it goes over (unlike
 *driveInch)
  */
void driveInchNoFix(float inches, int power);
/*
 *Resets the given timer, 1-8
 */
void timerReset(int number);
/*
 *Gets the value of the givie timer, 1-8
 *Returns unsigned long
 */
unsigned long timer(int number);
/*
 *Performs the jerk motion used to drop
 *the intake and fire the preload
 */
void jerk(void);

void auto0();
void auto1();
void auto2();
void auto3();
void auto4();
void auto6();

void sendValues(void);
void printValues(void);

void turnNoFix(float degrees, int power);
void driveInchAbs(float inches, int power);
void driveToAbs(float targetPosition, int power);
/*
 *A function used to turn to a certain
 *relative position in degrees
 */
void smartTurn(float degrees, int power);
/*
 *A function used to turn to a certain
 *absolute position in degrees,
 *where absolute is from the last time
 *gyroReset(gyro); was called
 */
void smartTurnTo(float degrees, int power);
/*
 *Used to determine whether
 *or not a line is detected by the
 *line sensor
 */
bool isLine(void);

/*
 *Performs a reset on all three of
 *the gyro, left quad encoder, and the
 *right quad encoder
 */
void calibrate(void);

/*
 *Returns the average between
 *the two gyros
 */
int rGyros();

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
 *Dumps quickly and effectively from the wall
 */
void fastDumpFromWall();

/*
 *Prints CHECK [checkNum] to the
  *debug stream
 */
void check(int checkNum);

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
 *Waits until the lift is at a certain position
 */
void wLift(int position);

/*
 *Brakes the drive for a specified amount of time
 */
void brake(unsigned int time);

void turn(float degrees, int power);

void turnToNoFix(float degrees, int power);

#endif
