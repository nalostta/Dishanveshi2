#ifndef __DISHANVESHI_INCLUDED_DSVI_MATH_LINE_H__
#define __DISHANVESHI_INCLUDED_DSVI_MATH_LINE_H__

#include <stddef.h>
#include "point.h"

#define isLine_MT(line)  ((areEqual(line->head.x,line->tail.x)&&areEqual(line->head.y,line->tail.y))? enBoolean_True:enBoolean_False)

EnBoolean_t isLine_Valid( dsvi_LineR_t* line);
real_t      dsvi_math_getSlope( dsvi_Line_t* line,dsvi_status_t* status);
real_t      dsvi_math_getSlopeR( dsvi_LineR_t* line,dsvi_status_t* status);

EnBoolean_t dsvi_math_checkXBoundaryOverlap( dsvi_LineR_t* line1, dsvi_LineR_t* line2,dsvi_status_t* status);

dsvi_status_t dsvi_math_lineSegment2D_intersects( dsvi_LineR_t* l1,  dsvi_LineR_t* l2,EnBoolean_t* result);
dsvi_status_t dsvi_math_lineSegment2D_intersect(dsvi_LineR_t* l1,dsvi_LineR_t* l2,dsvi_PointR_t* result);

#endif // __DISHANVESHI_INCLUDED_DSVI_MATH_LINE_H__
