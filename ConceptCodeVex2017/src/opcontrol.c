#include "main.h"

extern Gyro gyro;
extern Ultrasonic fsonar;

//digital ports//
//pnuematic lift lock ports:
const int liftlock1 = 1;
const int liftlock2 = 2;
const int hanglock = 3;//pnuematic hang lock port:
const int led = 4;//led port:
//analog ports//
const int pot = 1;//potentiometer port, an analog port
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
void motorSet ( unsigned char channel, int speed);
int joystickGetAnalog (unsigned char joystick, unsigned char axis );//get joystick value allong an axis
bool joystickGetDigital (unsigned char joystick , unsigned char buttonGroup, unsigned char button);//get a button value: JOY_LEFT, JOY_RIGHT, JOY_UP, JOY_DOWN
void liftLock(bool on) { //lock the lift
	digitalWrite(liftlock1, on);
	digitalWrite(liftlock2, on);
};
void ledSet(bool on) {//turn led on or off, true for on, false for off
  digitalWrite(led, on);
};
void lift(void * parameter) {
	int pow = 0;
	while(true) {
    while (joystickGetDigital(1,6,JOY_UP)) {
			motorSet(OLL,liftspeed);
			motorSet(TIRLBIRL,liftspeed);
			motorSet(ORL,-liftspeed);
			motorSet(TILLBILL,-liftspeed);
			potdat = analogRead(pot);
		};
		while (joystickGetDigital(1,6,JOY_DOWN)) {
			motorSet(OLL,-liftspeed);
			motorSet(TIRLBIRL,-liftspeed);
			motorSet(ORL,liftspeed);
			motorSet(TILLBILL,liftspeed);
			potdat = analogRead(pot);
		};
		pow = 0;
		while(joystickGetDigital(1,6,JOY_UP)  == false && joystickGetDigital(1,6,JOY_DOWN) == false) {
      if (analogRead(pot) < potdat) {
				pow = pow + 1;
      }
      else if(analogRead(pot) > potdat) {
				pow = pow - 1;
      motorSet(OLL,80);
      motorSet(ORL,-80);
      motorSet(TILLBILL,-80);
      motorSet(TIRLBIRL,80);
			};
		};
	};
};
void drive() {
  motorSet(TLD, -1 * joystickGetAnalog(1, 3));
  motorSet(BLD, -1 * joystickGetAnalog(1, 3));
  motorSet(TRD, joystickGetAnalog(1, 2));
  motorSet(BRD, joystickGetAnalog(1, 2));
};
void buttons(void * parameter) {
  while(1) {
    drive();
    if (joystickGetDigital(1, 5, JOY_DOWN)) {
      liftLock(false);
    }
    else if (joystickGetDigital(1, 5, JOY_UP)) {
      liftLock(true);
    };
    if (joystickGetDigital(1, 7, JOY_DOWN)) {
      digitalWrite(hanglock, false);
    }
    else if (joystickGetDigital(1, 7, JOY_UP)) {
      digitalWrite(hanglock, true);
    };
  };
};

void operatorControl() {
  TaskHandle firstTaskHandle = taskCreate(buttons, TASK_DEFAULT_STACK_SIZE, NULL, TASK_PRIORITY_DEFAULT);
  TaskHandle secondTaskHandle = taskCreate(lift, TASK_DEFAULT_STACK_SIZE, NULL, TASK_PRIORITY_DEFAULT);
};
