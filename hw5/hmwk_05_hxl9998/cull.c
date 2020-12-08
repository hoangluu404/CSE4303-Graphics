// Luu, Hoang
// hxl9998
// 2020-11-28
//----------------------------------------------------------------
#include <stdio.h>
#include <stdlib.h>

#include "pipeline.h"

//----------------------------------------------------------------
void backfaceCull( Parameters *paramsP, int noisy )
{
  printf( "//-------------------- Backface Culling --\n" );

  int numInvisible = 0;

  Point *p = paramsP->points;
  Face  *f = paramsP->faces;
  
  FLOAT64 cameraPosX = 0, cameraPosY = 0, cameraPosZ = DBL_MAX;
  if(paramsP->viewpoint!=0)
    cameraPosZ = paramsP->viewpoint;

  if ( noisy ) {
    printf(
      "//   cameraPos  : ( %14.6e, %14.6e, %14.6e )\n",
      cameraPosX, cameraPosY, cameraPosZ );
  }

  for ( int i=0; i<paramsP->numFaces; i++ ) {
    if ( noisy ) {
      printf(
        "//----------------------------------------\n"
        "//   %d.  f %3d %3d %3d\n",
        i, f[i].v1, f[i].v2, f[i].v3 );
    }

    FLOAT64 ax, ay, az;
    ax = p[f[i].v2][0]  -  p[f[i].v1][0];
    ay = p[f[i].v2][1]  -  p[f[i].v1][1];
    az = p[f[i].v2][2]  -  p[f[i].v1][2];

    if ( noisy ) {
      printf( "//     a vector          : ( %14.6e, %14.6e, %14.6e )\n",
        ax, ay, az );
    }

    FLOAT64 bx, by, bz;
    bx = p[f[i].v3][0]  -  p[f[i].v1][0];
    by = p[f[i].v3][1]  -  p[f[i].v1][1];
    bz = p[f[i].v3][2]  -  p[f[i].v1][2];

    if ( noisy ) {
      printf( "//     b vector          : ( %14.6e, %14.6e, %14.6e )\n",
        bx, by, bz );
    }

    FLOAT64 snx, sny, snz;
    snx = (ay*bz-az*by);
    sny = (az*bx-ax*bz);
    snz = (ax*by-ay*bx);

    if ( noisy ) {
      printf( "//     surface normal    : ( %14.6e, %14.6e, %14.6e )\n",
        snx, sny, snz );
    }
    FLOAT64 toTx, toTy, toTz;
    toTx = p[f[i].v1][0] - cameraPosX;
    toTy = p[f[i].v1][1] - cameraPosY;
    toTz = p[f[i].v1][2] - cameraPosZ;

    if ( noisy ) {
      printf( "//     toTriangle vector : ( %14.6e, %14.6e, %14.6e )\n",
        toTx, toTy, toTz );
    }

    FLOAT64 dotProd = snx*toTx + sny*toTy + snz*toTz;

    if ( noisy ) {
      printf( "//     dot product       : %14.6e\n",
        dotProd );
    }

    if(dotProd<0)
      f[i].invisible = 0;
    else
      f[i].invisible = 1;   

    if(f[i].invisible)
      numInvisible++;
    if ( noisy ) {
      printf( "//   %d.  f %3d %3d %3d  ->   %d (%svisible)\n",
        i, f[i].v1, f[i].v2, f[i].v3, f[i].invisible,
        f[i].invisible ? "in" : "" );
    }
  }

  if ( noisy ) {
    printf( "//----------------------------------------\n" );
  }

  printf( "//   %d face%s in model, %d %s invisible.\n",
    paramsP->numFaces, paramsP->numFaces == 1 ? "" : "s",
    numInvisible, numInvisible == 1 ? "is" : "are" );
}

//----------------------------------------------------------------
