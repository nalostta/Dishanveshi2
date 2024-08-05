#ifndef __DISHANVESHI_INCLUDED_DSVI_MATH_POINT_H__
#define __DISHANVESHI_INCLUDED_DSVI_MATH_POINT_H__

#include "common.h"

#define isPt_MT(point) (((point.x<dsvi_epsilonF)&&(point.y<dsvi_epsilonF))? enBoolean_True:enBoolean_False)

EnBoolean_t isPt_Valid(const dsvi_PointR_t* pt);

//seems to be working but proper testing still required i guess...
dsvi_status_t dsvi_math_3ptCrossProduct(const dsvi_PointR_t* refPt,const dsvi_PointR_t* p1,const dsvi_PointR_t* p2,real_t* result); //(p1)x(P2)

#endif // __DISHANVESHI_INCLUDED_DSVI_MATH_POINT_H__
