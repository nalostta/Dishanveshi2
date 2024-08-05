#ifndef __DISHANVESHI_INCLUDED_DSVI_MATH_TYPES_H__
#define __DISHANVESHI_INCLUDED_DSVI_MATH_TYPES_H__

#include <dishanveshi/math/defs.h>
#include <dishanveshi/types.h>

//2D-point
typedef struct
{
    int x;
    int y;
}dsvi_Point_t;

typedef struct
{
    real_t x;
    real_t y;
}dsvi_PointR_t;

typedef struct
{
    dsvi_Point_t head;
    dsvi_Point_t tail;
}dsvi_Line_t;

typedef struct
{
    dsvi_PointR_t head;
    dsvi_PointR_t tail;
}dsvi_LineR_t;
#endif // __DISHANVESHI_INCLUDED_DSVI_MATH_TYPES_H__
