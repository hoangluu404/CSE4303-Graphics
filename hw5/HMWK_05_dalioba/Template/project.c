// Dalio, Brian A.
// dalioba
// 2020-11-21
//----------------------------------------------------------------
#include <stdio.h>
#include <string.h>

#include "matrix.h"
#include "pipeline.h"
#include "transforms.h"

//----------------------------------------------------------------
void project( Parameters *paramsP )
{
  Matrix projection;

  //----------------------------------------
  // Announce ourselves and construct the proper
  //  projection matrix.
  if ( paramsP->viewpoint == 0.0 ) {
    printf(
      "//------------------------- Parallel -----\n"
      "// Projecting\n"
      "//    World view  : (%g, %g) .. (%g, %g)\n"
      "//    Canvas size : (W %d, H %d)\n",
      paramsP->wXmin, paramsP->wYmin,
      paramsP->wXmax, paramsP->wYmax,
      paramsP->width, paramsP->height );

    projectParallelMatrix(
      paramsP->wXmin, paramsP->wXmax,
      paramsP->wYmin, paramsP->wYmax,
      paramsP->width, paramsP->height,
      projection );

  } else {
    printf(
      "//------------------------- Perspective --\n"
      "// Projecting\n"
      "//    World view  : (%g, %g) .. (%g, %g)\n"
      "//    Canvas size : (W %d, H %d)\n"
      "//    Viewpoint   : %g\n",
      paramsP->wXmin, paramsP->wYmin,
      paramsP->wXmax, paramsP->wYmax,
      paramsP->width, paramsP->height,
      paramsP->viewpoint );

    projectPerspectiveMatrix(
      paramsP->viewpoint,
      paramsP->wXmin, paramsP->wXmax,
      paramsP->wYmin, paramsP->wYmax,
      paramsP->width, paramsP->height,
      projection );
  }

  //----------------------------------------
  // Transform all of the points!
  Point *p = &paramsP->points[0];

  for ( int i=0; i<paramsP->numPoints; i++, p++ ) {
    Point tempP;

    multMatrixPoint( projection, *p, tempP );
    normalizePoint( tempP, tempP );

    memcpy( p, &tempP, sizeof( Point ) );
  }
}

//----------------------------------------------------------------
