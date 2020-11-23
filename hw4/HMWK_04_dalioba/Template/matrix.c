// Dalio, Brian A.
// dalioba
// 2020-10-04
//----------------------------------------------------------------
#include <stdio.h>

#include "matrix.h"

//----------------------------------------------------------------
const Matrix I4 = {
    { 1, 0, 0, 0 },
    { 0, 1, 0, 0 },
    { 0, 0, 1, 0 },
    { 0, 0, 0, 1 } };

const Matrix Z4 = {
    { 0, 0, 0, 0 },
    { 0, 0, 0, 0 },
    { 0, 0, 0, 0 },
    { 0, 0, 0, 0 } };

//----------------------------------------------------------------
void addMatrix( const Matrix a, const Matrix b, Matrix c )
{
  FLOAT64 *ap = (FLOAT64 *) a;
  FLOAT64 *bp = (FLOAT64 *) b;
  FLOAT64 *cp = (FLOAT64 *) c;

  for ( int i = 0; i < 16; i++ ) {
    *cp++ = *ap++ + *bp++;
  }
}

//----------------------------------------------------------------
void copyMatrix( const Matrix a, Matrix b )
{
  FLOAT64 *ap = (FLOAT64 *) a;
  FLOAT64 *bp = (FLOAT64 *) b;

  for ( int i = 0; i < 16; i++ ) {
    *bp++ = *ap++;
  }
}

//----------------------------------------------------------------
void multMatrix( const Matrix a, const Matrix b, Matrix c )
{
  for ( int row = 0; row < 4; row++ ) {
    for ( int col = 0; col < 4; col++ ) {
      c[row][col] =
        a[row][0] * b[0][col] +
        a[row][1] * b[1][col] +
        a[row][2] * b[2][col] +
        a[row][3] * b[3][col];
    }
  }
}

//----------------------------------------------------------------
void multMatrixPoint( const Matrix a, const Point b, Point c )
{
  for ( int row = 0; row < 4; row++ ) {
    c[row] =
      a[row][0] * b[0] +
      a[row][1] * b[1] +
      a[row][2] * b[2] +
      a[row][3] * b[3];
  }
}

//----------------------------------------------------------------
void scalarMultMatrix( FLOAT64 s, const Matrix a, Matrix b )
{
  FLOAT64 *ap = (FLOAT64 *) a;
  FLOAT64 *bp = (FLOAT64 *) b;

  for ( int i = 0; i < 16; i++ ) {
    *bp++ = s * *ap++;
  }
}

//----------------------------------------------------------------
void scalarMultPoint( FLOAT64 s, const Point a, Point b )
{
  b[0] = s * a[0];
  b[1] = s * a[1];
  b[2] = s * a[2];
  b[3] = s * a[3];
}

//----------------------------------------------------------------
void dumpMatrix( const Matrix m ) { fdumpfMatrix( stdout, m, ELEMENT_DEFAULT_FORMAT ); }
void dumpfMatrix( const Matrix m, const char *fmt ) { fdumpfMatrix( stdout, m, fmt ); }
void fdumpMatrix( FILE *fp, const Matrix m ) { fdumpfMatrix( fp, m, ELEMENT_DEFAULT_FORMAT ); }

void fdumpfMatrix( FILE *fp, const Matrix m, const char *fmt )
{
  for ( int r = 0; r < 4; r++ ) {
    fprintf( fp, fmt, m[r][0] );
    fputc( ' ', fp );
    fprintf( fp, fmt, m[r][1] );
    fputc( ' ', fp );
    fprintf( fp, fmt, m[r][2] );
    fputc( ' ', fp );
    fprintf( fp, fmt, m[r][3] );
    fputc( '\n', fp );
  }
}

//----------------------------------------------------------------
void dumpPoint( const Point p ) { fdumpfPoint( stdout, p, ELEMENT_DEFAULT_FORMAT ); }
void dumpfPoint( const Point p, const char *fmt ) { fdumpfPoint( stdout, p, fmt ); }
void fdumpPoint( FILE *fp, const Point p ) { fdumpfPoint( fp, p, ELEMENT_DEFAULT_FORMAT ); }

void fdumpfPoint( FILE *fp, const Point p, const char *fmt )
{
  fprintf( fp, fmt, p[0] );
  fputc( '\n', fp );
  fprintf( fp, fmt, p[1] );
  fputc( '\n', fp );
  fprintf( fp, fmt, p[2] );
  fputc( '\n', fp );
  fprintf( fp, fmt, p[3] );
  fputc( '\n', fp );
}

//----------------------------------------------------------------
void dumpPointT( const Point p ) { fdumpfPointT( stdout, p, ELEMENT_DEFAULT_FORMAT ); }
void dumpfPointT( const Point p, const char *fmt ) { fdumpfPointT( stdout, p, fmt ); }
void fdumpPointT( FILE *fp, const Point p ) { fdumpfPointT( fp, p, ELEMENT_DEFAULT_FORMAT ); }

void fdumpfPointT( FILE *fp, const Point p, const char *fmt )
{
  fputs( "[ ", fp );
  fprintf( fp, fmt, p[0] );
  fputs( ", ", fp );
  fprintf( fp, fmt, p[1] );
  fputs( ", ", fp );
  fprintf( fp, fmt, p[2] );
  fputs( ", ", fp );
  fprintf( fp, fmt, p[3] );
  fputs( " ]T\n", fp );
}

//----------------------------------------------------------------
