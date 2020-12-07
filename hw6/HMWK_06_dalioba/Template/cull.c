// Dalio, Brian A.
// dalioba
// 2020-12-03
//----------------------------------------------------------------
#include <stdio.h>
#include <stdlib.h>

#include "pipeline.h"

//----------------------------------------------------------------
void backfaceCull( int mode, Parameters *paramsP, int noisy )
{
  printf( "//-------------------- Backface Culling --\n" );

  int numInvisible = 0;

  // Get pointers to the Point and Face arrays to make them
  //  easier to access.  We have to distinguish culls of the
  //  regular faces (the ones described by f lines in the object
  //  file) from Bézier faces (the ones created by processing the
  //  p lines in the object file).
  Point *p        = mode == REGULAR ? paramsP->points   : paramsP->bPoints;
  Face  *f        = mode == REGULAR ? paramsP->faces    : paramsP->bFaces;
  int    numFaces = mode == REGULAR ? paramsP->numFaces : paramsP->numBFaces;

  // Construct the x, y, z of the camera position using the
  //  viewpoint in paramsP.  Remember that when the viewpoint is
  //  0.0 the projection is Parallel and the camera position must
  //  approach infinity!  (Use DBL_MAX.)
  FLOAT64  cameraPosX = 0.0;
  FLOAT64  cameraPosY = 0.0;
  FLOAT64  cameraPosZ = paramsP->viewpoint == 0.0 ? DBL_MAX : paramsP->viewpoint;

  if ( noisy ) {
    printf(
      "//   cameraPos  : ( %14.6e, %14.6e, %14.6e )\n",
      cameraPosX, cameraPosY, cameraPosZ );
  }

  // Loop over all of the faces, properly setting the face's
  //  invisible bit.

  for ( int i=0; i<numFaces; i++ ) {
    if ( noisy ) {
      printf(
        "//----------------------------------------\n"
        "//   %d.  f %3d %3d %3d\n",
        i, f[i].v1, f[i].v2, f[i].v3 );
    }

    // Compute the a vector: the face's v2 minus the face's v1.
    FLOAT64 ax = p[f[i].v2][0] - p[f[i].v1][0];
    FLOAT64 ay = p[f[i].v2][1] - p[f[i].v1][1];
    FLOAT64 az = p[f[i].v2][2] - p[f[i].v1][2];

    if ( noisy ) {
      printf( "//     a vector          : ( %14.6e, %14.6e, %14.6e )\n",
        ax, ay, az );
    }

    // Compute the b vector: the face's v3 minus the face's v1.
    FLOAT64 bx = p[f[i].v3][0] - p[f[i].v1][0];
    FLOAT64 by = p[f[i].v3][1] - p[f[i].v1][1];
    FLOAT64 bz = p[f[i].v3][2] - p[f[i].v1][2];

    if ( noisy ) {
      printf( "//     b vector          : ( %14.6e, %14.6e, %14.6e )\n",
        bx, by, bz );
    }

    // Compute the surface normal sn = a × b.
    FLOAT64 snx = ay*bz - az*by;
    FLOAT64 sny = az*bx - ax*bz;
    FLOAT64 snz = ax*by - ay*bx;

    if ( noisy ) {
      printf( "//     surface normal    : ( %14.6e, %14.6e, %14.6e )\n",
        snx, sny, snz );
    }

    // Compute the toTriangle vector, toT: the face's v1 - the camera
    //  position.
    FLOAT64 toTx = p[f[i].v1][0] - cameraPosX;
    FLOAT64 toTy = p[f[i].v1][1] - cameraPosY;
    FLOAT64 toTz = p[f[i].v1][2] - cameraPosZ;

    if ( noisy ) {
      printf( "//     toTriangle vector : ( %14.6e, %14.6e, %14.6e )\n",
        toTx, toTy, toTz );
    }

    // Compute the cos of the angle: sn · toT.
    FLOAT64 dotProd = toTx*snx + toTy*sny + toTz*snz;

    if ( noisy ) {
      printf( "//     dot product       : %14.6e\n",
        dotProd );
    }

    // Set the face's invisible bit.  (1 when the face is
    //  invisible, 0 when the face is visible.)
    f[i].invisible = dotProd >= 0.0;

    // We count how many faces were made invisible.
    if ( f[i].invisible ) numInvisible++;

    if ( noisy ) {
      printf( "//   %d.  f %3d %3d %3d  ->   %d (%svisible)\n",
        i, f[i].v1, f[i].v2, f[i].v3, f[i].invisible,
        f[i].invisible ? "in" : "" );
    }
  }

  if ( noisy ) {
    printf( "//----------------------------------------\n" );
  }

  printf( "//   %d face%s, %d %s invisible.\n",
    numFaces, numFaces == 1 ? "" : "s",
    numInvisible, numInvisible == 1 ? "is" : "are" );
}

//----------------------------------------------------------------
