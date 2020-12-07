// Dalio, Brian A.
// dalioba
// 2020-12-03
//----------------------------------------------------------------
#if !defined( __PIPELINE_H__ )
#define __PIPELINE_H__

//----------------------------------------------------------------
#include <math.h>
#include <stdio.h>

#include "matrix.h"
#include "primitiveTypes.h"

//----------------------------------------------------------------
#define ERROR     (-1)
#define OK        (0)

enum {
  REGULAR,
  PATCHES
};

//----------------------------------------------------------------
#define DEG2RAD( d ) ((d)*M_PI/180.0)
#define RAD2DEG( r ) ((r)*180.0/M_PI)

#define ROUND( f ) ((int)((f)+0.5))

//----------------------------------------------------------------
typedef struct {
  int invisible;
  int v1, v2, v3;
} Face;

typedef int Patch[16];

//----------------------------------------------------------------
typedef struct {
  // Whether to print debugging messages or not.
  int debug;

  // Bézier resolution (0 if no generation of Bézier points)
  int resolution;

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
  int    numPatches;

  // Whether or not we should cull invisible triangles.
  int    cull;

  // The explicit vertices, faces, and patches of the object.
  Point *points;
  Face  *faces;
  Patch *patches;

  // The generated Bézier points of a patch.  (We do one patch at
  //  a time, so we don't need any more space than for one patch.)
  Point *bPoints;
  int    numBPoints;

  // The generated Bézier faces of a patch.  (We generate the
  //  faces once since all patches have the same faces.)
  Face  *bFaces;
  int    numBFaces;
} Parameters;

//----------------------------------------------------------------
void backfaceCull( int mode, Parameters *paramsP, int noisy );
int  clipLine( int *p1x, int *p1y, int *p2x, int *p2y, Parameters *paramsP );
void display( int mode, FILE *fp, Parameters *paramsP );
void eulerRotate( Parameters *paramsP );
void generatePatches( FILE *fp, Parameters *paramsP );
int  loadObject( char *fName, Parameters *paramsP );
int  loadParams( char *fName, Parameters *paramsP );
void project( int mode, Parameters *paramsP );
void sdfHeader( FILE *fp, Parameters *paramsP );
void unloadObject( Parameters *paramsP );

//----------------------------------------------------------------
#endif
