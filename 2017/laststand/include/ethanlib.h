#pragma once
#include "main.h"
/*
 *Sets the lift to a desired power
 */
void liftSet(int power);
/*
 *Sets the drive to a desired power
 */
void driveSet(int Lpower, int Rpower);
/*
 *brings the lift to a desired value
 */
void liftTo(int pos);
void liftTask(void *parameter);
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
Gyro gyro;
Encoder lencoder;
Encoder rencoder;
void stopAllPeriodic(void);
void turn(float degrees, int power);
void turnTo(float degrees, int power);
void liftToTask(void *parameter);
/*
 *Resets the given timer, 1-8
 */
void timerReset(int number);
/*
 *Gets the value of the givie timer, 1-8
 *Returns unsigned long
 */
unsigned long timer(int number);
unsigned long startTimes[12];
void lcdDisplayTime(void *parameter);
/*
 *The list of drive motors available
 *for use
 */
int driveMotorList[6];
/*
 *The listr of lift motors available
 *for use
 */
int liftMotorList[6];
void stopAfter(void *milliseconds);
void gyroResetAfter(void *milliseconds);
void stopDriveAfter(void *milliseconds);
/*
 *Stops the drive.
 *Equivelent of driveSet(0,0);
 */
void driveStop(void);
/*
 *Performs the jerk motion used to drop
 *the intake and fire the preload
 */
void jerk(void);
Mutex potMutex;
int *arr;
const char* uptown[3];
/*
 *Performs a software reset on
 *the Cortex
 *NOTE: static variables may survive
 */
void hard_reset(void);
void printButtons(void *parameter);
TaskHandle motorsSafe;
TaskHandle showTime;
TaskHandle liftToHandle;
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
float TURN_TOLERANCE;
int blinker;
int checknum;
TaskHandle calibrateHandle;
/*
 *Performs a reset on all three of
 *the gyro, left quad encoder, and the
 *right quad encoder
 */
void calibrate(void);
void calibrateTask(void *parameter);
/*
 *Prints a lot of values to the debug stream
 */
void printValues(void);
TaskHandle AutonHandle;
Mutex timerMutex;
Mutex driveMutex;
TaskHandle leftToHandle;
TaskHandle rightToHandle;
Mutex isThisDone;
Mutex isThatDone;
bool thisDoneYet;
bool thatDoneYet;
/*
 *Sends all relevant values
 *to JINX
 */
void sendValues(void);

Ultrasonic sonic;
int lcdMode;
Gyro gyra;
/*
 *Sets only the back 4 motors on the drive,
 *while driveSet uses all 6. Used mainly in
  *revised turning functions.
  */
void driveSetBack(int Lpower, int Rpower);

/*
 *Returns the average between
 *the two gyros
 */
int rGyros();
