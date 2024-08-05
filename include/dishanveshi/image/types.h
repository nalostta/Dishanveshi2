#ifndef __DISHANVESHI_INCLUDED_DSVI_IMAGE_TYPES_H__
#define __DISHANVESHI_INCLUDED_DSVI_IMAGE_TYPES_H__

#include <dishanveshi/types.h>
#include <stddef.h>
#include "defs.h"

//-------------labels----------------
typedef int16_t dsvi_Label_t;
//--------------------------------------------------

#ifdef DSVI_PUBLIC_IMAGE
typedef struct
{
    uint32_t  x;
    uint32_t  y;
    uint32_t  width;
    uint32_t  height;
}dsvi_ImageROI_t;

struct _dsvi_image_t
{
    uint8_t* data;
    uint16_t width;
    uint16_t height;
    uint32_t format;
    dsvi_Label_t* label; //data size should depend on input image size
    dsvi_ImageROI_t* roi;
};
#endif // DSVI_PUBLIC_IMAGE

typedef struct _dsvi_image_t dsvi_Image_t;


typedef void* (*ImageAlloc_fn)(size_t);
typedef void  (*ImageFree_fn)(void*);

typedef struct
{
    ImageAlloc_fn image_alloc;
    ImageFree_fn  image_free;
}DsviImageMemoryConfig_t;

typedef enum
{
    enDsviImagePixelFormat_BINARY,
    enDsviImagePixelFormat_DEFAULT,
    enDsviImagePixelFormat_RGB565,
    enDsviImagePixelFormat_RGB888,
    enDsviImagePixelFormat_HSV,
    enDsviImagePixelFormat_NOTSUPPORTED
}EnDsviImagePixelFormat_t;

typedef struct
{
    uint16_t                  width;
    uint16_t                  height;
    EnDsviImagePixelFormat_t  format;
    void*                     extra;
}ImageInfo_t;

#endif // __DISHANVESHI_INCLUDED_DSVI_IMAGE_TYPES_H__
