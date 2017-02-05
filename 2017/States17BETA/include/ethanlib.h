<<<<<<< HEAD
#pragma once
#include "main.h"
int liftZero;
void liftSet(int power);
void driveSet(int Lpower, int Rpower);
void liftTo(int pos);
void liftTask(void *parameter);
void driveTo(float targetPosition, int power);
void driveInch(float inches, int power);
Gyro gyro;
Encoder lencoder;
Encoder rencoder;
void stopAllPeriodic();
void turn(float degrees, int power);
void turnTo(float degrees, int power);
void liftToTask(void *parameter);
void timerReset(int number);
unsigned long timer(int number);
unsigned long startTimes[12];
void lcdDisplayTime(void *parameter);
int driveMotorList[6];
int liftMotorList[6];
void stopAfter(void *milliseconds);
void gyroResetAfter(void *milliseconds);
void stopDriveAfter(void *milliseconds);
void driveStop();
void ideals(void *parameter);
bool useIdeals[2];
float systems[3];
float systems_pow[3];
void systemsReset();
Mutex mutex;
=======
#pragma once
#include "main.h"
int liftZero;
void liftSet(int power);
void driveSet(int Lpower, int Rpower);
void liftTo(int pos);
void liftTask(void *parameter);
void driveTo(float targetPosition, int power);
void driveInch(float inches, int power);
Gyro gyro;
Encoder lencoder;
Encoder rencoder;
void stopAllPeriodic();
void turn(float degrees, int power);
void turnTo(float degrees, int power);
void liftToTask(void *parameter);
void timerReset(int number);
unsigned long timer(int number);
unsigned long startTimes[12];
void lcdDisplayTime(void *parameter);
int driveMotorList[6];
int liftMotorList[6];
void stopAfter(void *milliseconds);
void gyroResetAfter(void *milliseconds);
void stopDriveAfter(void *milliseconds);
void driveStop();
void ideals(void *parameter);
bool useIdeals[2];
float systems[3];
float systems_pow[3];
void systemsReset();
Mutex mutex;
>>>>>>> e5141102db8af40aea4118a544d7d9175c9e9efe
