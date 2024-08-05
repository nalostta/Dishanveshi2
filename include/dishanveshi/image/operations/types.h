#ifndef __DISHANVESHI_INCLUDED_DSVI_IMAGE_OPERATIONS_TYPES_H__
#define __DISHANVESHI_INCLUDED_DSVI_IMAGE_OPERATIONS_TYPES_H__


#include <dishanveshi/image/types.h>
#include <dishanveshi/image/defs.h>
#include "defs.h"
#include <stdlib.h> //have to remove this?

typedef struct
{
    uint16_t  width;
    uint16_t  height;
    int16_t*  el;
}dsvi_Matrix_t;

typedef struct
{
    uint16_t  width;
    uint16_t  height;
    int16_t*  el; //change this to real_t?
}dsvi_Kernel_t;

typedef struct
{
    uint16_t  width;
    uint16_t  height;
    real_t*   el;
}dsvi_RealMatrix_t;

typedef struct
{
    uint16_t  width;
    uint16_t  height;
    real_t*   el;
}dsvi_RealKernel_t;

#ifdef  DSVI_PUBLIC_HISTOGRAM
struct _dsvi_histogramInfo_t
{
    uint8_t bins;
    uint8_t* bounds;
};

struct _dsvi_histogram_t
{
    //source data to be analyzed is of the type uint8_t
    uint8_t     bins;       //NOTE : number of bins cannot exceed data-type range, which here is 256!
    uint8_t*    bounds;
    uint32_t*   data;
};
#endif  //DSVI_PUBLIC_HISTOGRAM

typedef struct _dsvi_histogram_t       dsvi_histogram_t;
typedef struct _dsvi_histogramInfo_t   dsvi_histogramInfo_t;



//template
typedef enum
{
    enTrapezium
}enShape;

typedef struct
{
    uint16_t x;
    uint16_t y;
    uint16_t* parameters;
    enShape  templateShape;
}dsvi_template_t;

//color format
typedef struct
{
    uint8_t r;
    uint8_t g;
    uint8_t b;
}dsvi_colorFormatRGB;

typedef struct
{
    uint8_t h;
    uint8_t s;
    uint8_t v;
}dsvi_colorFormatHSV;

typedef struct
{
    uint8_t h;
    uint8_t s;
    uint8_t i;
}dsvi_colorFormatHSI;

//queue for CCL
struct dsvi_queue_t
{
    int32_t    data;
    struct dsvi_queue_t* next;
};

typedef struct dsvi_queue_t dsvi_Queue_t;

//
typedef struct
{
    uint16_t length;
    uint8_t* nextRelative;
    uint32_t* counts;
}dsvi_regionBoundary_t;

#endif // __DISHANVESHI_INCLUDED_DSVI_IMAGE_OPERATIONS_TYPES_H__

