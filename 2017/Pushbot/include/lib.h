#include "main.h"

void hard_reset();
void leftUntilLine(void *parameter);
void rightUntilLine(void *parameter);
void timerReset();
unsigned long timer();
void driveSet(int left, int right);
void liftSet(int power);
void liftTo(void *parameter);
void stopAllPeriodic();
void turn(float degrees, int power);
