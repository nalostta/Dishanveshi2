#ifndef __DISHANVESHI_INCLUDED_DSVI_IMAGE_H__
#define __DISHANVESHI_INCLUDED_DSVI_IMAGE_H__

#include "types.h"

#include <stdlib.h>
#include <stdint.h>
#include <misc.h>

///Module initialization
dsvi_status_t dsvi_image_Init(const DsviImageMemoryConfig_t* image_allocator);

///Module construction and destruction
dsvi_Image_t* dsvi_image_new(const ImageInfo_t* info, dsvi_status_t* status);
dsvi_status_t dsvi_image_delete(dsvi_Image_t* image);

///Module properties (attributes)
dsvi_status_t dsvi_image_getInfo(const dsvi_Image_t* image, ImageInfo_t* result);
dsvi_status_t dsvi_image_getData(const dsvi_Image_t* image, uint8_t** data);
dsvi_status_t dsvi_image_getMemConfig(void);

///Module functions (verbs)
dsvi_status_t dsvi_image_writeToFile(const dsvi_Image_t* image, const char* filename);
dsvi_status_t dsvi_image_readFromFile(dsvi_Image_t** image, const char* filename);
dsvi_status_t dsvi_image_clone(const dsvi_Image_t* image, dsvi_Image_t** result);

dsvi_status_t dsvi_image_setROI(dsvi_Image_t* image, const dsvi_ImageROI_t* roi);

//this should be transferred to a separate file in operations folder
dsvi_Image_t* dsvi_image_crop(const dsvi_Image_t* image,dsvi_status_t* status);
dsvi_Image_t* dsvi_image_cropEx(const dsvi_Image_t* image, const dsvi_ImageROI_t* roi, dsvi_status_t* status);  //extension method where user is explicit about the ROI
dsvi_Image_t* dsvi_image_colorToGrey(const dsvi_Image_t* srcImage,dsvi_status_t* status);                       //preserves format
dsvi_Image_t* dsvi_image_colorToGreyEx(const dsvi_Image_t* srcImage,real_t weightR,real_t weightG,real_t weightB,dsvi_status_t* status);
dsvi_Image_t* dsvi_image_getChannel(const dsvi_Image_t* srcImage,uint8_t ch,dsvi_status_t* status);
dsvi_Image_t* dsvi_image_replaceChannel(const dsvi_Image_t* srcImage,const dsvi_Image_t* srcChannel,uint8_t chIndex,dsvi_status_t* status);
dsvi_Image_t* dsvi_image_mergeUsingOR(const dsvi_Image_t* srcImage,const dsvi_Image_t* srcChannel,dsvi_status_t* status);
dsvi_Image_t* dsvi_image_mergeUsingAND(const dsvi_Image_t* srcImage,const dsvi_Image_t* srcChannel,dsvi_status_t* status);
dsvi_Image_t* dsvi_image_convertToDefault(const dsvi_Image_t* srcImage,dsvi_status_t* status);
dsvi_Image_t* dsvi_image_convertTo888(const dsvi_Image_t* srcImage,dsvi_status_t* status);
dsvi_Image_t* dsvi_image_combine3chTo888(const dsvi_Image_t* srcChR,const dsvi_Image_t* srcChG,const dsvi_Image_t* srcChB,dsvi_status_t* status);
#endif // __DISHANVESHI_INCLUDED_DSVI_IMAGE_H__
