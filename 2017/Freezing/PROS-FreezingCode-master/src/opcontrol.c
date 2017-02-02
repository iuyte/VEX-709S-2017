#include "main.h"

void operatorControl() {
	autonSelection = programSelected(8);
	taskDelete(wheelMonitorHandle);
	taskDelete(liftMonitorHandle);

	while (1) {

		if(joystickGetDigital(1, 6, JOY_UP)){
			dLift(false);
		}else if(joystickGetDigital(1, 6, JOY_DOWN)){
			dLift(true);
		}else{
			stopLift();
		}

		if(joystickGetDigital(1, 5, JOY_DOWN)){
			closeClaw(OFF);
		}else if(joystickGetDigital(1, 5, JOY_UP)){
			openClaw();
		}

		analogDrive();
		delay(2);
	}
}
