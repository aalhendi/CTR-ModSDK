#include <common.h>

void UI_CupStandings_FinalizeCupRanks(void)
{
  struct GameTracker *gGT;
  int totalNumCharacters;
  int trackRank;
  int i, j;
  short numEqualRanks;
  short bestRank;
  int numRanks, rankDriver;

  gGT = sdata->gGT;
  numEqualRanks = 0;

  // numPlyrCurrGame + number of AIs
  totalNumCharacters = gGT->numPlyrCurrGame + gGT->numBotsNextGame;

  // if there are more than four drivers, keep it at 4
  if (4 < totalNumCharacters) {
    totalNumCharacters = 4;
  }

  // if there is more than one driver
  if (1 < totalNumCharacters) {
    for (i = 1; i < totalNumCharacters;i++)
    {
	  // if current player cup points is the same as another player
      if ((sdata->gGT->cup.points[data.cupPositionPerPlayer[i]]) !=
          (gGT->cup.points[data.cupPositionPerPlayer[0]])) break;
      // add the number of driver with same rank
      numEqualRanks++;
    } 
  }

  for (i = 0; i < numEqualRanks + 1; i++)
  {
    // default rank at the most behind
      bestRank = 99;
      numRanks = -1;
      for(j = i; j < numEqualRanks + 1; j++)
	  {
		// get current driver rank for this track
        trackRank = gGT->drivers[data.cupPositionPerPlayer[j]]->driverRank;
        // if current driver rank is better (lower) than 99
        if (trackRank < bestRank)
		{
        // save rank of current driver
          bestRank = trackRank;
          numRanks = j;
        }
      }
      // get driverID from cup rank i
      rankDriver = data.cupPositionPerPlayer[i];
	  // set final cup rank for each driver
      data.cupPositionPerPlayer[i] = numRanks;
      data.cupPositionPerPlayer[numRanks] = rankDriver;
  }
  return;
}
