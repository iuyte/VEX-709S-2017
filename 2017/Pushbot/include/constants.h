#include "main.h"

#define leftDrive 2
#define llift 3
#define rlift 4
#define encoderPort 1
#define rightDrive 9
#define leftLine 1
#define rightLine 2
#define whiteLine 500
#define chooser 3
#define GYRO_PORT 8
#define sonicin 4
#define sonicout 5
#define chooserPosOne 1700
#define chooserPosThree 3000
Gyro gyro;
Encoder encoder;
/*
const char* mario[] = {
        "Super Mario:d=4,o=5,b=100:"
                "16e6,16e6,32p,8e6,16c6,8e6,8g6,8p,8g,8p,"
                "8c6,16p,8g,16p,8e,16p,8a,8b,16a#,8a,16g.,16e6,16g6,8a6,16f6,8g6,8e6,16c6,16d6,8b,16p,"
                "8c6,16p,8g,16p,8e,16p,8a,8b,16a#,8a,16g.,16e6,16g6,8a6,16f6,8g6,8e6,16c6,16d6,8b,16p,"
                "8p,16g6,16f#6,16f6,16d#6,16p,16e6,16p,16g#,16a,16c6,16p,16a,16c6,16d6,"
                "8p,16g6,16f#6,16f6,16d#6,16p,16e6,16p,16c7,16p,16c7,16c7,8p.,"
                "8p,16g6,16f#6,16f6,16d#6,16p,16e6,16p,16g#,16a,16c6,16p,16a,16c6,16d6,"
                "8p,16d#6,8p,16d6,8p,16c6",
        "Super Mario:d=4,o=4,b=100:"
                "16d,16d,32p,8d,16d,8d,8g5,8p,8g,8p,"
                "8g5,16p,8e,16p,8c,16p,8f,8g,16f#,8f,16e.,16c5,16e5,8f5,16d5,8e5,8c5,16a,16b,8g,16p,"
                "8g5,16p,8e,16p,8c,16p,8f,8g,16f#,8f,16e.,16c5,16e5,8f5,16d5,8e5,8c5,16a,16b,8g,16p,"
                "8c,16p,16g,8p,8c5,8f,16p,16c5,16c5,16c5,8f,"
                "8c,16p,16e,8p,16g,16c5,p.,8g,"
                "8c,16p,16g,8p,8c5,8f,16p,16c5,16c5,16c5,8f,"
                "8c,16g#,8p,16a#,8p,16c5",
        NULL
};

const char* uptown[] =
{"UptownGi:d=16,o=6,b=125:8e7,8p,8e7,8p,8e7,4p,8d#7,8e7,f#7,p,e7,p,d#7,p,8c#7,8c#7,p,8b,8g#,8b,4p,a,p,g#,p,a,p,b,p,a,p,g#,p,f#,p", "UptownGi:d=8,o=6,b=125:4e,4e,2e,4p,d#,e,f#,e,d#,c#,4c#,b5,g#5,2b5,4p,a5,g#5,a5,b5,a5,g#5,f#5,4e5,4e,2e,4p,e,d#,e,f#,e,d#,c#,c#,b5,b5,g#5,2b5,p,a5,g#5,a5,b5", "UptownGi:d=4,o=5,b=140:c6,c6,c.6,p,8c6,8b,8c6,8d6,8c6,8b,8a,a,g,g.,p,8f,8e,8f,8g,8f,8e,8d,c,c6,d.6,p,8c6,8b,8c6,8e6,8d6,8c6,8b,a,g,g.,p,8f,8e,8f,g,8a,8b,c6,c6,c.6"}; 


const char* xfiles[] =
{"Xfiles:d=4,o=5,b=125:e,b,a,b,d6,2b.,1p,e,b,a,b,e6,2b.,1p,g6,f#6,e6,d6,e6,2b.,1p,g6,f#6,e6,d6,f#6,2b.,1p,e,b,a,b,d6,2b.,1p,e,b,a,b,e6,2b.,1p,e6,2b,",
NULL, NULL};
const char* mi[] =
{"MissionImp:d=16,o=6,b=95:32d,32d#,32d,32d#,32d,32d#,32d,32d#,32d,32d,32d#,32e,32f,32f#,32g,g,8p,g,8p,a#,p,c7,p,g,8p,g,8p,f,p,f#,p,g,8p,g,8p,a#,p,c7,p,g,8p,g,8p,f,p,f#,p,a#,g,2d,32p,a#,g,2c#,32p,a#,g,2c,a#5,8c,2p,32p,a#5,g5,2f#,32p,a#5,g5,2f,32p,a#5,g5,2e,d#,8d",
NULL, NULL};
const char* star[] =
{"StarWars:d=4,o=5,b=45:32p,32f#,32f#,32f#,8b.,8f#.6,32e6,32d#6,32c#6,8b.6,16f#.6,32e6,32d#6,32c#6,8b.6,16f#.6,32e6,32d#6,32e6,8c#.6,32f#,32f#,32f#,8b.,8f#.6,32e6,32d#6,32c#6,8b.6,16f#.6,32e6,32d#6,32c#6,8b.6,16f#.6,32e6,32d#6,32e6,8c#6",
NULL, NULL};
const char* enterta[] =
{"Entertainer:d=4,o=5,b=140:8d,8d#,8e,c6,8e,c6,8e,2c.6,8c6,8d6,8d#6,8e6,8c6,8d6,e6,8b,d6,2c6,p,8d,8d#,8e,c6,8e,c6,8e,2c.6,8p,8a,8g,8f#,8a,8c6,e6,8d6,8c6,8a,2d6",
NULL, NULL};
const char* bond[] =
{"Bond:d=4,o=5,b=80:32p,16c#6,32d#6,32d#6,16d#6,8d#6,16c#6,16c#6,16c#6,16c#6,32e6,32e6,16e6,8e6,16d#6,16d#6,16d#6,16c#6,32d#6,32d#6,16d#6,8d#6,16c#6,16c#6,16c#6,16c#6,32e6,32e6,16e6,8e6,16d#6,16d6,16c#6,16c#7,c.7,16g#6,16f#6,g#.6",
NULL, NULL};
*/
