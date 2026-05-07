#include <common.h>

int RefreshCard_BoolGhostForLEV(u_short trackID)
{
  int numGhost = 0;

  // if ghost profiles are present
  if (sdata->numGhostProfilesSaved > 0)
  {
    // loop through ghost profiles
    for (int i = 0; i < sdata->numGhostProfilesSaved; i++)
    {
      // Check trackID of each profile
      if (sdata->ghostProfile_memcard[i].trackID == trackID)
      {
        numGhost++;
      }
    }
  }
  return numGhost;
}
