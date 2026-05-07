#include <common.h>

struct SelectProfile_LoadSaveData
{
  struct Instance *inst;
  short rot[3];
  short padding;
};

struct SelectProfile_LoadSaveObj
{
  struct Thread *thread;
  struct SelectProfile_LoadSaveData *loadSaveData;
};

void SelectProfile_ThTick(struct Thread *t)
{
  struct SelectProfile_LoadSaveData *loadSaveData =
    ((struct SelectProfile_LoadSaveObj*)t->object)->loadSaveData;

  // loop through 12 instances,
  // 3 instances per profile, 4 profile slots
  for (int i = 0; i < 12; i++)
  {
    // determine if it's Relic, Key or Trophy
    int objIndex = (i % 3);

    // and add its corresponding rate
    loadSaveData[i].rot[1] += sdata->LoadSave_SpinRateY[objIndex];

    struct Instance * inst = loadSaveData[i].inst;

    // convert 3 rotation shorts into rotation matrix
    ConvertRotToMatrix(&inst->matrix, loadSaveData[i].rot);

    if (objIndex != 1)
    {
      // from MetaDataSaveLoad
      short rot[3];
      rot[0] = data.MetaDataLoadSave[i].vec3_specular_inverted[0];
      rot[1] = data.MetaDataLoadSave[i].vec3_specular_inverted[1];
      rot[2] = data.MetaDataLoadSave[i].vec3_specular_inverted[2];
      Vector_SpecLightSpin3D(inst, loadSaveData[i].rot, rot);
    }
  }
}
