#include <common.h>

u_int DECOMP_MixRNG_GetValue(int val)
{
  return val * 0x6255 + 0x3619U & 0xffff;
}
