#include <common.h>

void howl_VolumeSet(int type, unsigned char vol);
void howl_ModeSet(int newMode);

void RaceConfig_LoadGameOptions(void)
{
    struct GameOptions *options;
    char i;

    if (sdata->boolHasLoadedOptions == false)
    {
        sdata->boolHasLoadedOptions = true;
        options = &sdata->gameOptions;

        // loop 3 times (fx, music, voice)
        for (i = 0; i < 3; i++)
        {
            howl_VolumeSet(i, ((u_char*)&options->volFx)[i * 2]);
        }

        // copy 24 bytes (4 structs) of RWD from sdata to data
        for (i = 0; i < 4; i++)
        {
            data.rwd[i].gamepadCenter = options->rwd[i].gamepadCenter;
            data.rwd[i].deadZone = options->rwd[i].deadZone;
            data.rwd[i].range = options->rwd[i].range;
        }

        sdata->gGT->gameMode1 |= options->gameMode1_0xf00 & 0xf00;

        howl_ModeSet(*(u_char*)&options->audioMode & 1);
    }
}
