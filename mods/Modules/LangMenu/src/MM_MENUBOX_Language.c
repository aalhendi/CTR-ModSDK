#include <common.h>
#include "macro.h"

char langIndex[7] =
    {
        0, // JAPANESE (no text, language glitch)
        // 2 ENGLISH - UK
        1, // ENGLISH - US
        3, // FRENCH
        4, // GERMAN
        5, // ITALIAN
        6, // SPANISH
        7  // NETHERLANDS
};

int timer;

void MM_MENUBOX_Language(struct MenuBox *mb)
{
  struct GameTracker *gGT = sdata->gGT;
  char row = mb->rowSelected;

  if (mb->ptrPrevBox_InHierarchy == 0)
  {
    if (mb->unk1e == 1)
    {
      // if anyone move the menu cursor
      if (sdata->gGamepads->anyoneHeldCurr != 0)
      {
        // reset menu timer
        timer = 900;
      }

      timer--;

      // if the timer ran out,
      // automatically choose current selection (do the rest of the func)
      if (-1 < timer)
      {
        return;
      }
    }
    struct MenuBox *mainMenu = &OVR_230.menubox_mainMenu;
    gGT->notFoundInCode2 = 1; // boolLangChosen
    mainMenu->state |= DISABLE_INPUT_ALLOW_FUNCPTRS;
    sdata->ptrDesiredMenuBox = mainMenu;
    goto LOAD_LNG;
  }

  if ((row > -1) && (row < 6))
  {
  LOAD_LNG:
    gGT->langIndex = langIndex[row + 1];
    gGT->gameMode2 |= 0x10000000; // LANG_CHANGE
  }

  mb->unk1e = 0;
  mb->ptrPrevBox_InHierarchy->state &= ~(DRAW_NEXT_MENU_IN_HIERARCHY | ONLY_DRAW_TITLE);
};

struct MenuRow rows_language[] =
    {
        [0] = MENU_ROW(133, ROW_ENGLISH, ROW_FRENCH, ROW_ENGLISH, ROW_FRENCH),
        [1] = MENU_ROW(134, ROW_ENGLISH, ROW_GERMAN, ROW_ENGLISH, ROW_GERMAN),
        [2] = MENU_ROW(135, ROW_FRENCH, ROW_ITALIAN, ROW_FRENCH, ROW_ITALIAN),
        [3] = MENU_ROW(136, ROW_GERMAN, ROW_SPANISH, ROW_GERMAN, ROW_SPANISH),
        [4] = MENU_ROW(137, ROW_ITALIAN, ROW_DUTCH, ROW_ITALIAN, ROW_DUTCH),
        [5] = MENU_ROW(138, ROW_SPANISH, ROW_DUTCH, ROW_SPANISH, ROW_DUTCH),
        [6] = FINALIZER_ROW
        };

struct MenuBox menubox_language =
    {
        .stringIndexTitle = -1,
        .posX_curr = 256,
        .posY_curr = 118,
        .unk1e = 1,
        .state = (CENTER_ON_COORDS | EXECUTE_FUNCPTR | 0x800),
        .rows = rows_language,
        .funcPtr = MM_MENUBOX_Language,
        .width = 171,
        .height = 120
};

struct MenuBox* MM_Menubox_LanguageBoot(struct GameTracker* gGT) 
{
  if (!gGT->notFoundInCode2) {
    timer = 900;
    return &menubox_language;
  } else {
    return &OVR_230.menubox_mainMenu;
  }
};