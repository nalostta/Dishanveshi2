#ifndef __DISHANVESHI_INCLUDED_DSVI_IMAGE_DEFS_H__
#define __DISHANVESHI_INCLUDED_DSVI_IMAGE_DEFS_H__

#include <dishanveshi/defs.h>

#ifdef DSVI_CONFIG_PUBLIC_IMAGE
#define DSVI_PUBLIC_IMAGE 1
#undef DSVI_CONFIG_PUBLIC_IMAGE
#else
#define DSVI_PUBLIC_IMAGE 0
#endif // DSVI_CONFIG_PUBLIC_IMAGE

#endif // __DISHANVESHI_INCLUDED_DSVI_IMAGE_DEFS_H__
