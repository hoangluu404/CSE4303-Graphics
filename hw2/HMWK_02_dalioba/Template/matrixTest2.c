// Dalio, Brian A.
// dalioba
// 2020-10-04
//----------------------------------------------------------------
#include <stdio.h>

#include "matrix.h"

//----------------------------------------------------------------
int main()
{
  Point p = { 1, 2, 3, 4 };
  Point pTmp;

  Matrix m = {
    {  1,  2,  3,  4 },
    {  5,  6,  7,  8 },
    {  9, 10, 11, 12 },
    { 13, 14, 15, 16 } };
  Matrix mTmp;

  printf( "//-----------------------------------------------------------------\n" );
  printf( "-- p ----------\n" );
  dumpPointT( p );

  scalarMultPoint( 3, p, pTmp );
  printf( "\n-- 3 · p ------\n" );
  dumpPointT( pTmp );

  printf( "\n-- Z4 ---------\n" );
  dumpMatrix( Z4 );

  printf( "\n-- I4 ---------\n" );
  dumpMatrix( I4 );

  printf( "\n-- m ----------\n" );
  dumpMatrix( m );

  printf( "\n//-----------------------------------------------------------------\n" );
  multMatrixPoint( I4, p, pTmp );
  printf( "-- I4 · p -----\n" );
  dumpPointT( pTmp );

  multMatrixPoint( m, p, pTmp );
  printf( "\n-- m · p ------\n" );
  dumpPointT( pTmp );

  printf( "\n//-----------------------------------------------------------------\n" );
  addMatrix( m, m, mTmp );
  printf( "-- m + m ------\n" );
  dumpMatrix( mTmp );

  scalarMultMatrix( 3, m, mTmp );
  printf( "\n-- 3 · m ------\n" );
  dumpMatrix( mTmp );

  multMatrix( I4, m, mTmp );
  printf( "\n-- I4 · m -----\n" );
  dumpMatrix( mTmp );

  multMatrix( m, I4, mTmp );
  printf( "\n-- m · I4 -----\n" );
  dumpMatrix( mTmp );

  multMatrix( m, m, mTmp );
  printf( "\n-- m · m ------\n" );
  dumpMatrix( mTmp );

  printf( "\n//-----------------------------------------------------------------\n" );

  return 0;
}

//----------------------------------------------------------------
