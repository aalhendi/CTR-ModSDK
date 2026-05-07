#include <common.h>

int * DecalGlobal_FindInLEV(struct Level *lev, char *str)
{
  struct LevTexLookup *lookup;
  struct IconGroup **curr;
  struct IconGroup **last;
  int *checkBytes;
  int *iconGroup;

  // lev -> levTexLookup
  lookup = lev->levTexLookup;
  checkBytes = (int *)str;

  // if pointer is valid
  if (lookup != NULL)
  {
    // pointer to first
    curr = lookup->firstIconGroupPtr;

    // pointer to end
    last = curr + lookup->numIconGroup;
    while (curr < last)
    {
      iconGroup = (int *)*curr;

      // check first 16 bytes,
      // remember this is an int array, not char
      if ((((iconGroup[0] == checkBytes[0]) &&
            (iconGroup[1] == checkBytes[1])) &&
            (iconGroup[2] == checkBytes[2])) &&
            (iconGroup[3] == checkBytes[3]))
      {
        // return pointer
        return iconGroup;
      }
      // next in loop
      curr++;
    } 
  }
  return NULL;
}
