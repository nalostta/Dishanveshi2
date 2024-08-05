#ifndef __DISHANVESHI_INCLUDED_DSVI_MATH_COMMON_H__
#define __DISHANVESHI_INCLUDED_DSVI_MATH_COMMON_H__

#include <stddef.h>
#include "types.h"

#define MaxOf2(num1,num2)   ((num1>num2)?    num1:num2)  //in case num1==num2, then both the macros below will not return the same number
#define MinOf2(num1,num2)   ((num1>num2)?    num2:num1)
#define GetRelErr(num1,num2)  ((MaxOf2(num1,num2)==0)?   1:(1-((real_t)MinOf2(num1,num2)/(real_t)MaxOf2(num1,num2))))
#define ABSF(a) (a > 0.0f ? a :-a)


//misc fns
int areEqual(float a, float b);

//dsvi_status_t dsvi_math_lineToPtCrossProduct(const dsvi_LineR_t* l1,const dsvi_PointR_t* pt,real_t* result);
#endif // __DISHANVESHI_INCLUDED_DSVI_MATH_COMMON_H__


