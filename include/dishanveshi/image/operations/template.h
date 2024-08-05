#ifndef __DISHANVESHI_INCLUDED_DSVI_IMAGE_OPERATIONS_TEMPLATE_H_
#define __DISHANVESHI_INCLUDED_DSVI_IMAGE_OPERATIONS_TEMPLATE_H_

#include <dishanveshi/image/operations/types.h>
#include <dishanveshi/image/image.h>

/**
 *  getTemplate fn requires a template (type : dsvi_template_t)
 *  having the parameters : x,y which are the position of the top-left corner of the boundary of template.
 */


dsvi_Image_t* dsvi_image_getTemplate(const dsvi_Image_t* srcImage,const dsvi_template_t* srcTemplate,uint8_t bgVal,dsvi_status_t* status);

#endif // __DISHANVESHI_INCLUDED_DSVI_IMAGE_OPERATIONS_TEMPLATE_H_
