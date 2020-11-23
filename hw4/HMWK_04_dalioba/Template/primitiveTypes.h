// Dalio, Brian A.
// dalioba
// 2020-09-18
//----------------------------------------------------------------
#if !defined( __PRIMITIVE_TYPES_H__ )
#define __PRIMITIVE_TYPES_H__

//----------------------------------------------------------------
#include <float.h>

#if DBL_MANT_DIG == 53
typedef double FLOAT64;

#elif LDBL_MANT_DIG == 53
typedef long double FLOAT64;

#else
#error "Can't determine which type to use for FLOAT64?"

#endif

//----------------------------------------------------------------
#endif
