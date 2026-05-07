#include <common.h>

u_int RefreshCard_GhostEncodeByte(int currByte)
{
  // only called from RefreshCard_GhostEncodeProfile

  u_int uVar1;
  short sVar2;

  sVar2 = (short)currByte;
  if (sVar2 < 10)
  {
    return currByte + 0x30U & 0xff;
  }
  if (sVar2 < 0x24)
  {
    return currByte + 0x37U & 0xff;
  }
  if (sVar2 < 0x3e)
  {
    uVar1 = currByte + 0x3dU & 0xff;
  }
  else
  {
    uVar1 = 0x2d;
    if (sVar2 != 0x3e)
    {
      return 0x5f;
    }
  }
  return uVar1;
}
