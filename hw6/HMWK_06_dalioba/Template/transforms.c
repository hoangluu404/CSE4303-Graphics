// Dalio, Brian A.
// dalioba
// 2020-10-04
//----------------------------------------------------------------
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#include "transforms.h"

//----------------------------------------------------------------
void normalizePoint( Point p1, Point p2 )
{
  if ( p1[3] == 0.0 ) {
    fprintf( stderr, "normalizePoint():  w was 0.0.\n" );
    exit( 1 );
  }

  p2[0] = p1[0] / p1[3];
  p2[1] = p1[1] / p1[3];
  p2[2] = p1[2] / p1[3];
  p2[3]  = 1.0;
}

//----------------------------------------------------------------
void projectParallelMatrix(
  FLOAT64 xmin, FLOAT64 xmax,
  FLOAT64 ymin, FLOAT64 ymax,
  int width, int height,
  Matrix xform )
{
  FLOAT64 sx;
  FLOAT64 sy;
  Matrix  xlate;
  Matrix  scale;

  if ( xmax == xmin ) {
    fprintf( stderr,
      "projectParallelMatrix():  xmax == xmin (%g).\n",
      xmax );

    exit( 1 );
  }

  if ( ymax == ymin ) {
    fprintf( stderr,
      "projectParallelMatrix():  ymax == ymin (%g).\n",
      ymax );

    exit( 1 );
  }

  sx = ( width - 1 )  / ( xmax - xmin );
  sy = ( height - 1 ) / ( ymax - ymin );

  translateMatrix( -xmin, -ymin, 0.0, xlate );
  scaleMatrix( sx, sy, 0.0, scale );

  multMatrix( scale, xlate, xform );
}

void projectPerspectiveMatrix(
  FLOAT64 d,
  FLOAT64 xmin, FLOAT64 xmax,
  FLOAT64 ymin, FLOAT64 ymax,
  int width, int height,
  Matrix xform )
{
  Matrix WStoSS;
  Matrix project;

  if ( d == 0.0 ) {
    fprintf( stderr, "projectPerspectiveMatrix():  d == 0.0.\n" );
    exit( 1 );
  }

  projectParallelMatrix( xmin, xmax, ymin, ymax, width, height, WStoSS );

  copyMatrix( I4, project );

  project[2][2] = 0.0;
  project[3][2] = -1/d;

  multMatrix( WStoSS, project, xform );
}

//----------------------------------------------------------------
void rotateXMatrix( FLOAT64 theta, Matrix xform )
{
  copyMatrix( I4, xform );

  double cth = cos( theta );
  double sth = sin( theta );

  xform[1][1] =  cth;
  xform[1][2] = -sth;
  xform[2][1] =  sth;
  xform[2][2] =  cth;
}

void rotateYMatrix( FLOAT64 theta, Matrix xform )
{
  copyMatrix( I4, xform );

  double cth = cos( theta );
  double sth = sin( theta );

  xform[0][0] =  cth;
  xform[0][2] =  sth;
  xform[2][0] = -sth;
  xform[2][2] =  cth;
}

void rotateZMatrix( FLOAT64 theta, Matrix xform )
{
  copyMatrix( I4, xform );

  double cth = cos( theta );
  double sth = sin( theta );

  xform[0][0] =  cth;
  xform[0][1] = -sth;
  xform[1][0] =  sth;
  xform[1][1] =  cth;
}

//----------------------------------------------------------------
void scaleMatrix( FLOAT64 sx, FLOAT64 sy, FLOAT64 sz, Matrix xform )
{
  copyMatrix( I4, xform );

  xform[0][0] = sx;
  xform[1][1] = sy;
  xform[2][2] = sz;
}

//----------------------------------------------------------------
void shearXMatrix( FLOAT64 hxy, FLOAT64 hxz, Matrix xform )
{
  copyMatrix( I4, xform );

  xform[0][1] = hxy;
  xform[0][2] = hxz;
}

void shearYMatrix( FLOAT64 hyx, FLOAT64 hyz, Matrix xform )
{
  copyMatrix( I4, xform );

  xform[1][0] = hyx;
  xform[1][2] = hyz;
}

void shearZMatrix( FLOAT64 hzx, FLOAT64 hzy, Matrix xform )
{
  copyMatrix( I4, xform );

  xform[2][0] = hzx;
  xform[2][1] = hzy;
}

void shearXYZMatrix(
  FLOAT64 hxy, FLOAT64 hxz,
  FLOAT64 hyx, FLOAT64 hyz,
  FLOAT64 hzx, FLOAT64 hzy,
  Matrix xform )
{
  copyMatrix( I4, xform );

  xform[0][1] = hxy;
  xform[0][2] = hxz;
  xform[1][0] = hyx;
  xform[1][2] = hyz;
  xform[2][0] = hzx;
  xform[2][1] = hzy;
}

//----------------------------------------------------------------
void translateMatrix( FLOAT64 tx, FLOAT64 ty, FLOAT64 tz, Matrix xform )
{
  copyMatrix( I4, xform );

  xform[0][3] = tx;
  xform[1][3] = ty;
  xform[2][3] = tz;
}

//----------------------------------------------------------------
