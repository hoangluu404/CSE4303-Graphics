// Dalio, Brian A.
// dalioba
// 2020-12-03
//----------------------------------------------------------------
#include <errno.h>
#include <stdio.h>
#include <string.h>

#include "pipeline.h"

static void drawFace( int mode, FILE *fp, int v1, int v2, int v3, Parameters *paramsP );
static void drawLine( int mode, FILE *fp, int p1, int p2, Parameters *paramsP );

//----------------------------------------------------------------
void sdfHeader( FILE *fp, Parameters *paramsP )
{
  //----------------------------------------
  // SDF parameters
  printf(
    "//----------------------------------------\n"
    "// Displaying\n"
    "//    Canvas size : (W %d, H %d)\n"
    "//    Clip region : (%d, %d) .. (%d, %d)\n",
    paramsP->width, paramsP->height,
    paramsP->sXmin, paramsP->sYmin,
    paramsP->sXmax, paramsP->sYmax );

  //--------------------------------------
  // Canvas size
  fprintf( fp, "# Canvas size\n" );

  fprintf( fp, "c  %d  %d\n",
    paramsP->width, paramsP->height );

  //--------------------------------------
  // Clip region lines
  fprintf( fp, "\n# Clip region lines ...\n" );

  fprintf( fp, "l %d %d %d %d\n",
    paramsP->sXmin, paramsP->sYmin, paramsP->sXmax, paramsP->sYmin );

  fprintf( fp, "l %d %d %d %d\n",
    paramsP->sXmax, paramsP->sYmin, paramsP->sXmax, paramsP->sYmax );

  fprintf( fp, "l %d %d %d %d\n",
    paramsP->sXmax, paramsP->sYmax, paramsP->sXmin, paramsP->sYmax );

  fprintf( fp, "l %d %d %d %d\n",
    paramsP->sXmin, paramsP->sYmax, paramsP->sXmin, paramsP->sYmin );

}

//----------------------------------------------------------------
void display( int mode, FILE *fp, Parameters *paramsP )
{
  //--------------------------------------
  // Draw each face
  Face *f        = mode == REGULAR ? paramsP->faces    : paramsP->bFaces;
  int   numFaces = mode == REGULAR ? paramsP->numFaces : paramsP->numBFaces;

  if ( paramsP->debug ) {
    printf( "//------------------------------- Faces --\n" );
  }

  for ( int i=0; i<numFaces; i++, f++ ) {
    if ( !f->invisible ) {
      drawFace( mode, fp, f->v1, f->v2, f->v3, paramsP );
    }
  }
}

//----------------------------------------------------------------
static void drawFace( int mode, FILE *fp, int v1, int v2, int v3, Parameters *paramsP )
{
  // Draw each of the sides of the face, IN THE PROPER ORDER.
  //  (The proper order is v1 -> v2, v2 -> v3, v3 -> v1.)
  drawLine( mode, fp, v1, v2, paramsP );
  drawLine( mode, fp, v2, v3, paramsP );
  drawLine( mode, fp, v3, v1, paramsP );
}

static void drawLine( int mode, FILE *fp, int p1, int p2, Parameters *paramsP )
{
  Point *p = mode == REGULAR ? paramsP->points : paramsP->bPoints;

  // ROUND the x,y of each point to be an integer.
  int p1x, p1y;
  int p2x, p2y;

  p1x = ROUND( p[p1][0] );
  p1y = ROUND( p[p1][1] );

  p2x = ROUND( p[p2][0] );
  p2y = ROUND( p[p2][1] );

  // Use clipLine() to see if the line should be drawn.
  //  If so, draw it, using the updated coordinates of p1 and p2.
  if ( clipLine( &p1x, &p1y, &p2x, &p2y, paramsP ) ) {
    fprintf( fp, "l %d %d %d %d\n",
      p1x, p1y, p2x, p2y );
  } else {
    if  ( paramsP->debug ) {
      printf( "//   l (%4d, %4d), (%4d, %4d) not drawn\n",
        p1x, p1y, p2x, p2y );
    }
  }
}

//----------------------------------------------------------------
