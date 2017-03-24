#include "rerun.h"

bool isDriver(void) {return (isEnabled() && !isAutonomous());}

void toPos(int lift, int leftDrive, int rightDrive) {
  goalReached[0] = goalReached[1] = goalReached[2] = false;
  liftToRawTask((void *)lift);
  leftToTask((void *)leftDrive);
  rightToTask((void *)rightDrive);
  while (!(goalReached[0] && goalReached[1] && goalReached[2]))
    delay(5);
}

void rerunInit(void) {
  lift.value = liftGet;
  leftDrive.value = leftGet;
  leftDrive.port = TLD;
  rightDrive.port = TRD;
  rerunHandle = taskCreate(recordP, TASK_DEFAULT_STACK_SIZE, NULL, TASK_PRIORITY_DEFAULT);
}

void recordP(void *none) {
  FILE *rerun;
  for (size_t lines = 0; lines < 10; lines++) {
    print("\n");
  }
  while (!isDriver())
    delay(5);
  while ((rerun = fopen("rerun", "w")) == NULL)
    ;

  while (!stopButton && isDriver()) {
    fprintf(rerun, "%d %d %d\n", lift.value(), leftDrive.value(), rightDrive.value());
    printf("toPos(%d, %d, %d);\n\r", lift.value(), leftDrive.value(), rightDrive.value());
    delay(RERUN_DELAY);
  }
  fclose(rerun);
}

void replayF(void) {
  FILE *rerun;
  int liftR;
  int leftDriveR;
  int rightDriveR;
  while ((rerun = fopen("rerun", "r")) == NULL)
    ;
  while (!isAutonomous())
    delay(5);

  fseek(rerun, 0, SEEK_SET);
  while (isAutonomous()) {
    liftR = fgetc(rerun);
    fseek(rerun, 2, SEEK_CUR);
    leftDriveR = fgetc(rerun);
    fseek(rerun, 2, SEEK_CUR);
    rightDriveR = fgetc(rerun);
    fseek(rerun, 1, SEEK_CUR);
    toPos(liftR, leftDriveR, rightDriveR);
  }
}

void replayC(void) {
  toPos(0, 0, 0);
}
