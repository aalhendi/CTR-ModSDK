#include <common.h>

void SelectProfile_ToggleMode(u_int mode)
{
  short *modeData = (short*)sdata->data10_bbb;

  *(short*)&sdata->memcardAction = mode & 0xf;

  // 0x00 - AdvNew (garage) mode
  // 0x10 - AdvLoad (main menu) mode
  // 0x20 - Green mode
  // 0x30 - Ghost mode
  // 0x40 - SlotSelected (exiting) mode
  modeData[0] = mode & 0xf0;

  modeData[2] = 0;
  modeData[3] = 0;
  modeData[4] = 0;
  modeData[5] = 0;
  modeData[6] = 0;

  SelectProfile_UnMuteCursors();

  data.menuFourAdvProfiles.drawStyle = data.menuFourAdvProfiles.drawStyle & 0xffef;
  data.menuOverwriteAdv.drawStyle &= 0xffef;
  if (modeData[0] == 0x20) {
    data.menuFourAdvProfiles.drawStyle = data.menuFourAdvProfiles.drawStyle | 0x10;
    data.menuOverwriteAdv.drawStyle |= 0x10;
  }

  // SelectProfile_Init
  SelectProfile_Init(data.menuFourAdvProfiles.drawStyle);

  data.menuFourAdvProfiles.rowSelected = sdata->unk_8008d73C_relatedToRowHighlighted;
  modeData[1] = 0;
}
