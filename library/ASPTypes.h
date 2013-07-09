#ifndef __ASP_TYPES_H__
#define __ASP_TYPES_H__

#include "stdint.h"
#include "stddef.h"

typedef unsigned long int uint64;
typedef unsigned int uint;
typedef uint64_t u64;
typedef uint32_t u32;
typedef uint16_t u16;
typedef uint8_t  u8;
typedef struct { float x; float y; float z; float w; } vector4;
inline void pointer_make_relative(void** x) { *x = (void*)((intptr_t)*x - (intptr_t)x); }
inline void pointer_make_absolute(void** x) { *x = (void*)((intptr_t)*x + (intptr_t)x); }

#define null               0
#define countof(x)         sizeof(x)/sizeof(*x)
#define error(...)         assert(false)
#define bitcountof(x)      sizeof(x)*8
#define alignup(p, i)      ( ((uintptr_t)p + i - 1) & ~(i - 1) )
#define aligndown(p, i)    ( ((uintptr_t)p - i + 1) & ~(i - 1) )
#define rotate_left(x, r)  ( (x << r) | (x >> (bitcountof(x) - r)) )
#define clamp(x, min, max) ( x > max ? max : x < min ? min : x )
#define min(x,y)           ( x < y ? x : y )
#define max(x,y)           ( x > y ? x : y )
#define LinearInterpolate(x, x0, x1, y0, y1) ( y0+ ( (float)(y1-y0) * ( (float)(x-x0)/(float)(x1-x0) ) ) )

#endif
