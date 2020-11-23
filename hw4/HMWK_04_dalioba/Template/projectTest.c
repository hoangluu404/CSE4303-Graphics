// Dalio, Brian A.
// dalioba
// 2020-11-09
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
  if ( argc != 3 ) {
    printf( "// Usage: %s <paramsFile> <objectFile>\n",
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
    free( paramsP );
    goto exit;
  }

  //--------------------------------------
  // Load the object we're going to manipulate and display.
  if ( loadObject( argv[2], paramsP ) ) {
    printf( "// Unable to load object.\n" );
    goto exit;
  }

  //--------------------------------------
  // Display what we read as a check.
  printf( "// %d point%s\n",
    paramsP->numPoints,
    paramsP->numPoints == 1 ? "" : "s" );

  for ( int i=0; i<paramsP->numPoints; i++ ) {
    printf( "// %4d.  %11.6f  %11.6f  %11.6f  %11.6f\n",
      i,
      paramsP->points[i][0], paramsP->points[i][1],
      paramsP->points[i][2], paramsP->points[i][3] );
  }

  printf( "// %d face%s\n",
    paramsP->numFaces,
    paramsP->numFaces == 1 ? "" : "s" );

  for ( int i=0; i<paramsP->numFaces; i++ ) {
    printf( "// %4d.  %4d  %4d  %4d\n",
      i,
      paramsP->faces[i].v1,
      paramsP->faces[i].v2,
      paramsP->faces[i].v3 );
  }

  //--------------------------------------
  // Rotate the object according to the supplied Euler angles.
  eulerRotate( paramsP );

  //--------------------------------------
  // Project the object according to the supplied projection
  //  parameters.
  project( paramsP );

  //--------------------------------------
  // Display what we did as a check.
  printf( "// %d point%s\n",
    paramsP->numPoints,
    paramsP->numPoints == 1 ? "" : "s" );

  for ( int i=0; i<paramsP->numPoints; i++ ) {
    printf( "// %4d.  %11.6f  %11.6f  %11.6f  %11.6f\n",
      i,
      paramsP->points[i][0], paramsP->points[i][1],
      paramsP->points[i][2], paramsP->points[i][3] );
  }

  //--------------------------------------
  // A drop-through to here indicates all went well.
  status = 0;

  //--------------------------------------
  // At this point, status is set and we can free all
  //  dynamically allocated items and exit.
exit:
  unloadObject( paramsP );
  free( paramsP );

  return status;
}

//----------------------------------------------------------------
