#include <common.h>

int MixRNG_Particles(int seed)
{
  return (int)((RngDeadCoed(&sdata->gGT->deadcoed_struct.unk1) & 0xffff) * seed) >> 0x10;
}
