#ifndef __DISHANVESHI_INCLUDED_TYPES_H__
#define __DISHANVESHI_INCLUDED_TYPES_H__

#include "defs.h"
#include <stdint.h>

#if DSVI_REAL_IS_FLOAT
typedef float real_t;
#else
typedef double real_t;
#endif // DSVI_REAL_IS_FLOAT

#ifndef enBoolIsDefined
#define enBoolIsDefined
typedef enum
{
    enBoolean_False = 0,
    enBoolean_True = 1
}EnBoolean_t;
#endif


typedef int32_t dsvi_status_t;
#endif // __DISHANVESHI_INCLUDED_TYPES_H__
