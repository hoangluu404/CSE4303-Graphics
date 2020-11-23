// Dalio, Brian A.
// dalioba
// 2020-10-04
//----------------------------------------------------------------
#if !defined( __TRANSFORMS_H__ )
#define __TRANSFORMS_H__

//----------------------------------------------------------------
#include <math.h>

#include "matrix.h"

#if !defined( RAD2DEG )
#define RAD2DEG(r) (M_1_PI*180.0*(r))
#endif

#if !defined( DEG2RAD )
#define DEG2RAD(d) (M_PI*(d)/180.0)
#endif

//----------------------------------------------------------------
extern void normalizePoint( Point p1, Point p2 );

extern void projectParallelMatrix(
  FLOAT64 xmin, FLOAT64 xmax,
  FLOAT64 ymin, FLOAT64 ymax,
  int width, int height,
  Matrix xform );

extern void projectPerspectiveMatrix(
  FLOAT64 d,
  FLOAT64 xmin, FLOAT64 xmax,
  FLOAT64 ymin, FLOAT64 ymax,
  int width, int height,
  Matrix xform );

extern void rotateXMatrix( FLOAT64 theta, Matrix xform );
extern void rotateYMatrix( FLOAT64 theta, Matrix xform );
extern void rotateZMatrix( FLOAT64 theta, Matrix xform );

extern void scaleMatrix( FLOAT64 sx, FLOAT64 sy, FLOAT64 sz, Matrix xform );

extern void shearXMatrix( FLOAT64 hxy, FLOAT64 hxz, Matrix xform );
extern void shearYMatrix( FLOAT64 hyx, FLOAT64 hyz, Matrix xform );
extern void shearZMatrix( FLOAT64 hzx, FLOAT64 hzy, Matrix xform );
extern void shearXYZMatrix(
  FLOAT64 hxy, FLOAT64 hxz,
  FLOAT64 hyx, FLOAT64 hyz,
  FLOAT64 hzx, FLOAT64 hzy,
  Matrix xform );

extern void translateMatrix( FLOAT64 tx, FLOAT64 ty, FLOAT64 tz, Matrix xform );

//----------------------------------------------------------------
#endif
