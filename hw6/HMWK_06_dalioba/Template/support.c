// Dalio, Brian A.
// dalioba
// 2020-12-03
//----------------------------------------------------------------
#include <ctype.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "pipeline.h"

//----------------------------------------------------------------
int loadParams( char *fName, Parameters *paramsP )
{
  printf(
    "//----------------------------------------\n"
    "// Loading parameters from \"%s\" ...\n",
    fName );

  FILE *fp = fopen( fName, "r" );
  if ( fp == NULL ) {
    printf( "// Unable to open \"%s\" for read, (%d) %s.\n",
      fName, errno, strerror( errno ) );

    return ERROR;
  }

  //--------------------------------------
  char   *line    = NULL;
  size_t  len     = 0;
  size_t  read;
  int     lineNum = 0;

  while ( ( read = getline( &line, &len, fp ) ) != -1ul ) {
    lineNum++;

    char *ptr = line;

    while ( *ptr && isspace( *ptr ) ) ptr++;

    if ( *ptr == 'b' ) {
      int conv = sscanf( ptr+1, " %d",
        &paramsP->cull );

      if ( conv != 1 ) {
        printf( "// Unable to load cull status at line %d.", lineNum );
      }

      continue;

    } else if ( *ptr == 'c' ) {
      int conv = sscanf( ptr+1, " %d %d",
        &paramsP->width, &paramsP->height );

      if ( conv != 2 ) {
        printf( "// Unable to load canvas size at line %d.", lineNum );
      }

      continue;

    } else if ( *ptr == 'd' ) {
      int conv = sscanf( ptr+1, " %lf",
        &paramsP->viewpoint );

      if ( conv != 1 ) {
        printf( "// Unable to load camera viewpoint at line %d.", lineNum );
      }

      continue;

    } else if ( *ptr == 'e' ) {
      int conv = sscanf( ptr+1, " %lf %lf %lf",
        &paramsP->phi, &paramsP->theta, &paramsP->psi );
      paramsP->phi   = DEG2RAD( paramsP->phi );
      paramsP->theta = DEG2RAD( paramsP->theta );
      paramsP->psi   = DEG2RAD( paramsP->psi );

      if ( conv != 3 ) {
        printf( "// Unable to load Euler angles at line %d.", lineNum );
      }

      continue;

    } else if ( *ptr == 'w' ) {
      int conv = sscanf( ptr+1, " %lf %lf %lf %lf",
        &paramsP->wXmin, &paramsP->wXmax,
        &paramsP->wYmin, &paramsP->wYmax );

      if ( conv != 4 ) {
        printf( "// Unable to load world view range at line %d.", lineNum );
      }

      continue;

    } else if ( *ptr == 'r' ) {
      int conv = sscanf( ptr+1, " %d",
        &paramsP->resolution );

      if ( conv != 1 ) {
        printf( "// Unable to load Bézier resolution at line %d.", lineNum );
      }

      continue;

    } else if ( *ptr == 's' ) {
      int conv = sscanf( ptr+1, " %d %d %d %d",
        &paramsP->sXmin, &paramsP->sXmax,
        &paramsP->sYmin, &paramsP->sYmax );

      if ( conv != 4 ) {
        printf( "// Unable to load screen clip range at line %d.", lineNum );
      }

      continue;

    } else if ( *ptr == 'x' ) {
      int conv = sscanf( ptr+1, " %d",
        &paramsP->debug );

      if ( conv != 1 ) {
        printf( "// Unable to load debug status at line %d.", lineNum );
      }

      continue;

    } else if ( *ptr == '#' || *ptr == '\0' ) {
      // Comment or blank line -- ignore

    } else {
      printf( "// Line %d not understood:\n%s\n", lineNum, ptr );
    }
  }

  //--------------------------------------
  // Allocate space for the generated Bézier points and faces.
  if ( paramsP->resolution != 0 ) {
    paramsP->numBPoints = paramsP->resolution*paramsP->resolution;

    paramsP->bPoints = (Point *) calloc(
      paramsP->numBPoints, sizeof( Point ) );
    if ( paramsP->bPoints == NULL ) {
      printf( "// Couldn't allocate Bézier Point array.\n" );
      return ERROR;
    }

    paramsP->numBFaces = 2*(paramsP->resolution-1)*(paramsP->resolution-1);

    paramsP->bFaces = (Face *) calloc(
      paramsP->numBFaces, sizeof( Face ) );
    if ( paramsP->bFaces == NULL ) {
      printf( "// Couldn't allocate Bézier Face array.\n" );
      return ERROR;
    }
  }

  //--------------------------------------
  // Display what we read as a check.
  printf( "// Params from \"%s\"\n", fName );

  printf( "//   Bézier resolution : %d\n",
    paramsP->resolution );

  printf( "//   Euler angles      : ϕ %g°, θ %g°, ψ %g°\n",
    RAD2DEG( paramsP->phi ),
    RAD2DEG( paramsP->theta ),
    RAD2DEG( paramsP->psi ) );

  if ( paramsP->viewpoint == 0.0 ) {
    printf( "//   Camera distance   : <parallel projection>\n" );

  } else {
    printf( "//   Camera distance   : %g\n", paramsP->viewpoint );
  }

  printf( "//   World view        : (%g, %g) .. (%g, %g)\n",
    paramsP->wXmin, paramsP->wYmin,
    paramsP->wXmax, paramsP->wYmax );

  printf( "//   Canvas size       : (W %d, H %d)\n",
    paramsP->width, paramsP->height );

  printf( "//   Screen clip       : (%d, %d) .. (%d, %d)\n",
    paramsP->sXmin, paramsP->sYmin,
    paramsP->sXmax, paramsP->sYmax );

  printf( "//   Cull              : %s\n",
    paramsP->cull ? "Yes" : "No" );

  printf( "//   Debug             : %s\n",
    paramsP->debug ? "Yes" : "No" );

  //--------------------------------------
  // Free/close local items.
  if ( line != NULL ) { free( line ); }
  fclose( fp );

  // All done, w/no error!
  return OK;
}

