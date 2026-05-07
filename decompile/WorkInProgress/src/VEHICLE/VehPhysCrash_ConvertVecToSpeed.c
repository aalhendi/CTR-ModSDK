#include <common.h>

void VehPhysCrash_ConvertVecToSpeed(struct Driver* d, int *v)
{
  int speed3D;
  long speed2D;
  int projX;
  int projY;
  int projZ;
  int projOnMovingDirAxis;

  // 2D movement: sqrt(x^2 + z^2 << 0x10)
  speed2D = VehCalc_FastSqrt(v[0] * v[0] + v[2] * v[2], 0x10);

  // 3D movement: sqrt(x^2 + y^2 + z^2 << 0x10)
  speed3D = (u_int)VehCalc_FastSqrt(v[0] * v[0] + v[1] * v[1] + v[2] * v[2], 0x10) >> 8;

  // Set driver's 3D speed
  d->speed = speed3D;
  
  // Calculate vertical axis rotation (y-axis)
  speed2D = ratan2(v[1] << 8, speed2D);
  d->axisRotationY = (short)speed2D;
  
  // Calculate horizontal axis rotation (x-axis)
  speed2D = ratan2(v[0], v[2]);
  d->axisRotationX = (short)speed2D;

  // Project the velocity vector onto the moving direction axis
  projOnMovingDirAxis = FP_INT(
      v[0] * (int)d->matrixMovingDir.m[0][1] +
      v[1] * (int)d->matrixMovingDir.m[1][1] +
      v[2] * (int)d->matrixMovingDir.m[2][1]);
		  
  projX = (int)FP_INT(d->matrixMovingDir.m[0][1] * projOnMovingDirAxis);
  projY = (int)FP_INT(d->matrixMovingDir.m[1][1] * projOnMovingDirAxis);
  projZ = (int)FP_INT(d->matrixMovingDir.m[2][1] * projOnMovingDirAxis);

  // Calculate 3D projection speed
  speed3D = (u_int)VehCalc_FastSqrt(projX * projX + projY * projY + projZ * projZ, 0x10) >> 8;

  if (projOnMovingDirAxis < 0) {
    speed3D = -speed3D;
  }
  d->jumpHeightCurr = speed3D;

  // Calculate deviation from projection
  projX = v[0] - projX;
  projY = v[1] - projY;
  projZ = v[2] - projZ;

  // Compute approximate speed based on deviation
  speed3D = (u_int)VehCalc_FastSqrt(projX * projX + projY * projY + projZ * projZ, 0x10) >> 8;

  d->speedApprox = speed3D;

  if (
      projX * d->matrixMovingDir.m[0][2] + 
      projY * d->matrixMovingDir.m[1][2] +
      projZ * d->matrixMovingDir.m[2][2] < 0) 
  {
    d->speedApprox = -speed3D;
  }
}
