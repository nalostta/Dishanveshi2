#ifndef __DISHANVESHI_INCLUDED_DSVI_IMAGE_OPERATIONS_HISTOGRAM_H_
#define __DISHANVESHI_INCLUDED_DSVI_IMAGE_OPERATIONS_HISTOGRAM_H_

#include <dishanveshi/image/operations/types.h>
#include <dishanveshi/image/image.h>

//module construction and destruction
dsvi_histogram_t*   dsvi_image_histogram_new(const dsvi_histogramInfo_t* histogramInfo,dsvi_status_t* status);
dsvi_status_t       dsvi_image_histogram_delete(dsvi_histogram_t* histogram);

//module attributes

//module functions (verbs)
dsvi_status_t       dsvi_image_histogram_checkBoundsIntegrity(const dsvi_histogramInfo_t* srcInfo);
dsvi_histogram_t*   dsvi_image_getHistogram(const dsvi_Image_t* srcImage,const dsvi_histogramInfo_t* hInfo,dsvi_status_t* status);
dsvi_Image_t*       dsvi_image_histogramToImage(const dsvi_histogram_t* histogram,dsvi_status_t* status);
dsvi_Image_t*   dsvi_image_histogramToImageFixedHeight(const dsvi_histogram_t* histogram,dsvi_status_t* status);
#endif // __DISHANVESHI_INCLUDED_DSVI_IMAGE_OPERATIONS_HISTOGRAM_H_
