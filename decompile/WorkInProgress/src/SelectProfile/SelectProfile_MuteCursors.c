#include <common.h> 

void SelectProfile_MuteCursors(void)
{
  data.menuFourAdvProfiles.state |= MUTE_SOUND_OF_MOVING_CURSOR;

  data.menuGhostSelection.state |= MUTE_SOUND_OF_MOVING_CURSOR;

  data.menuWarning2.state |= MUTE_SOUND_OF_MOVING_CURSOR;
  return;
}
