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

void SelectProfile_Destroy(void)
{
    struct SelectProfile_LoadSaveObj *loadSaveObj =
        (struct SelectProfile_LoadSaveObj *)sdata->ptrLoadSaveObj;

    // if loadsave object exists
    if (loadSaveObj != NULL)
    {
        struct SelectProfile_LoadSaveData *loadSaveData =
            loadSaveObj->loadSaveData;
        struct Thread *t = loadSaveObj->thread;

        // destroy 12 instances, for LoadSave's
        // 4x trophy, 4x relic, and 4x key

        for (char i = 0; i < 0xc; i++)
        {
            struct Instance *inst = loadSaveData[i].inst;

            if (inst != NULL)
            {
                INSTANCE_Death(inst);
            }
        }

        // erase pointer to object
        sdata->ptrLoadSaveObj = 0;

        // 0x800 = this thread needs to be deleted
        t->flags |= 0x800;
    }
}
