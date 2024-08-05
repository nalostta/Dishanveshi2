#ifndef __DISHANVESHI_INCLUDED_DSVI_IMAGE_OPERATIONS_QUEUE_H_
#define __DISHANVESHI_INCLUDED_DSVI_IMAGE_OPERATIONS_QUEUE_H_

#include <dishanveshi/image/image.h>

dsvi_status_t dsvi_imageOp_label(dsvi_Image_t* srcImage,uint8_t threshold);
dsvi_Image_t* dsvi_imageOp_debugLabels(const dsvi_Image_t* srcImage,dsvi_status_t* status);
dsvi_Image_t* dsvi_imageOp_printLabelMtx(const dsvi_Image_t* srcImage,dsvi_status_t* status);

//module init
dsvi_regionBoundary_t* dsvi_regionBoundary_fixed_new(dsvi_status_t* status);
dsvi_status_t          dsvi_regionBoundary_delete(dsvi_regionBoundary_t* srcBdry);

//module attributes
int dsvi_imageOp_checkNeigbhorForegnd_startEastClkwise(dsvi_Image_t* srcImage,uint16_t x,uint16_t y,uint8_t threshold,uint8_t prevNbr,dsvi_status_t* status);
dsvi_regionBoundary_t* dsvi_imageOp_getRegionBoundary(dsvi_Image_t* srcImage,uint16_t originX,uint16_t originY,uint8_t threshold,dsvi_status_t* status);
dsvi_status_t dsvi_imageOp_writeRegionBoundaryToFile(dsvi_regionBoundary_t* srcBdry,const char* path);
//

dsvi_status_t dsvi_image_labelRegion(dsvi_Image_t* srcImage,uint8_t threshold);
dsvi_Image_t* dsvi_image_labeledRegionAsImage(const dsvi_Image_t* srcImage,uint8_t channel,dsvi_status_t* status);

#endif // __DISHANVESHI_INCLUDED_DSVI_IMAGE_OPERATIONS_QUEUE_H_
