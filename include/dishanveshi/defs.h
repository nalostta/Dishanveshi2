#ifndef __DISHANVESHI_INCLUDED_DEFS_H__
#define __DISHANVESHI_INCLUDED_DEFS_H__

#include "config.h"

#ifndef DSVI_REAL_IS_FLOAT
#error "Configuration isn't valid, you must define the right configuration for floating points."
#endif // DSVI_REAL_IS_FLOAT

#define DSVI_STR_BUFFER_SIZE 40

#define DSVI_STATUS_OK            0
#define DSVI_STATUS_ERROR         -1
#define DSVI_STATUS_UNKNOWN_ERROR DSVI_STATUS_ERROR
#define DSVI_STATUS_INVALID_ARG   (DSVI_STATUS_UNKNOWN_ERROR - 1)                        //-2
#define DSVI_STATUS_NULL_ARG      (DSVI_STATUS_INVALID_ARG - 1)                          //-3

#define DSVI_STATUS_INVALID_OPERATION (DSVI_STATUS_NULL_ARG - 1)                         //-4
#define DSVI_STATUS_NOT_SUPPORTED     (DSVI_STATUS_INVALID_OPERATION - 1)                //-5
#define DSVI_STATUS_NOT_IMPLEMENTED   (DSVI_STATUS_NOT_SUPPORTED - 1)                    //-6

#define DSVI_STATUS_MEMORY_ERROR             (DSVI_STATUS_NOT_IMPLEMENTED - 1)           //-7
#define DSVI_STATUS_MEMORY_ALLOCATION_FAILED (DSVI_STATUS_MEMORY_ERROR - 1)              //-8
#define DSVI_STATUS_MEMORY_WRITE_FAILED       (DSVI_STATUS_MEMORY_ALLOCATION_FAILED - 1) //-9
#define DSVI_STATUS_MEMORY_READ_FAILED       (DSVI_STATUS_MEMORY_WRITE_FAILED - 1)       //-10




#endif // __DISHANVESHI_INCLUDED_DEFS_H__
