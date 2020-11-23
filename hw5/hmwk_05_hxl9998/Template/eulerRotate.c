// Dalio, Brian A.
// dalioba
// 2020-11-09
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

  printf(
    "//----------------------------------------\n"
    "// Euler rotation by ϕ %g°, θ %g°, ψ %g° ...\n",
    phiDeg, thetaDeg, psiDeg );

  //----------------------------------------
  // Construct the three rotation matrices.
  Matrix xRot, yRot, zRot;

  rotateXMatrix( paramsP->phi,   xRot );
  rotateYMatrix( paramsP->theta, yRot );
  rotateZMatrix( paramsP->psi,   zRot );

  //----------------------------------------
  // Compute the object's center
  FLOAT64 xmin = DBL_MAX, xmax = -DBL_MAX;
  FLOAT64 ymin = DBL_MAX, ymax = -DBL_MAX;
  FLOAT64 zmin = DBL_MAX, zmax = -DBL_MAX;

  Point *p = &paramsP->points[0];
  for ( int i=0; i<paramsP->numPoints; i++, p++ ) {
    if ( (*p)[0] < xmin ) xmin = (*p)[0];
    if ( (*p)[0] > xmax ) xmax = (*p)[0];

    if ( (*p)[1] < ymin ) ymin = (*p)[1];
    if ( (*p)[1] > ymax ) ymax = (*p)[1];

    if ( (*p)[2] < zmin ) zmin = (*p)[2];
    if ( (*p)[2] > zmax ) zmax = (*p)[2];
  }

  FLOAT64 tx = (xmin + xmax)/2.0;
  FLOAT64 ty = (ymin + ymax)/2.0;
  FLOAT64 tz = (zmin + zmax)/2.0;

  //----------------------------------------
  // Construct the two translation matrices.
  Matrix toOrigin;
  Matrix fromOrigin;

  translateMatrix( -tx, -ty, -tz, toOrigin );
  translateMatrix( tx, ty, tz, fromOrigin );

  //----------------------------------------
  // Construct the combined transformation.
  Matrix tempM1, tempM2;
  Matrix combined;

  multMatrix( fromOrigin, xRot, tempM1 );
  multMatrix( tempM1, yRot, tempM2 );
  multMatrix( tempM2, zRot, tempM1 );
  multMatrix( tempM1, toOrigin, combined );

  //----------------------------------------
  // Display the r and e values.
  printf( "//   r00 = %11.6f, r01 = %11.6f, r02 = %11.6f\n",
    combined[0][0], combined[0][1], combined[0][2] );
  printf( "//   r10 = %11.6f, r11 = %11.6f, r12 = %11.6f\n",
    combined[1][0], combined[1][1], combined[1][2] );
  printf( "//   r20 = %11.6f, r21 = %11.6f, r22 = %11.6f\n",
    combined[2][0], combined[2][1], combined[2][2] );

  printf( "//   ex  = %11.6f, ey  = %11.6f, ez  = %11.6f\n",
    combined[0][3], combined[1][3], combined[2][3] );

  //----------------------------------------
  // Transform all of the points!
  p = &paramsP->points[0];
  for ( int i=0; i<paramsP->numPoints; i++, p++ ) {
    Point tempP;
    multMatrixPoint( combined, *p, tempP );

    memcpy( p, &tempP, sizeof( Point ) );
  }
}

//----------------------------------------------------------------
