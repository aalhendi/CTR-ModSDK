#include <common.h>

u_char* CAM_StartLine_FlyIn_FixY(u_short* posRot)
{
  int iVar1;
  int iVar2;
  short posTop[3];
  short posBottom[3];
  short posX;
  short posY;
  short posZ;

  // lev -> mesh_info
  sdata->scratchpadStruct.ptr_mesh_info = sdata->gGT->level1->ptr_mesh_info;
  sdata->scratchpadStruct.Union.QuadBlockColl.searchFlags = 2;
  sdata->scratchpadStruct.Union.QuadBlockColl.qbFlagsWanted = 0x3000; // ground and wall
  sdata->scratchpadStruct.Union.QuadBlockColl.qbFlagsIgnored = 0;

  posX = posRot[0];
  posY = posRot[1];
  posZ = posRot[2];
  
  for (iVar2 = 0; iVar2 < 8; iVar2++)
  {
    iVar1 = 0x400 * iVar2;
    posTop[0] = posX;
    posTop[1] = posY - (iVar1 + 0x400);
    posTop[2] = posZ;
    posBottom[0] = posX;
    posBottom[1] = posY - (iVar1 - 0x100);
    posBottom[2] = posZ;

    COLL_SearchBSP_CallbackQUADBLK(
        (u_int*)posTop,
        (u_int*)posBottom,
        &sdata->scratchpadStruct,
        0);

    if (sdata->scratchpadStruct.boolDidTouchQuadblock != 0)
    {
      posX = sdata->scratchpadStruct.Union.QuadBlockColl.hitPos[0];
      posY = sdata->scratchpadStruct.Union.QuadBlockColl.hitPos[1];
      posZ = sdata->scratchpadStruct.Union.QuadBlockColl.hitPos[2];
      break;
    }
  }

  posRot[1] = posY;

  return (u_char*)&sdata->scratchpadStruct;
}
