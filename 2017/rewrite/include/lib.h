#include "mtrmgr.h"

typedef struct {
    Motor TLD;
    Motor MLD;
    Motor BLD;
    Motor TRD;
    Motor MRD;
    Motor BRD;
    void go(int lpower, int rpower);
} Drive;
