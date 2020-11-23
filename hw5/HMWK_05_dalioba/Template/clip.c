// Dalio, Brian A.
// dalioba
// 2020-11-13
//----------------------------------------------------------------
#include <stdio.h>
#include <stdlib.h>

#include "pipeline.h"

//----------------------------------------------------------------
#define INSIDE    (0)
#define LEFT      (1 << 0)
#define ABOVE     (1 << 1)
#define RIGHT     (1 << 2)
#define BELOW     (1 << 3)

static int _zone( int x, int y, Parameters *paramsP );

//----------------------------------------------------------------
int clipLine( int *p1x, int *p1y, int *p2x, int *p2y, Parameters *paramsP )
{
  while ( 1 ) {
    FLOAT64 x1 = *p1x, y1 = *p1y;
    FLOAT64 x2 = *p2x, y2 = *p2y;

    int zone1 = _zone( *p1x, *p1y, paramsP );
    int zone2 = _zone( *p2x, *p2y, paramsP );

    // Trivial accept
    if ( ( zone1 | zone2 ) == 0 ) return 1;

    // Trivial reject
    if ( ( zone1 & zone2 ) != 0 ) return 0;

    // Not a trivial case -- have to slide an endpoint
    //  and try again.
    int     zone3 = zone1 == INSIDE ? zone2 : zone1;
    FLOAT64 x, y;

    if ( zone3 & ABOVE ) {
      // Point was ABOVE.  Move it along the line down to Y max.
      x = x1 + ( x2 - x1 )*( paramsP->sYmax - y1 )/( y2 - y1 );
      y = paramsP->sYmax;

    } else if ( zone3 & BELOW ) {
      // Point was BELOW.  Move it along the line up to Y min.
      x = x1 + ( x2 - x1 )*( paramsP->sYmin - y1 )/( y2 - y1 );
      y = paramsP->sYmin;

    } else if ( zone3 & RIGHT ) {
      // Point was to the RIGHT.  Move it along the line over to X max.
      x = paramsP->sXmax;
      y = y1 + ( y2 - y1 )*( paramsP->sXmax - x1 )/( x2 - x1 );

    } else if ( zone3 & LEFT ) {
      // Point was to the LEFT.  Move it along the line over to X min.
      x = paramsP->sXmin;
      y = y1 + ( y2 - y1 )*( paramsP->sXmin - x1 )/( x2 - x1 );

    } else {
      // Huh?  We didn't match _any_ region?  How did that happen?
      printf( "Code 0x%x did not match any region?\n", zone3 );
      exit( 1 );
    }

    // Replace the point that got slid with the new computed
    //  value.  We have to convert it from FLOAT64 to int!
    if ( zone3 == zone1 ) {
      *p1x = ROUND( x );
      *p1y = ROUND( y );

    } else {
      *p2x = ROUND( x );
      *p2y = ROUND( y );
    }
  }
}

//----------------------------------------------------------------
static int _zone( int x, int y, Parameters *paramsP )
{
  int result = INSIDE;

  if ( x < paramsP->sXmin ) {
    result |= LEFT;
  } else if ( x > paramsP->sXmax ) {
    result |= RIGHT;
  }

  if ( y < paramsP->sYmin ) {
    result |= BELOW;
  } else if ( y > paramsP->sYmax ) {
    result |= ABOVE;
  }

  return result;
}

//----------------------------------------------------------------
