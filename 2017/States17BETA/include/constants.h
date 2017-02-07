#pragma once

#define OLL 2      // Outside Left Lift
#define TILLBILL 4 // Top Inside Left Lift and Bottom Inside Left Lift (y-cables to power expander)
#define ORL 9     // Outside Right Lift
#define TIRLBIRL 7 // Top Inside Right Lift and Bottom Inside Right Lift
                  // (y-cables to power expander)
#define liftZero 17
#define LIFT 0
// DRIVE//
#define TLD 3 // Top Left Drive
#define MLD 5 // Middle Left Drive
#define BLD 10 // Back Left Drive
#define TRD 8 // Top Right Drive
#define MRD 6 // Middle Right Drive
#define BRD 1 // Back Right Drive
#define DRIVE 1
// DIGITAL INPUT//
#define lencPort 8  // Left encoder top port is 8 (means that bottom port is 9)
#define rencPort 11 // Right encoder top port is 11(means that bottom port is 12)
#define isWall 1    // Wall bumper port is digital 1
#define isWall2 10
// DIGITAL OUTPUT//
#define hangLockLeft 3  // Left Hang Lock Pnuematic
#define hangLockRight 2 // Right Hang Lock Pnuematic
#define hangHook 4      // Hook for hanging pneumatic
// ANALOG INPUT//
#define pot 4 // Potentiometer on lift in analog port 4
#define potTop 2000
#define potBottom 10 // when the potentiometer is all the way down, this is it's position
#define potHalf 550 // When the lift is halfway up
#define gyroPort 2  // Sets the gyroscope port
#define isLine 3    // Sets the port for the line sensor
#define lineLight 2000; // Sets the point where a line is white  if it's less than this
#define NUMBER_OF_AUTON 5
#define TURN_CORRECTION 2.7
#define ENCO_CORRECTION 3
#define DEFAULT_AUTON 1
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
// Although it has been shortened for pruposes relating to ease of use
#define inchesMultiplier 17.90493109783822527399942337940786572887671014580135048412544009
