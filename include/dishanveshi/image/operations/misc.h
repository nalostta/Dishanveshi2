//USES STDIO FUNCTIONALITY, FOR ABSTRACTION USE OPAQUE POINTER TO A DEBUG_PRINT FN; INPUT SHALL BE A CHARACTER BUFFER AND IT"S CHAR SIZE.

#ifndef __DISHANVESHI_INCLUDED_DSVI_IMAGE_OPERATIONS_MISC_H_
#define __DISHANVESHI_INCLUDED_DSVI_IMAGE_OPERATIONS_MISC_H_

#include <dishanveshi/image/operations/types.h>
#include <stdio.h>

dsvi_status_t dsvi_printMatrix(dsvi_Matrix_t* matrix);
dsvi_status_t dsvi_printKernel(dsvi_Kernel_t* kernel);
#endif // __DISHANVESHI_INCLUDED_DSVI_IMAGE_OPERATIONS_MISC_H_
