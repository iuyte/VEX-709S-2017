#include "main.h"

extern Gyro gyro;
extern Encoder leftQuad;
extern Encoder rightQuad;
extern Ultrasonic fsonar;

//digital ports//
//pnuematic lift lock ports:
const int liftlock1 = 1;
const int liftlock2 = 2;
const int hanglock = 3;//pnuematic hang lock port:
const int led = 4;//led port:
//analog ports//
const int pot = 1;//potentiometer port, an analog port
int acc = 8;
int xacc = analogReadCalibratedHR(acc);
double potdat;
//motor ports//
const int OLL = 2;//Outside Left Lift
const int TLD = 3;//Top Left Drive, which should be inverted
const int TILLBILL = 4;//Top Indide Left Lift and Bottom Inside Left Lift, which should be inverted
const int BLD = 5;//Back Left Drive, which should be inverted
const int BRD = 6;//Back Right Drive
const int TIRLBIRL = 7;//Top Inside Right Lift and Bottom Indide Right Lift
const int TRD = 8;//Top Right Drive
const int ORL = 9;//Outside Right Lift, which should be inverted
const int liftspeed = 80;
//FUNCTIONS//
bool goal = false;
int gs = gyroGet(gyro);
int g;
int lp;
int rp;
void motorSet ( unsigned char channel, int speed);
void driveSet(int leftSpeed, int rightSpeed) {
  motorSet(TLD, -leftSpeed);
  motorSet(BLD, -leftSpeed);
  motorSet(TRD, rightSpeed);
  motorSet(BRD, rightSpeed);
};
void liftLock(bool on) { //lock the lift
	digitalWrite(liftlock1, on);
	digitalWrite(liftlock2, on);
};
void ledSet(bool on) {//turn led on or off, true for on, false for off
  digitalWrite(led, on);
};
void driveStraight(void * parameter) {
  gs = gyroGet(gyro);
  lp = 70;
  rp = 70;
  while (!goal) {
    g = abs(gyroGet(gyro) - gs);
    if (g < 0) {
      rp = rp - 1;
      lp = lp + 1;
    }
    else if (g > 0) {
      rp = rp + 1;
      lp = lp - 1;
    };
    driveSet(lp,rp);
  };
};
bool noCube() {
  if (ultrasonicGet(fsonar) > 3 || ultrasonicGet(fsonar) != 0) {
    return true;
  }
  else {
    return false;
  };
};


void auton() {
  int tlp = 0;
  int trp= 0;
  TaskHandle ds = taskCreate(driveStraight, TASK_DEFAULT_STACK_SIZE, NULL, TASK_PRIORITY_DEFAULT);
  while (noCube()) {
      delay(1);
  };
  taskDelete(ds);
  gyroReset(gyro);
  while (gyroGet(gyro) < 30) {
    driveSet(-70,70);
  driveSet(0,0);
  xacc = analogReadCalibratedHR(acc);
  while (xacc < 100) {
    
  };

  };
};

void autonomous() {
  auton();
};
