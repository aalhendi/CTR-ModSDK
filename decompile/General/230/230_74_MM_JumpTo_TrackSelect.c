#include <common.h>

void MM_JumpTo_TrackSelect(void)
{
  // return to track selection
  sdata->ptrActiveMenuBox = &OVR_230.menubox_trackSelect;

  OVR_230.menubox_trackSelect.state &= ~(ONLY_DRAW_TITLE);

  MM_TrackSelect_Init();
}