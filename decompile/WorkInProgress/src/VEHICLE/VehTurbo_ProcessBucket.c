#include <common.h>

void VehTurbo_ProcessBucket(struct Thread* turboThread) {
  u_short temp2;
  u_int temp4;
  struct Instance *driverInstance;
  struct Instance *inst;
  struct Turbo *turbo;

  if (turboThread == NULL) return;

  struct GameTracker *gGT = sdata->gGT;

  do {
    inst = turboThread->inst;
    turbo = turboThread->object;
    driverInstance = turbo->driver->instSelf;

    int numPlyr = gGT->numPlyrCurrGame;

    if (numPlyr) {
      struct InstDrawPerPlayer *idpp = INST_GETIDPP(inst);
      struct InstDrawPerPlayer *driveridpp = INST_GETIDPP(driverInstance);
      struct InstDrawPerPlayer *turboIdpp = INST_GETIDPP(turbo->inst);

      int i = 0;

      // each InstDrawPerPlayer
      do {
        temp4 = driveridpp[i].instFlags;
        if ((temp4 & PUSHBUFFER_EXISTS) == 0) {
          temp4 |= ~(DRAW_SUCCESSFUL);
          turboIdpp[i].instFlags &= temp4;
          idpp[i].instFlags &= temp4;

          temp4 = driveridpp[i].unkE4;
          idpp[i].unkE4 = temp4;
          turboIdpp[i].unkE4 = temp4;

          temp4 = driveridpp[i].unkE8;
          idpp[i].unkE8 = temp4;
          turboIdpp[i].unkE8 = temp4;

          temp2 = driveridpp[i].depthOffset[0];
          idpp[i].depthOffset[0] = temp2;
          turboIdpp[i].depthOffset[0] = temp2;

          temp2 = driveridpp[i].depthOffset[1];
          idpp[i].depthOffset[1] = temp2;
          turboIdpp[i].depthOffset[1] = temp2;
        }
        i++;
      } while (i < numPlyr);
    }
    // go to next thread
    turboThread = turboThread->siblingThread;
  } while (turboThread != NULL);
}
