#include <common.h>

void CalculateVolumeFromDistance(u_int *soundCount_ID, u_int soundID, int distance);

void PlayWarppadSound(u_int distance)
{
    CalculateVolumeFromDistance(&sdata_static.SoundFadeInput[0].soundID_soundCount, 152, distance);
}
