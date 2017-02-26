#include "motorSlew.h"
#include "constants.h"

void motorSlewTask(void *none){
    int motorIndex;
    int motorPort;
    int motorTmp;

    for(motorIndex=1;motorIndex<MOTOR_NUM;motorIndex++)
    {
        motorReq[motorIndex] = 0;
        motorSlew[motorIndex] = MOTOR_DEFAULT_SLEW_RATE;
    }

    while( true )
    {
        int requestCopy[10];

        for(int i = 0; i < 10; i++){
            requestCopy[i] = motorReq[i];
        }
        for( motorIndex=1; motorIndex<MOTOR_NUM; motorIndex++)
        {
            motorPort = motorIndex;
            mutexTake(motorMutexes[motorIndex], 100);
            motorTmp = motorGet(motorPort);
            mutexGive(motorMutexes[motorIndex]);
            if( motorTmp != requestCopy[motorIndex] )
            {

                if( requestCopy[motorIndex] > motorTmp )
                {

                    motorTmp += motorSlew[motorIndex];

                    if( motorTmp > requestCopy[motorIndex] )
                    motorTmp = requestCopy[motorIndex];
                }

                if( requestCopy[motorIndex] < motorTmp )
                {
                    motorTmp -= motorSlew[motorIndex];

                    if( motorTmp < requestCopy[motorIndex] )
                    motorTmp = requestCopy[motorIndex];
                }

                mutexTake(motorMutexes[motorIndex], 100);
                motorSet(motorPort, motorTmp);
                mutexGive(motorMutexes[motorIndex]);
            }
        }
        delay( MOTOR_TASK_DELAY );
    }
}

void motorRek(int motorPort, int power) {
  motorReq[motorPort] = power;
}
