// Dalio, Brian A.
// dalioba
// 2020-11-21
//----------------------------------------------------------------
#include <stdio.h>
#include <stdlib.h>

#include "pipeline.h"

//----------------------------------------------------------------
void backfaceCull( Parameters *paramsP, int noisy )
{
  printf( "//-------------------- Backface Culling --\n" );

  int numInvisible = 0;

  // Get pointers to the Point and Face arrays to make them
  //  easier to access.
  Point *p = paramsP->points;
  Face  *f = paramsP->faces;

  // Construct the x, y, z of the camera position using the
  //  viewpoint in paramsP.  Remember that when the viewpoint is
  //  0.0 the projection is Parallel and the camera position must
  //  approach infinity!  (Use DBL_MAX.)

  // TODO: Determine the x,y,z of the camera position based on
  //        the viewpoint in paramsP.
  //
  //    (HINT:  Use the names as shown in the printf just below
  //        so the debug statement will work properly.)

  if ( noisy ) {
    printf(
      "//   cameraPos  : ( %14.6e, %14.6e, %14.6e )\n",
      cameraPosX, cameraPosY, cameraPosZ );
  }

  // Loop over all of the faces, properly setting the face's
  //  invisible bit.

  for ( int i=0; i<paramsP->numFaces; i++ ) {
    if ( noisy ) {
      printf(
        "//----------------------------------------\n"
        "//   %d.  f %3d %3d %3d\n",
        i, f[i].v1, f[i].v2, f[i].v3 );
    }

    // Compute the a vector: the face's v2 minus the face's v1.

    // TODO: Determine the x,y,z of the a vector.  The math is in
    //        the handout.
    //
    //    (HINT:  Use the names as shown in the printf just below
    //        so the debug statement will work properly.)

    if ( noisy ) {
      printf( "//     a vector          : ( %14.6e, %14.6e, %14.6e )\n",
        ax, ay, az );
    }

    // Compute the b vector: the face's v3 minus the face's v1.

    // TODO: Determine the x,y,z of the b vector.  The math is in
    //        the handout.
    //
    //    (HINT:  Use the names as shown in the printf just below
    //        so the debug statement will work properly.)

    if ( noisy ) {
      printf( "//     b vector          : ( %14.6e, %14.6e, %14.6e )\n",
        bx, by, bz );
    }

    // Compute the surface normal sn = a × b.

    // TODO: Determine the x,y,z of the surface normal.  Be
    //        careful with the math!  Refer to the handout for the
    //        proper formulae.
    //
    //    (HINT:  Use the names as shown in the printf just below
    //        so the debug statement will work properly.)

    if ( noisy ) {
      printf( "//     surface normal    : ( %14.6e, %14.6e, %14.6e )\n",
        snx, sny, snz );
    }

    // Compute the toTriangle vector, toT: the face's v1 - the camera
    //  position.

    // TODO: Determine the x,y,z of the toTriangle vector.  The
    //        math is in the handout.
    //
    //    (HINT:  Use the names as shown in the printf just below
    //        so the debug statement will work properly.)

    if ( noisy ) {
      printf( "//     toTriangle vector : ( %14.6e, %14.6e, %14.6e )\n",
        toTx, toTy, toTz );
    }

    // Compute the cos of the angle: sn · toT.

    // TODO: Determine cos θ using the dot product.  All the math
    //        you need is in the handout.
    //
    //    (HINT:  Use the name as shown in the printf just below
    //        so the debug statement will work properly.)

    if ( noisy ) {
      printf( "//     dot product       : %14.6e\n",
        dotProd );
    }

    // Set the face's invisible bit.  (1 when the face is
    //  invisible, 0 when the face is visible.)

    // TODO: Set the invisible bit of the current face using the
    //        cos θ you just computed.  Be careful that you set it
    //        with the correct polarity.

    // We count how many faces were made invisible.

    // TODO: Count the number of faces that were determined to be
    //        invisible.  This running total is kept in the
    //        variable numInvisible.

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
