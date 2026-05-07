#include <common.h>

int EngineSound_VolumeAdjust(int desired, int current, int amount)
{
  int gap = desired - current;
  int absGap = (gap < 0) ? -gap : gap;
  
  // amount less than absolute gap between
  // "current" and "desired"
  if (amount < absGap) {
    return (gap < 1) ? current - amount : current + amount;
  }

  return desired;
}
