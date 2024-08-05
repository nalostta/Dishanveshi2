#ifndef __DISHANVESHI_INCLUDED_DSVI_IMAGE_OPERATIONS_CONVOLVE_H_
#define __DISHANVESHI_INCLUDED_DSVI_IMAGE_OPERATIONS_CONVOLVE_H_

#include "types.h"

dsvi_Matrix_t* dsvi_image_getMtxFromImage(const dsvi_Image_t* srcImage,dsvi_status_t* status);
dsvi_Image_t*  dsvi_image_getImageFromMtx(const dsvi_Matrix_t* srcMatrix,dsvi_status_t* status);
dsvi_Image_t*  dsvi_image_getImageFromRealMatrix(const dsvi_RealMatrix_t* srcMatrix,dsvi_status_t* status);

dsvi_Matrix_t* dsvi_image_convolve(const dsvi_Matrix_t* srcMatrix, const dsvi_Kernel_t* kernel, dsvi_status_t* status);
dsvi_status_t* dsvi_image_mtxConvolve(const dsvi_Matrix_t srcMatrix,const dsvi_Matrix_t kernelMtx,  dsvi_Matrix_t* result);

dsvi_RealMatrix_t* dsvi_image_RKConvolve(const dsvi_Matrix_t* srcMatrix, const dsvi_RealKernel_t* kernel, dsvi_status_t* status);
dsvi_RealMatrix_t* dsvi_image_RKConvolve_preserveSize(const dsvi_Matrix_t* srcMatrix, const dsvi_RealKernel_t* kernel, dsvi_status_t* status);

dsvi_status_t dsvi_image_deleteMatrix(dsvi_Matrix_t* srcMatrix);
dsvi_status_t dsvi_image_deleteRealMatrix(dsvi_RealMatrix_t* srcMatrix);
#endif // __DISHANVESHI_INCLUDED_DSVI_IMAGE_OPERATIONS_CONVOLVE_H_
