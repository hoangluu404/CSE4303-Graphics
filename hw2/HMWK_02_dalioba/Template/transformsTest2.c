// Dalio, Brian A.
// dalioba
// 2020-10-04
//----------------------------------------------------------------
#include <stdio.h>

#include "matrix.h"
#include "transforms.h"

//----------------------------------------------------------------
#define XMIN       (-1.00)
#define XMAX       ( 2.20)
#define YMIN       (-0.90)
#define YMAX       ( 0.90)

#define SCALE      (50)
#define WIDTH      (SCALE*16)
#define HEIGHT     (SCALE* 9)

#define CAMERAPOS  (3.30)

Point pts[] = {
  { -0.1,  0.35355339,  0.35355339, 1 },
  {  0.7,  0.35355339,  0.35355339, 1 },
  { -0.1, -0.21213203, -0.21213203, 1 },
  {  0.7, -0.21213203, -0.21213203, 1 },
  {  0.3, -0.42426407,  0.56568542, 1 }
};

#define NUM_POINTS ( (int) ( sizeof( pts ) / sizeof( Point ) ) )

//----------------------------------------------------------------
int main()
{
  Point  pTmp;
  Point  pTmp2;

  printf( "//-- Raw points ---------------------------------------------------" );
  for ( int i = 0; i < NUM_POINTS; i++ ) {
    printf( "\n//-- pts[%2d] --------------------\n", i );
    dumpPointT( pts[i] );
  }

  printf( "\n//-- shearXYZ 1.1, 2.2, 3.3, 4.4, 5.5, 6.6 ----------------------\n" );
  Matrix shearXYZ;

  shearXYZMatrix( 1.1, 2.2, 3.3, 4.4, 5.5, 6.6, shearXYZ );

  printf( "//-- shearXYZ --------------------\n" );
  dumpMatrix( shearXYZ );

  for ( int i = 0; i < NUM_POINTS; i++ ) {
    printf( "\n//-- shearXYZ·pts[%2d] ------------\n", i );
    multMatrixPoint( shearXYZ, pts[i], pTmp );
    dumpPointT( pTmp );
  }

  printf( "\n//-- Projection parameters --------------------------------------\n" );
  printf( "XMIN, XMAX    : ( %g, %g )\n", XMIN, XMAX );
  printf( "YMIN, YMAX    : ( %g, %g )\n", YMIN, YMAX );
  printf( "WIDTH, HEIGHT : ( %d, %d )\n", WIDTH, HEIGHT );
  printf( "CAMERAPOS     : %g\n", CAMERAPOS );

  printf( "\n//-- Projections (using above parameters) -----------------------\n" );
  Matrix parMatrix;
  Matrix perMatrix;

  projectParallelMatrix( XMIN, XMAX, YMIN, YMAX, WIDTH, HEIGHT, parMatrix );
  projectPerspectiveMatrix( CAMERAPOS, XMIN, XMAX, YMIN, YMAX, WIDTH, HEIGHT, perMatrix );

  printf( "//-- projectParallel -------------\n" );
  dumpMatrix( parMatrix );

  printf( "//-- projectPerspective ----------\n" );
  dumpMatrix( perMatrix );

  for ( int i = 0; i < NUM_POINTS; i++ ) {
    printf( "\n//-- pts[%2d] --------------------\n", i );
    printf( "Original    : " ); dumpPointT( pts[i] );
    multMatrixPoint( parMatrix, pts[i], pTmp );
    printf( "Parallel    : " ); dumpPointT( pTmp );
    multMatrixPoint( perMatrix, pts[i], pTmp );
    printf( "Perspective : " ); dumpPointT( pTmp );
    normalizePoint( pTmp, pTmp2 );
    printf( "Normalized  : " ); dumpPointT( pTmp2 );
  }

  printf( "\n//---------------------------------------------------------------\n" );

  return 0;
}

//----------------------------------------------------------------
