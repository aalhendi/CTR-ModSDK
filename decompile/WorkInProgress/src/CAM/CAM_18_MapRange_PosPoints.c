#include <common.h>

// get percentage [0 to 0x1000] of Driver between pos1 and pos2
int CAM_MapRange_PosPoints(short *pos1, short *pos2, short *currPos)
{
    int percent;
    short vec1[3];
    short vec2[3];

    // vector distance between position1 and position2.
    vec1[0] = pos1[0] - pos2[0];
    vec1[1] = pos1[1] - pos2[1];
    vec1[2] = pos1[2] - pos2[2];

    MATH_VectorNormalize((VECTOR*)&vec1[0]);
    
    // vector distance between position1 and currentPosition.
    vec2[0] = currPos[0] - pos1[0];
    vec2[1] = currPos[1] - pos1[1];
    vec2[2] = currPos[2] - pos1[2];

    // replace R11R12 and R13R21
	gte_ldsvrtrow0(&vec1[0]);
    __asm__ volatile("" : : "m"(vec2));
    gte_ldv0(&vec2[0]);
    gte_mvmva(0, 0, 0, 3, 0);
#if defined(__mips__)
    __asm__ volatile("mfc2 %0, $25" : "=r"(percent));
#else
    gte_stlvnl0(&percent);
#endif

    // Shift by 12 bits to get the percentage between 0 and 0x1000.
    return FP_INT(percent);
}
