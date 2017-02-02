#include "main.h"

void wheelMonitorTask(void *parameter){
    while(true){
        while(runWheels){
            if(abs(encoderGet(leftQuad)) < wheelTargetTicks){
                switch(wheelDir){
                    case FORWARD: dLeftDirection = false; break;
                    case BACKWARD: dLeftDirection = true; break;
                    case LEFT: dLeftDirection = true; break;
                    case RIGHT: dLeftDirection = false; break;
                }
                dLeft(dLeftDirection, false);
            }else{
                leftDone = true;
                stopLeft();
            }
            if(abs(encoderGet(rightQuad)) < wheelTargetTicks){
                switch(wheelDir){
                    case FORWARD: dRightDirection = false; break;
                    case BACKWARD: dRightDirection = true; break;
                    case LEFT: dRightDirection = false; break;
                    case RIGHT: dRightDirection = true; break;
                }
                dRight(dRightDirection, false);
            }else{
                rightDone = true;
                stopRight();
            }
            if(leftDone && rightDone)
              runWheels = false;
        }
        stopDrive();
    }
}

void setSyncMove(enum WheelDirection d, int targetTicks){
    wheelTargetTicks = targetTicks;
    wheelDir = d;
    leftDone = false;
    rightDone = false;
    zeroDriveSensors();
    runWheels = true;
}

void dLeft(bool backwards, bool bypassSlew){
    if(bypassSlew){
        motorSet(backLeft, backwards ?  -DRIVEBASE_POWER : DRIVEBASE_POWER);
        motorSet(frontLeft, backwards ?  -DRIVEBASE_POWER : DRIVEBASE_POWER);
        motorReq[backLeft - 1] = backwards ?  -DRIVEBASE_POWER : DRIVEBASE_POWER;
        motorReq[frontLeft - 1] = backwards ? -DRIVEBASE_POWER : DRIVEBASE_POWER;
    }else{
        motorReq[backLeft - 1] = backwards ?  -DRIVEBASE_POWER : DRIVEBASE_POWER;
        motorReq[frontLeft - 1] = backwards ? -DRIVEBASE_POWER : DRIVEBASE_POWER;
    }
}

void dRight(bool backwards, bool bypassSlew){
    if(bypassSlew){
        motorSet(backRight, backwards ? DRIVEBASE_POWER : -DRIVEBASE_POWER);
        motorSet(frontRight, backwards ?  DRIVEBASE_POWER : -DRIVEBASE_POWER);
        motorReq[backRight - 1] = backwards ?  DRIVEBASE_POWER : -DRIVEBASE_POWER;
        motorReq[frontRight - 1] = backwards ? DRIVEBASE_POWER : -DRIVEBASE_POWER;
    }else{
        motorReq[backRight - 1] = backwards ?  DRIVEBASE_POWER : -DRIVEBASE_POWER;
        motorReq[frontRight - 1] = backwards ? DRIVEBASE_POWER : -DRIVEBASE_POWER;
    }
}

void analogDrive(){
    motorReq[backRight - 1] = -joystickGetAnalog(1, 2);
    motorReq[frontRight - 1] = -joystickGetAnalog(1, 2);
    motorReq[backLeft - 1] = joystickGetAnalog(1, 3);
    motorReq[frontLeft - 1] = joystickGetAnalog(1, 3);
}

void strafeLeft(int millis){
    if(millis != 0){
        motorReq[backRight - 1] = -127;
        motorReq[frontRight - 1] = 127;
        motorReq[backLeft - 1] = 127;
        motorReq[frontLeft - 1] = -127;
        delay(millis);
        stopDrive();
    }else{
        motorReq[backRight - 1] = -127;
        motorReq[frontRight - 1] = 127;
        motorReq[backLeft - 1] = 127;
        motorReq[frontLeft - 1] = -127;
    }
}

void strafeRight(int millis){
    if(millis != 0){
        motorReq[backRight - 1] = 127;
        motorReq[frontRight - 1] = -127;
        motorReq[backLeft - 1] = -127;
        motorReq[frontLeft - 1] = 127;
        delay(millis);
        stopDrive();
    }else{
        motorReq[backRight - 1] = 127;
        motorReq[frontRight - 1] = -127;
        motorReq[backLeft - 1] = -127;
        motorReq[frontLeft - 1] = 127;
    }
}

void stopLeft(){
    if(isAutonomous()){
        motorStop(backLeft);
        motorStop(frontLeft);
        motorReq[backLeft - 1] = 0;
        motorReq[frontLeft - 1] = 0;
    }else{
        motorReq[backLeft - 1] = 0;
        motorReq[frontLeft - 1] = 0;
    }
}

void stopRight(){
    if(isAutonomous()){
        motorStop(backRight);
        motorStop(frontRight);
        motorReq[backRight - 1] = 0;
        motorReq[frontRight - 1] = 0;
    }else{
        motorReq[backRight - 1] = 0;
        motorReq[frontRight - 1] = 0;
    }
}

void stopDrive(){
    stopLeft();
    stopRight();
}
