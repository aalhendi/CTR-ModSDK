#include <common.h>

void MainInit_RainBuffer(struct GameTracker *gGT)
{
    int i;
    u_char numPlyr;
    int* src;
    int* dst;
    struct RainBuffer *levelBuffer;
    struct RainBuffer *globalBuffer;

    numPlyr = gGT->numPlyrCurrGame;

    // if numPlyrCurrGame is not zero
    if (numPlyr)
    {
        // copy rain buffer from level, to player's global rain buffer
        levelBuffer = &gGT->level1->rainBuffer;
        
        for (i = 0; i < numPlyr; i++)
        {
            // Rain Buffer
            globalBuffer = &gGT->rainBuffer[i];

            src = (int*)levelBuffer;
            dst = (int*)globalBuffer;

            while(src != (int*)((int)levelBuffer + sizeof(struct RainBuffer)))
            {  
                dst[0] = src[0];
                dst[1] = src[1];
                dst[2] = src[2];
                dst[3] = src[3];
                src += 4;
                dst += 4;
            }

            gGT->rainBuffer[i].numParticles_curr /= numPlyr;
            gGT->rainBuffer[i].numParticles_max =
                (u_short)gGT->rainBuffer[i].numParticles_max / numPlyr;
        }
    }
}
