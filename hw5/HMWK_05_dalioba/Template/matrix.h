// Dalio, Brian A.
// dalioba
// 2020-10-04
//----------------------------------------------------------------
#if !defined( __MATRIX_H__ )
#define __MATRIX_H__

//----------------------------------------------------------------
#include <stdio.h>

#include "primitiveTypes.h"

//----------------------------------------------------------------
typedef FLOAT64 Matrix[4][4];

typedef FLOAT64 Point[4];

//----------------------------------------------------------------
extern const Matrix I4;
extern const Matrix Z4;

//----------------------------------------------------------------
extern void addMatrix( const Matrix a, const Matrix b, Matrix c );
extern void copyMatrix( const Matrix a, Matrix b );
extern void multMatrix( const Matrix a, const Matrix b, Matrix c );
extern void multMatrixPoint( const Matrix a, const Point b, Point c );
extern void scalarMultMatrix( const FLOAT64 s, const Matrix a, Matrix b );
extern void scalarMultPoint( const FLOAT64 s, const Point a, Point b );

//----------------------------------------------------------------
#define ELEMENT_DEFAULT_FORMAT ("%15g")

extern void dumpMatrix( const Matrix m );
extern void dumpfMatrix( const Matrix m, const char *fmt );
extern void fdumpMatrix( FILE *fp, const Matrix m );
extern void fdumpfMatrix( FILE *fp, const Matrix m, const char *fmt );

extern void dumpPoint( const Point p );
extern void dumpfPoint( const Point p, const char *fmt );
extern void fdumpPoint( FILE *fp, const Point p );
extern void fdumpfPoint( FILE *fp, const Point p, const char *fmt );

extern void dumpPointT( const Point p );
extern void dumpfPointT( const Point p, const char *fmt );
extern void fdumpPointT( FILE *fp, const Point p );
extern void fdumpfPointT( FILE *fp, const Point p, const char *fmt );

//----------------------------------------------------------------
#endif
