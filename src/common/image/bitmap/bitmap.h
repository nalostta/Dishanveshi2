#ifndef __DISHANVESHI_IMAGE_BITMAP_H
#define __DISHANVESHI_IMAGE_BITMAP_H

#include <string.h>
#include <dishanveshi/image/types.h>
#include <dishanveshi/image/image.h>
#include "defs.h"

typedef struct
{
    uint32_t filesize;
    uint32_t dataoffset;
    //
    int32_t  width;
    int32_t  height;
    uint16_t bitsPerPixel;
    uint32_t imageSize;
}BitmapHeader_t;

dsvi_Image_t* dsvi_image_bitmap_read(const char* BitmapFilename,dsvi_status_t* status);
dsvi_status_t dsvi_image_bitmap_write(const dsvi_Image_t* SrcImageHandle,const char* FileName); //INCOMPLETE FN

///module properties
BitmapHeader_t* dsvi_image_bitmap_getheader(const char* buf_header,dsvi_status_t* status);

#endif // __DISHANVESHI_IMAGE_BITMAP_H
