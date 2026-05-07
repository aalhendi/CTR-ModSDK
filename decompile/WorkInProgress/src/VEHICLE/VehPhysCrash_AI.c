#include <common.h>

void VehPhysCrash_AI(struct Driver* bot, Vec3 *v)
{
  int *axis = (int*)0x8009ae28;
  MATRIX *rotMtx = (MATRIX*)0x8009ae38;
  struct NavFrame *navFrame = bot->botData.botNavFrame;

  sdata->unk_rot[0] = (u_short)navFrame->rot[0] << 4;
  sdata->unk_rot[1] = (u_short)navFrame->rot[1] << 4;
  sdata->unk_rot[2] = (u_short)navFrame->rot[2] << 4;

   // convert 3 rotation shorts into rotation matrix
  ConvertRotToMatrix(rotMtx, &sdata->unk_rot[0]);

  axis[0] = rotMtx->m[0][2] >> 4;
  axis[1] = rotMtx->m[1][2] >> 4;
  axis[2] = rotMtx->m[2][2] >> 4;
  
  int botSpeed = FP8_INT(axis[0] * v->x + axis[1] * v->y + axis[2] * v->z);

  bot->botData.unk5bc.ai_speedLinear = botSpeed;
  bot->botData.unk5bc.ai_accelAxis[0] = v->x - FP8_INT(axis[0] * botSpeed);
  
  int projSpeed = axis[2] * botSpeed;
  
  bot->botData.botFlags |= 8;
  
  bot->botData.unk5bc.ai_accelAxis[2] = v->z - FP8_INT(projSpeed);
}
