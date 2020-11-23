// Dalio, Brian A.
// dalioba
// 2020-11-21
//----------------------------------------------------------------
#if !defined( __PIPELINE_H__ )
#define __PIPELINE_H__

//----------------------------------------------------------------
#include <math.h>

#include "matrix.h"
#include "primitiveTypes.h"

//----------------------------------------------------------------
#define ERROR     (-1)
#define OK        (0)

//----------------------------------------------------------------
#define DEG2RAD( d ) ((d)*M_PI/180.0)
#define RAD2DEG( r ) ((r)*180.0/M_PI)

#define ROUND( f ) ((int)((f)+0.5))

//----------------------------------------------------------------
typedef struct {
  int invisible;
  int v1, v2, v3;
} Face;

//----------------------------------------------------------------
typedef struct {
  // Euler angles (in radians!)
  FLOAT64 phi, theta, psi;

  // Camera viewpoint (0.0 if parallel projection)
  FLOAT64 viewpoint;

  // World view
  FLOAT64 wXmin, wXmax;
  FLOAT64 wYmin, wYmax;

  // Screen canvas size
  int width;
  int height;

  // Screen clip region
  int sXmin, sXmax;
  int sYmin, sYmax;

  // Object info
  int    numPoints;
  int    numFaces;

  // Whether or not we should cull invisible triangles.
  int    cull;

  Point *points;
  Face  *faces;
} Parameters;

//----------------------------------------------------------------
void backfaceCull( Parameters *paramsP, int noisy );
int  clipLine( int *p1x, int *p1y, int *p2x, int *p2y, Parameters *paramsP );
int  display( char *fName, Parameters *paramsP );
void eulerRotate( Parameters *paramsP );
int  loadObject( char *fName, Parameters *paramsP );
int  loadParams( char *fName, Parameters *paramsP );
void project( Parameters *paramsP );
void unloadObject( Parameters *paramsP );

//----------------------------------------------------------------
#endif
