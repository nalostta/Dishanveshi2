#ifndef __DISHANVESHI_INCLUDED_DSVI_IMAGE_OPERATIONS_QUEUE_H_
#define __DISHANVESHI_INCLUDED_DSVI_IMAGE_OPERATIONS_QUEUE_H_

#include <dishanveshi/image/operations/types.h>
#include <dishanveshi/image/image.h>

dsvi_status_t   dsvi_imageOp_qPush(dsvi_Queue_t** headRef,uint32_t data);
dsvi_status_t   dsvi_imageOp_qSortedInsert(dsvi_Queue_t** headRef,uint32_t data);
uint32_t        dsvi_imageOp_qPop(dsvi_Queue_t** headRef,dsvi_status_t* status);

#endif // __DISHANVESHI_INCLUDED_DSVI_IMAGE_OPERATIONS_TEMPLATE_H_
