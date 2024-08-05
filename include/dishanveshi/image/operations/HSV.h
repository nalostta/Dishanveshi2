#ifndef __DISHANVESHI_INCLUDED_DSVI_IMAGE_OPERATIONS_HSV_H__
#define __DISHANVESHI_INCLUDED_DSVI_IMAGE_OPERATIONS_HSV_H__

#include <dishanveshi/image/image.h>
#include "types.h"

//the status returned by the 2 fns below aren't of much use,still they provide the functionality.
dsvi_status_t   dsvi_image_colorFormat_getHSV(const dsvi_colorFormatRGB* inRGB,dsvi_colorFormatHSV* outHSV);
dsvi_status_t   dsvi_image_colorFormat_getRGB(const dsvi_colorFormatHSV* inHSV,dsvi_colorFormatRGB* outRGB);
dsvi_status_t   dsvi_image_colorFormat_getHSV2(const dsvi_colorFormatRGB* inRGB,dsvi_colorFormatHSV* outHSV);
dsvi_status_t   dsvi_image_colorFormat_getRGB2(const dsvi_colorFormatHSV* inHSV,dsvi_colorFormatRGB* outRGB);
dsvi_Image_t*   dsvi_image_getHSV(const dsvi_Image_t* srcImage,dsvi_status_t* status);
dsvi_Image_t*   dsvi_image_getRGB(const dsvi_Image_t* srcImage,dsvi_status_t* status);
dsvi_Image_t*   dsvi_image_suppressUsingHueBand(const dsvi_Image_t* srcImage,uint8_t hueMin,uint8_t hueMax,dsvi_status_t* _status);
#endif // __DISHANVESHI_INCLUDED_DSVI_IMAGE_OPERATIONS_HSV_H__
