#include <common.h>

int DECOMP_TitleFlag_GetOT(void)
{
    short sVar1;
    int iVar2;
    struct GameTracker* gGT = sdata->gGT;
	
	int* otDrawFirst_FarthestDepth;
	int* otDrawLast_ClosestDepth;
	
	otDrawFirst_FarthestDepth = &gGT->tileView[0].ptrOT[0x3FF];
	otDrawLast_ClosestDepth = gGT->ot_tileView_UI[gGT->swapchainIndex];
	
	// on PC, the gGT->tileView[X].ptrOT
	// is not connected to the main swapchain OT
	#ifdef REBUILD_PC
	otDrawFirst_FarthestDepth = &otDrawLast_ClosestDepth[5];
	#endif

    if (sdata->unk_CheckFlag2 == 0)
    {
        sdata->unk_CheckFlag2 = 1;
    }

	// transitioning on-screen
    if (sdata->TitleFlag_AnimationType == 0)
    {
		// set fully "off" to start transition "on"
        if (sdata->TitleFlag_Position < 0)
            sdata->TitleFlag_Position = 5000;

        sdata->unk_CheckFlag1 = 300;

        iVar2 = sdata->TitleFlag_Position;
		
		// if transitioning
        if(iVar2 != 0)
        {
            // skip last 8 frames to zero
            if (iVar2 < 8)
                sdata->TitleFlag_Position = 0;

            // transition for frame >= 8
            else
            {
				// rate of transition
                
				iVar2 = ((u_short)sdata->TitleFlag_Position >> 3) * gGT->elapsedTimeMS;
				iVar2 = iVar2 >> 5;
				
                sVar1 = -(short)iVar2;
                if (iVar2 < 1)
                {
                    sVar1 = -1;
                }

                sdata->TitleFlag_Position += sVar1;
            }
        }
		
		// transition is finished
        else
        {
			if (sdata->TitleFlag_DrawOrder != -1)
				return otDrawFirst_FarthestDepth;
				
			sdata->TitleFlag_DrawOrder = 0;
        }

    }

    // transition off-screen
    if (sdata->TitleFlag_AnimationType == 2)
    {
        if ((short)sdata->unk_CheckFlag1 < 1000)
        {
            sdata->unk_CheckFlag1 += (short)((gGT->elapsedTimeMS * 10) >> 5);
        }

        // If transitioning "off"
        if (sdata->TitleFlag_Position > -5000)
        {
            sdata->TitleFlag_Position -= 
				(((u_int)sdata->unk_CheckFlag1 >> 2) * gGT->elapsedTimeMS) >> 5;
        }
		
		// finished transitioning off
        else
        {
            sdata->TitleFlag_Position = 5000;
            sdata->TitleFlag_AnimationType = 0;
            gGT->renderFlags &= ~(0x1000);
        }
    }

    return otDrawLast_ClosestDepth;
}