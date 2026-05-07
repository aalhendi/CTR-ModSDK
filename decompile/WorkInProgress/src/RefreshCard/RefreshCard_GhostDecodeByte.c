#include <common.h>

int RefreshCard_GhostDecodeByte(char currByte)
{
  // only called from RefreshCard_GhostDecodeProfile

  if (currByte == 0x2d) return 0x3e;
  if (currByte == 0x5f) return 0x3f;
  if (currByte < 0x3a) return (short)(currByte - 0x30);
  if (0x5a < currByte) return (short)(currByte - 0x3d);

  return (short)(currByte - 0x37);
}
