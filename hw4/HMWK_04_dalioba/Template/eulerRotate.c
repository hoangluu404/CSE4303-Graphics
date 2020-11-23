// Luu, Hoang
// hxl9998
// 2020-11-16
//----------------------------------------------------------------
#include <float.h>
#include <math.h>
#include <stdio.h>
#include <string.h>

#include "matrix.h"
#include "pipeline.h"
#include "transforms.h"

//----------------------------------------------------------------
void eulerRotate( Parameters *paramsP )
{
  FLOAT64 phiDeg   = RAD2DEG( paramsP->phi );
  FLOAT64 thetaDeg = RAD2DEG( paramsP->theta );
  FLOAT64 psiDeg   = RAD2DEG( paramsP->psi );

  //----------------------------------------
  // Announce ourselves
  printf(
    "//----------------------------------------\n"
    "// Euler rotation by ϕ %g°, θ %g°, ψ %g° ...\n",
    phiDeg, thetaDeg, psiDeg );

  Matrix combined ={
    {0, 0, 0, 0},
    {0, 0, 0, 0},
    {0, 0, 0, 0},
    {0, 0, 0, 0}
  };

  // translation matrix
  Matrix transf = {
    {1, 0, 0, 0},
    {0, 1, 0, 0},
    {0, 0, 1, 0},
    {0, 0, 0, 1}
  };

  // translation back matrix
  Matrix transf2 = {
    {1, 0, 0, 0},
    {0, 1, 0, 0},
    {0, 0, 1, 0},
    {0, 0, 0, 1}
  };
  
  // x rotation matrix
  Matrix rotX = {
    { 1, 0, 0, 0 },
    { 0, 1, 0, 0 },
    { 0, 0, 1, 0 },
    { 0, 0, 0, 1 } 
  };
  rotateXMatrix(paramsP->phi,rotX);

  // y rotation matrix
  Matrix rotY = {
    { 1, 0, 0, 0 },
    { 0, 1, 0, 0 },
    { 0, 0, 1, 0 },
    { 0, 0, 0, 1 }
  };
  rotateYMatrix(paramsP->theta,rotY);

  // z rotation matrix
  Matrix rotZ = {
    { 1, 0, 0, 0 },
    { 0, 1, 0, 0 },
    { 0, 0, 1, 0 },
    { 0, 0, 0, 1 }
  };
  rotateZMatrix(paramsP->psi,rotZ);

  FLOAT64 xMin=paramsP->points[0][0];
  FLOAT64 xMax=paramsP->points[0][0];
  FLOAT64 yMin=paramsP->points[0][1];
  FLOAT64 yMax=paramsP->points[0][1];
  FLOAT64 zMin=paramsP->points[0][2];
  FLOAT64 zMax=paramsP->points[0][2];

  // calculate Min and Max
  for(int i=0;i<paramsP->numPoints;i++){

    //xMin xMax
    if(paramsP->points[i][0]<xMin)
      xMin=paramsP->points[i][0];
    else if(paramsP->points[i][0]>xMax)
      xMax=paramsP->points[i][0];

    //yMin yMax
    if(paramsP->points[i][1]<yMin)
      yMin=paramsP->points[i][1];
    else if(paramsP->points[i][1]>yMax)
      yMax=paramsP->points[i][1];

    //zMin zMax
    if(paramsP->points[i][2]<zMin)
      zMin=paramsP->points[i][2];
    else if(paramsP->points[i][2]>zMax)
      zMax=paramsP->points[i][2];
  }

  // Euler rotation matrix
  multMatrix(rotX,rotY,combined);
  multMatrix(combined,rotZ,combined);

  // tranlsation matrix
  translateMatrix( -(xMax+xMin)/2, -(yMax+yMin)/2, -(zMax+zMin)/2, transf);
  translateMatrix( (xMax+xMin)/2, (yMax+yMin)/2, (zMax+zMin)/2, transf2);

  // combined matrix
  multMatrix(combined, transf, combined);
  multMatrix(transf2, combined, combined);

  // Display the r and e values.
  printf( "//   r00 = %11.6f, r01 = %11.6f, r02 = %11.6f\n",
    combined[0][0], combined[0][1], combined[0][2] );
  printf( "//   r10 = %11.6f, r11 = %11.6f, r12 = %11.6f\n",
    combined[1][0], combined[1][1], combined[1][2] );
  printf( "//   r20 = %11.6f, r21 = %11.6f, r22 = %11.6f\n",
    combined[2][0], combined[2][1], combined[2][2] );

  printf( "//   ex  = %11.6f, ey  = %11.6f, ez  = %11.6f\n",
    combined[0][3], combined[1][3], combined[2][3] );

  // Transform all of the points!
  Point temp = { 0, 0, 0, 0};
  for(int i=0;i<paramsP->numPoints;i++){
    multMatrixPoint( combined, paramsP->points[i] ,temp );
    paramsP->points[i][0]=temp[0];
    paramsP->points[i][1]=temp[1];
    paramsP->points[i][2]=temp[2];
    paramsP->points[i][3]=temp[3];
  }
}

//----------------------------------------------------------------
