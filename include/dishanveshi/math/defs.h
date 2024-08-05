#ifndef __DISHANVESHI_INCLUDED_DSVI_MATH_COMMON_H__
#define __DISHANVESHI_INCLUDED_DSVI_MATH_COMMON_H__

#include <dishanveshi/defs.h>

/*
                           *(Beyond)
                         .
                        .
                 (End) *
                      /       *(Right)
                     /
        *(Left)     *(OnSegment)
                   /
                  *(Start)
                 .
                .
               .
              *(Behind)
*/

typedef enum
{
    Left = -1,
    Onsegment = 1,
    Start   =   OnSegment | 1<<1,
    End     =   OnSegment | 1<<2,
    Beyond  =   1<<3,
    Behind  =   1<<4,
    Right   =   1<<5
}enDsviPointSegmentClassification;

#endif // __DISHANVESHI_INCLUDED_DSVI_MATH_COMMON_H__
