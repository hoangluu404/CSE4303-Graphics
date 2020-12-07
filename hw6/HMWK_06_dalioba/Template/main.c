// Dalio, Brian A.
// dalioba
// 2020-12-03
//----------------------------------------------------------------
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "pipeline.h"

//----------------------------------------------------------------
int main( int argc, char *argv[] )
{
  int status = ERROR;

  //--------------------------------------
  // Check that we have the proper number of arguments.
  if ( argc != 4 ) {
    printf( "// Usage: %s <paramsFile> <objectFile> <displayFile>\n",
      argv[0] );
    goto exit;
  }

  //--------------------------------------
  // Allocate a parameters structure and load it with the
  //  parameters from the given parameter file.
  Parameters *paramsP = (Parameters *) calloc( 1, sizeof( Parameters ) );
  if ( paramsP == NULL ) {
    printf( "// Unable to allocate Parameters structure.\n" );
    goto exit;
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
  // Get the output file open ...
  FILE *outFP = fopen( argv[3], "w" );
  if ( outFP == NULL ) {
    printf( "// Unable to open \"%s\" for write, (%d) %s.\n",
      argv[3], errno, strerror( errno ) );

    goto exit;
  }

  // ... and write the standard info at the top of the SDF file.
  sdfHeader( outFP, paramsP );

  //--------------------------------------
  // Rotate the object according to the supplied Euler angles.
  eulerRotate( paramsP );

  //--------------------------------------
  // Generate the Bézier patches, if any.

  fprintf( outFP, "\n# Patch faces lines ...\n" );

  generatePatches( outFP, paramsP );

  //--------------------------------------
  // Now that the Bézier patches have been done, deal with the
  //  "ordinary" faces.
  //--------------------------------------

  fprintf( outFP, "\n# Object faces lines ...\n" );

  //--------------------------------------
  // Eliminate unseeable faces, if so requested.  This has to
  //  be done after Euler rotation (so the triangles are in their
  //  final 3D positions) but before projection (so the triangles
  //  are still 3D, not 2D).  (The "noisy" parameter is 0 since we
  //  do NOT want debugging messages printed.)
  if ( paramsP->cull ) {
    backfaceCull( REGULAR, paramsP, 0 );
  }

  //--------------------------------------
  // Project the object according to the supplied projection
  //  parameters.
  project( REGULAR, paramsP );

  //--------------------------------------
  // Display (which includes line clipping!) the object
  //  according to the supplied clipping region / screen size.
  display( REGULAR, outFP, paramsP );

  //--------------------------------------
  // A drop-through to here indicates all went well.
  status = OK;

  //--------------------------------------
  // At this point, status is set and we can free all
  //  dynamically allocated items and exit.
exit:
  if ( outFP != NULL ) fclose( outFP );
  if ( paramsP != NULL ) {
    unloadObject( paramsP );
    free( paramsP );
  }

  printf( "//----------------------------------------\n" );

  return status;
}

//----------------------------------------------------------------
