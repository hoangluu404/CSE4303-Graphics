// Luu, Hoang
// hxl9998
// 2020-11-16
//----------------------------------------------------------------
#include <stdio.h>
#include <stdlib.h>

#include "pipeline.h"

//----------------------------------------------------------------
#define INSIDE    (0)
#define LEFT      (1 << 0)
#define ABOVE     (1 << 1)
#define RIGHT     (1 << 2)
#define BELOW     (1 << 3)

static int _zone( int x, int y, Parameters *paramsP );

//----------------------------------------------------------------
int clipLine( int *p1x, int *p1y, int *p2x, int *p2y, Parameters *paramsP )
{
  
  while ( 1 ) {

    // compute zones for p1 and p2
    int code1 = _zone( *p1x, *p1y, paramsP );
    int code2 = _zone( *p2x, *p2y, paramsP );

    // if inside, accept
    if( code1 == INSIDE && code2 == INSIDE ){
      return 1;
    }

    // if trivial reject, reject
    if( (code1 & code2) != INSIDE ){
      return 0;
    }

    // init variables
    FLOAT64 x,y;
    FLOAT64 x1 = (FLOAT64)*p1x;
    FLOAT64 y1 = (FLOAT64)*p1y;
    FLOAT64 x2 = (FLOAT64)*p2x;
    FLOAT64 y2 = (FLOAT64)*p2y;
    FLOAT64 xMax = (FLOAT64)paramsP->sXmax;
    FLOAT64 yMax = (FLOAT64)paramsP->sYmax;
    FLOAT64 xMin = (FLOAT64)paramsP->sXmin;
    FLOAT64 yMin = (FLOAT64)paramsP->sYmin;

    int outCode;
    if( code1 == INSIDE )
      outCode = code2;
    else 
      outCode = code1;

    if(outCode & ABOVE){
      x = x1 + (x2-x1)*(yMax-y1)/(y2-y1);
      y = yMax;
    } else if( outCode & BELOW){
      x = x1 + (x2-x1)*(yMin-y1)/(y2-y1);
      y = yMin;
    } else if(outCode & RIGHT){
      x = xMax;
      y = y1 + (y2-y1)*(xMax-x1)/(x2-x1);
    } else if(outCode & LEFT){
      x = xMin;
      y = y1 + (y2-y1)*(xMin-x1)/(x2-x1);
    } else {
      fprintf(stderr, "ERROR: point not in this scope\n");
    }

    if(outCode == code1){
      *p1x = ROUND(x);
      *p1y = ROUND(y);
    } else if(outCode == code2){
      *p2x = ROUND(x);
      *p2y = ROUND(y);
    }

  }
  
}

//----------------------------------------------------------------
static int _zone( int x, int y, Parameters *paramsP )
{ 

  int code = INSIDE;

  // Left and Right
  if(x < paramsP->sXmin)
    code |= LEFT;
  else if(x > paramsP->sXmax)
    code |= RIGHT;

  // Above and Below
  if(y < paramsP->sYmin)
    code |= BELOW;
  else if(y > paramsP->sYmax)
    code |= ABOVE;

  return code;
}

//----------------------------------------------------------------
