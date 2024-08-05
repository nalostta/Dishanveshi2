#ifndef __DISHANVESHI_INCLUDED_DSVI_IMAGE_OPERATIONS_FILTER_H_
#define __DISHANVESHI_INCLUDED_DSVI_IMAGE_OPERATIONS_FILTER_H_

#include <dishanveshi/image/image.h>

dsvi_Image_t* dsvi_image_threshold(const dsvi_Image_t* srcImage,uint8_t threshold,dsvi_status_t* status);
dsvi_Image_t* dsvi_image_bandThreshold(const dsvi_Image_t* srcImage,uint8_t lower,uint8_t upper,dsvi_status_t* status);

dsvi_Image_t* dsvi_image_medianFilter(const dsvi_Image_t* srcImage,uint8_t frameSize,dsvi_status_t* status);
dsvi_Image_t* dsvi_image_medianFilterAdaptive(const dsvi_Image_t* srcImage,uint8_t bandpassL,uint8_t bandpassH,dsvi_status_t* status);
uint8_t dsvi_image_getMedianVal(uint8_t* buffer,uint8_t windowLen,dsvi_status_t* status);

dsvi_Image_t* dsvi_image_gaussianFilter_ch1sz5(const dsvi_Image_t* srcImage,dsvi_status_t* status);



#endif // __DISHANVESHI_INCLUDED_DSVI_IMAGE_OPERATIONS_FILTER_H_
