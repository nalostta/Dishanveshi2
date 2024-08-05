#ifndef __DISHANVESHI_INCLUDED_CONFIG_H__
#define __DISHANVESHI_INCLUDED_CONFIG_H__

#ifdef  DSVI_CONFIG_DEBUG
#define DSVI_DEBUG  1
#undef  DSVI_CONFIG_DEBUG
#endif // DSVI_CONFIG_DEBUG

#ifdef  DSVI_CONFIG_REAL_IS_FLOAT
#define DSVI_REAL_IS_FLOAT  1
#undef  DSVI_CONFIG_REAL_IS_FLOAT
#else
#define DSVI_REAL_IS_FLOAT  0
#endif // DSVI_CONFIG_DEBUG

#if defined(USE_NANO_EPSILON)
static const float dsvi_epsilonF = 1e-9f;
#elif defined(USE_MICRO_EPSILON)
static const float dsvi_epsilonF = 1e-6f;
#elif defined(USE_MILI_EPSILON)
static const float dsvi_epsilonF = 1e-3f;
#else
static const float dsvi_epsilonF = 1e-6f;
#endif

#endif // __DISHANVESHI_INCLUDED_CONFIG_H__
