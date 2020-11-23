// Luu, Hoang
// hxl9998
// 2020-11-16
//----------------------------------------------------------------
#include <errno.h>
#include <stdio.h>
#include <string.h>

#include "pipeline.h"

static void drawFace( FILE *fp, int v1, int v2, int v3, Parameters *paramsP );
static void drawLine( FILE *fp, int p1, int p2, Parameters *paramsP );

//----------------------------------------------------------------
int display( char *fName, Parameters *paramsP )
{
  //----------------------------------------
  // Announce ourselves
  printf(
    "//----------------------------------------\n"
    "// Displaying\n"
    "//    out file    : \"%s\"\n"
    "//    canvas size : (W %d, H %d)\n"
    "//    clip region : (%d, %d) .. (%d, %d)\n",
    fName,
    paramsP->width, paramsP->height,
    paramsP->sXmin, paramsP->sYmin,
    paramsP->sXmax, paramsP->sYmax );

  //----------------------------------------
  // Get output file open
  FILE *fp = fopen( fName, "w" );
  if ( fp == NULL ) {
    printf( "// Unable to open \"%s\" for write, (%d) %s.\n",
      fName, errno, strerror( errno ) );

    return ERROR;
  }

  //--------------------------------------
  // Canvas size
  fprintf( fp, "# Canvas size\n" );
  fprintf( fp, "c  %d  %d\n", paramsP->width, paramsP->height);
  // TODO:  fprintf to fp a canvas size instruction, which looks
  //          like this,
  //
  //            c  <width>  <height>
  //
  //          where each of the two <> is an integer.
  //
  //    (HINT:  The canvas size is in paramsP.)

  //--------------------------------------
  // Clip region lines
  fprintf( fp, "\n# Clip region lines ...\n" );
  fprintf( fp, "l %d %d %d %d\n", paramsP->sXmin, paramsP->sYmin, paramsP->sXmax, paramsP->sYmin);
  fprintf( fp, "l %d %d %d %d\n", paramsP->sXmax, paramsP->sYmin, paramsP->sXmax, paramsP->sYmax);
  fprintf( fp, "l %d %d %d %d\n", paramsP->sXmax, paramsP->sYmax, paramsP->sXmin, paramsP->sYmax);
  fprintf( fp, "l %d %d %d %d\n", paramsP->sXmin, paramsP->sYmax, paramsP->sXmin, paramsP->sYmin);
  // TODO:  fprintf to fp four line instructions, one for each of
  //          the sides of the clip region.  Start with the xmin,
  //          ymin corner and go counter-clockwise.
  //          The line instructions look like this,
  //
  //              l  <x1>  <y1>  <x2>  <y2>
  //
  //          where each of the four <> is an integer.
  //
  //    (HINT:  The clip region points are in paramsP.)

  //--------------------------------------
  // Draw each face
  fprintf( fp, "\n# Object lines ...\n" );
  for(int i=0; i<paramsP->numFaces; i++){
    drawFace(fp, paramsP->faces[i].v1, paramsP->faces[i].v2, paramsP->faces[i].v3, paramsP);
  }
  // TODO:  Draw each face by calling the drawFace() routine
  //          with the proper parameters for each face.

  //--------------------------------------
  // Free/close local items.
  fclose( fp );

  // All done, w/no error!
  return OK;
}

//----------------------------------------------------------------
static void drawFace( FILE *fp, int v1, int v2, int v3, Parameters *paramsP )
{
  // Draw each of the sides of the face, IN THE PROPER ORDER.
  //  (The proper order is v1 -> v2, v2 -> v3, v3 -> v1.)
  drawLine(fp, v1, v2, paramsP);
  drawLine(fp, v2, v3, paramsP);
  drawLine(fp, v3 ,v1, paramsP);


  // TODO:  Three calls to drawLine(), each with the proper
  //          parameters.
}

static void drawLine( FILE *fp, int p1, int p2, Parameters *paramsP )
{
  // Draw a line from the object's p1 to the object's p2.

  // ROUND the x,y of each point to be an integer.
  int p1x, p1y;
  int p2x, p2y;

  p1x = ROUND(paramsP->points[p1][0]);
  p1y = ROUND(paramsP->points[p1][1]);
  p2x = ROUND(paramsP->points[p2][0]);
  p2y = ROUND(paramsP->points[p2][1]);

  // TODO:  Get the x,y coords of p1 into p1x, p1y as ROUNDED
  //          INTEGERS.  Do the same for the x,y coords of p2
  //          into p2x, p2y as ROUNDED INTEGERS.
  //
  //    (HINT:  Just use the handy ROUND() macro.)
  if(clipLine(&p1x, &p1y, &p2x, &p2y, paramsP)){
    fprintf( fp, "l %d %d %d %d\n", p1x, p1y, p2x, p2y);
  };
  // Use clipLine() to see if the line should be drawn.
  //  If so, draw it, using the updated coordinates of p1 and p2.

  // TODO:  Call clipLine() with the proper parameters.  Check its
  //          return value:  0 means to NOT draw the line.
  //          Otherwise, fprintf to fp a line instruction, which
  //          looks like this:
  //
  //              l  <p1x>  <p1y>  <p2x>  <p2y>
  //
  //          where each the four <> is an integer.
}

//----------------------------------------------------------------
