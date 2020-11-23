// Dalio, Brian A.
// dalioba
// 2020-11-21
//----------------------------------------------------------------
#include <stdio.h>
#include <stdlib.h>

#include "pipeline.h"

//----------------------------------------------------------------
int main( int argc, char *argv[] )
{
  int status = 0;

  //--------------------------------------
  // Check that we have the proper number of arguments.
  if ( argc != 4 ) {
    printf( "// Usage: %s <paramsFile> <objectFile> <displayFile>\n",
      argv[0] );
    return 1;
  }

  //--------------------------------------
  // Allocate a parameters structure and load it with the
  //  parameters from the given parameter file.
  Parameters *paramsP = (Parameters *) calloc( 1, sizeof( Parameters ) );
  if ( paramsP == NULL ) {
    printf( "// Unable to allocate Parameters structure.\n" );
    return 1;
  }

  if ( loadParams( argv[1], paramsP ) ) {
    printf( "// Unable to load parameters.\n" );
    goto exit;
  }

  //--------------------------------------
  // Load the object we're going to manipulate and display.
  if ( loadObject( argv[2], paramsP ) ) {
    printf( "// Unable to load object.\n" );
    goto exit;
  }

  //--------------------------------------
  // Rotate the object according to the supplied Euler angles.
  eulerRotate( paramsP );

  //--------------------------------------
  // Eliminate unseeable triangles, if so requested.  This has to
  //  be done after Euler rotation (so the triangles are in their
  //  final 3D positions) but before projection (so the triangles
  //  are still 3D, not 2D).  (The "noisy" parameter is 0 since we
  //  do NOT want debugging messages printed.)
  if ( paramsP->cull ) {
    backfaceCull( paramsP, 0 );
  }

  //--------------------------------------
  // Project the object according to the supplied projection
  //  parameters.
  project( paramsP );

  //--------------------------------------
  // Display (which includes line clipping!) the object
  //  according to the supplied clipping region / screen size.
  display( argv[3], paramsP );

  //--------------------------------------
  // A drop-through to here indicates all went well.
  status = 0;

  //--------------------------------------
  // At this point, status is set and we can free all
  //  dynamically allocated items and exit.
exit:
  unloadObject( paramsP );
  free( paramsP );

  printf( "//----------------------------------------\n" );

  return status;
}

//----------------------------------------------------------------
