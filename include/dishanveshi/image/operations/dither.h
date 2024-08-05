#ifndef __DISHANVESHI_INCLUDED_DSVI_IMAGE_OPERATIONS_DITHER_H_
#define __DISHANVESHI_INCLUDED_DSVI_IMAGE_OPERATIONS_DITHER_H_

#include <dishanveshi/image/operations/types.h>
#include <dishanveshi/image/image.h>

dsvi_Image_t* dsvi_image_dither(const dsvi_Image_t* srcImage,uint8_t threshold, dsvi_status_t* status);

#endif // __DISHANVESHI_INCLUDED_DSVI_IMAGE_OPERATIONS_DITHER_H_
