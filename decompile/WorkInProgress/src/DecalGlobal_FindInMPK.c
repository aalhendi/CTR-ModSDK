#include <common.h>

int * DecalGlobal_FindInMPK(u_int* curr, char* str)
{
  u_int *check = (u_int *)str;

  while (*(char *)curr != 0)
  {
    // check first 16 bytes,
    // remember this is an int array, not char
    if (((
        // first character
        (curr[0] == check[0]) &&

        // second character
        (curr[1] == check[1])) &&

        // third character 
        (curr[2] == check[2])) &&

        // fourth
        (curr[3] == check[3]))
    {
      // found the texture
      return (int *)curr;
    }
    curr += 8;
  }
  return NULL;
}
