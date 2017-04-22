#include "ethanlib.h"
#include "constants.h"
#include "revision.h"

void auto0() {
  taskSuspend(motorsHandle);
  motorSet(BRD, 100);
  motorSet(BLD, 100);
  delay(1000);
  motorStopAll();
  taskResume(motorsHandle);
}
