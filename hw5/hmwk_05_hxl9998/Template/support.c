// Dalio, Brian A.
// dalioba
// 2020-11-21
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

    } else if ( *ptr == 's' ) {
      int conv = sscanf( ptr+1, " %d %d %d %d",
        &paramsP->sXmin, &paramsP->sXmax,
        &paramsP->sYmin, &paramsP->sYmax );

      if ( conv != 4 ) {
        printf( "// Unable to load screen clip range at line %d.", lineNum );
      }

      continue;

    } else if ( *ptr == '#' || *ptr == '\0' ) {
      // Comment or blank line -- ignore

    } else {
      printf( "// Line %d not understood:\n%s\n", lineNum, ptr );
    }
  }

  //--------------------------------------
  // Display what we read as a check.
  printf( "// Params from \"%s\"\n", fName );

  printf( "//   Euler angles    : ϕ %g°, θ %g°, ψ %g°\n",
    RAD2DEG( paramsP->phi ),
    RAD2DEG( paramsP->theta ),
    RAD2DEG( paramsP->psi ) );

  if ( paramsP->viewpoint == 0.0 ) {
    printf( "//   Camera distance : <parallel projection>\n" );

  } else {
    printf( "//   Camera distance : %g\n", paramsP->viewpoint );
  }

  printf( "//   World view      : (%g, %g) .. (%g, %g)\n",
    paramsP->wXmin, paramsP->wYmin,
    paramsP->wXmax, paramsP->wYmax );

  printf( "//   Canvas size     : (W %d, H %d)\n",
    paramsP->width, paramsP->height );

  printf( "//   Screen clip     : (%d, %d) .. (%d, %d)\n",
    paramsP->sXmin, paramsP->sYmin,
    paramsP->sXmax, paramsP->sYmax );

  printf( "//   Cull            : %s\n",
    paramsP->cull ? "Yes" : "No" );

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
  char   *line        = NULL;
  size_t  len         = 0;
  size_t  read;

  while ( ( read = getline( &line, &len, fp ) ) != -1ul ) {
    char *ptr = line;

    while ( *ptr && isspace( *ptr ) ) ptr++;
    if ( *ptr == 'v' ) ++numVertices;
    if ( *ptr == 'f' ) ++numFaces;
  }

  rewind( fp );

  //--------------------------------------
  // Allocate space for the vertices and faces.
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

  //--------------------------------------
  // Load the vertices and faces.
  Point *point   = paramsP->points;
  Face  *face    = paramsP->faces;
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

    } else if ( *ptr == '#' || *ptr == '\0' ) {
      // Comment or blank line -- ignore

    } else {
      printf( "// Line %d not understood:\n%s\n", lineNum, ptr );
    }
  }

  //--------------------------------------
  // Display what we read as a check.
  printf( "//   %d point%s, %d face%s\n",
    paramsP->numPoints,
    paramsP->numPoints == 1 ? "" : "s",
    paramsP->numFaces,
    paramsP->numFaces == 1 ? "" : "s" );

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
    paramsP->numFaces = 0;
  }

  if ( paramsP->points != NULL ) {
    free( paramsP->points );
    paramsP->numPoints = 0;
  }
}

//----------------------------------------------------------------
