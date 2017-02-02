#include "main.h"

void clawMonitorTask(void *parameter){
    fingerNeedsToOpen = (digitalRead(leftFingerSwitchPort) == 1 || digitalRead(rightFingerSwitchPort) == 1);
    while(true){
        if(downPressure == true && runFinger == false){
            motorSet(fingerY, 30);
        }else if(downPressure == false && runFinger == false){
            motorSet(fingerY, -5);
        }else if(runFinger == true){
            fingerNeedsToOpen = (digitalRead(leftFingerSwitchPort) == 1 || digitalRead(rightFingerSwitchPort) == 1);
        }
        while(runFinger && fingerNeedsToOpen){
            motorSet(fingerY, -127);
            fingerNeedsToOpen = (digitalRead(leftFingerSwitchPort) == 1 || digitalRead(rightFingerSwitchPort) == 1);
            if(fingerNeedsToOpen == false || (joystickGetDigital(1, 5, JOY_DOWN) && !isAutonomous())){
                runFinger = false;
            }
        }
    }
}

void closeClaw(int millis){
    if(millis != 0){
        motorSet(fingerY, CLAW_POWER);
        taskDelay(millis);
        motorStop(fingerY);
    }else{
        motorSet(fingerY, CLAW_POWER);
    }
    downPressure = true;
}

void openClaw(){
    motorSet(fingerY, -CLAW_POWER);
    downPressure = false;
    fingerNeedsToOpen = true;
    runFinger = true;
}