//----------------------------------------------------------------
int loadObject( char *fName, Parameters *paramsP )
{
  printf(
    "//----------------------------------------\n"
    "// Loading object from \"%s\" ...\n",
    fName );

  FILE *fp = fopen( fName, "r" );
  if ( fp == NULL ) {
    printf( "// Unable to open \"%s\" for read, (%d) %s.\n",
      fName, errno, strerror( errno ) );

    return ERROR;
  }

  //--------------------------------------
  int     numVertices = 0;
  int     numFaces    = 0;
  int     numPatches  = 0;
  char   *line        = NULL;
  size_t  len         = 0;
  size_t  read;

  while ( ( read = getline( &line, &len, fp ) ) != -1ul ) {
    char *ptr = line;

    while ( *ptr && isspace( *ptr ) ) ptr++;
    if ( *ptr == 'v' ) ++numVertices;
    if ( *ptr == 'f' ) ++numFaces;
    if ( *ptr == 'p' ) ++numPatches;
  }

  rewind( fp );

  //--------------------------------------
  // Allocate space for the vertices, faces, and patches.
  paramsP->points = (Point *) calloc( numVertices, sizeof( Point ) );
  if ( paramsP->points == NULL ) {
    printf( "// Couldn't allocate Point array.\n" );
    return ERROR;
  }
  paramsP->numPoints = numVertices;

  paramsP->faces  = (Face *)  calloc( numFaces, sizeof( Face ) );
  if ( paramsP->faces == NULL ) {
    printf( "// Couldn't allocate Face array.\n" );
    free( paramsP->points );
    return ERROR;
  }
  paramsP->numFaces = numFaces;

  paramsP->patches = (Patch *)  calloc( numPatches, sizeof( Patch ) );
  if ( paramsP->patches == NULL ) {
    printf( "// Couldn't allocate Patch array.\n" );
    free( paramsP->points );
    free( paramsP->faces );
    return ERROR;
  }
  paramsP->numPatches = numPatches;

  //--------------------------------------
  // Load the vertices, faces, and patches.
  Point *point   = paramsP->points;
  Face  *face    = paramsP->faces;
  Patch *patch   = paramsP->patches;
  int    lineNum = 0;

  while ( ( read = getline( &line, &len, fp ) ) != -1ul ) {
    lineNum++;

    char *ptr = line;

    while ( *ptr && isspace( *ptr ) ) ptr++;

    if ( *ptr == 'v' ) {
      int conv = sscanf( ptr+1, " %lf %lf %lf",
        &(*point)[0], &(*point)[1], &(*point)[2] );

      if ( conv != 3 ) {
        printf( "// Unable to load vertex at line %d.", lineNum );
      }

      (*point)[3] = 1;  // The w coordinate

      point++;
      continue;

    } else if ( *ptr == 'f' ) {
      int conv = sscanf( ptr+1, " %d %d %d",
        &face->v1, &face->v2, &face->v3 );

      face->v1--;
      face->v2--;
      face->v3--;

      if ( conv != 3 ) {
        printf( "// Unable to load face at line %d.", lineNum );
      }

      face++;
      continue;

    } else if ( *ptr == 'p' ) {
      int conv = sscanf( ptr+1, " %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d",
        &(*patch)[ 0], &(*patch)[ 1], &(*patch)[ 2], &(*patch)[ 3],
        &(*patch)[ 4], &(*patch)[ 5], &(*patch)[ 6], &(*patch)[ 7],
        &(*patch)[ 8], &(*patch)[ 9], &(*patch)[10], &(*patch)[11],
        &(*patch)[12], &(*patch)[13], &(*patch)[14], &(*patch)[15] );

      for ( int i=0; i<16; i++ ) {
        (*patch)[i]--;
      }

      if ( conv != 16 ) {
        printf( "// Unable to load patch at line %d.", lineNum );
      }

      patch++;
      continue;

    } else if ( *ptr == '#' || *ptr == '\0' ) {
      // Comment or blank line -- ignore

    } else {
      printf( "// Line %d not understood:\n%s\n", lineNum, ptr );
    }
  }

  //--------------------------------------
  // Display what we read as a check.
  printf( "//   %d point%s, %d face%s, %d patch%s\n",
    paramsP->numPoints,
    paramsP->numPoints == 1 ? "" : "s",
    paramsP->numFaces,
    paramsP->numFaces == 1 ? "" : "s",
    paramsP->numPatches,
    paramsP->numPatches == 1 ? "" : "es" );

  //--------------------------------------
  // Free/close local items.
  if ( line != NULL ) { free( line ); }
  fclose( fp );

  // All done, w/no error!
  return OK;
}

//----------------------------------------------------------------
void unloadObject( Parameters *paramsP )
{
  if ( paramsP->faces != NULL ) {
    free( paramsP->faces );
    paramsP->faces    = NULL;
    paramsP->numFaces = 0;
  }

  if ( paramsP->points != NULL ) {
    free( paramsP->points );
    paramsP->points    = NULL;
    paramsP->numPoints = 0;
  }

  if ( paramsP->patches != NULL ) {
    free( paramsP->patches );
    paramsP->patches    = NULL;
    paramsP->numPatches = 0;
  }

  if ( paramsP->bPoints != NULL ) {
    free( paramsP->bPoints );
    paramsP->bPoints = NULL;
  }

  if ( paramsP->bFaces != NULL ) {
    free( paramsP->bFaces );
    paramsP->bFaces = NULL;
  }
}

//----------------------------------------------------------------
