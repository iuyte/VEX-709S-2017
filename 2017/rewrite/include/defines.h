#ifndef DEFINES_H
#define DEFINES_H

#include "lib.h"
#include <Math.h>

#define ENCO_MULTIPLIER .4 // 65
#define ENCO_TOL 10
#define VALUE 0
#define POWER 1
#define TOLERANCE 0
#define FIX 2

#define OLL 2      // Outside Left Lift
#define TILLBILL 4 // Top Inside Left Lift and Bottom Inside Left Lift (y-cables to power expander)
#define ORL 9     // Outside Right Lift
#define TIRLBIRL 7 // Top Inside Right Lift and Bottom Inside Right Lift
                  // (y-cables to power expander)
#define LIFTZERO 17
#define LIFT 0
#define LIFT_CAP 0.8
// DRIVE//
#define TLD 3 // Top Left Drive
#define MLD 5 // Middle Left Drive
#define BLD 10 // Back Left Drive
#define TRD 8 // Top Right Drive
#define MRD 6 // Middle Right Drive
#define BRD 1 // Back Right Drive
#define DRIVE 1
#define DRIVE_CAP 0.8
// DIGITAL INPUT//
#define LENCPORT 8, 9, false  // Left encoder top port is 8 (means that bottom port is 9)
#define RENCPORT 6, 7, false // Right encoder top port is 11(means that bottom port is 12)
#define isWall 5    // Wall bumper port is digital 1
#define isWall2 4
// ANALOG INPUT//
#define POT 4 // Potentiometer on lift in analog port 4
#define POTTOP 2000
#define POTBOTTOM 50 // when the POTentiometer is all the way down, this is it's position  // 10
#define POTHALF 650 // When the lift is halfway up // 800
#define GYROPORT 2  // Sets the gyroscope ports
#define GYRAPORT 5
#define LINE 1    // Sets the port for the line sensor
#define LINE2 3
#define LINELIGHT 2850 // Sets the point where a line is white  if it's less than this
#define sonicPING 3
#define sonicECHO 2
#define SONICGET ultrasonicGet(sonic)

//////////////////////////////////////////////////////
#define NUMBER_OF_AUTON 6
#define TURN_CORRECTION (1.2 - ((double)powerLevelMain()) / 30000)
#define SMART_TURN_MULT 0.67
#define ENCO_CORRECTION 3
#define DEFAULT_AUTON 0 // Skills is 4, reg is 7, onlycube is 1
#define MAX_TIME_TO_WALL 3500

#define MOTOR_DEFAULT_SLEW_RATE 40
#define MOTOR_FAST_SLEW_RATE 256

#define USE_JINX false
//Lots of math to get the next value:
// Encoders are on a 5/8 gear ratio of actual rotation
// This means that there are 225 degrees in a rotation
// The diameter of the current omni wheels is 4", therefore 4*pi =
// 12.56637061435917295385057353311801153678867759750042328389
// This means that after 225 degrees rotated,
// 12.56637061435917295385057353311801153678867759750042328389 inches are
// traveled
// To interpret this, for every degree rotated, the robot travels forward
// apporoxamately 0.05585053606381854646155810459163560683017190043333521459507
// inches for every degree rotated (This is about 21/376 inches, which is about
// 1/18 of an inch), meaning that because 1 divided by ^THAT NUMBER^
// is equal to
// 17.90493109783822527399942337940786572887671014580135048412544009,
// which is the number of degrees necessary to be traveled for 1 inch.
#define INCHESMULTIPLIER 17.90493109783822527399942337940786572887671014580135048412544009

#endif
