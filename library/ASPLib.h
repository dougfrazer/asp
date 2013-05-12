//*****************************************************************************
// ASPLib
// ------
//   A collection of simple library functions
//
// (c) April 2013
// @author Doug Frazer
//*****************************************************************************

#ifndef __ASPLIB_H__
#define __ASPLIB_H__

#include "stdint.h"
#include "assert.h"

// TODO: error handling module

typedef unsigned long int uint64;
typedef unsigned int uint;
typedef uint64_t u64;
typedef uint32_t u32;
typedef uint16_t u16;
typedef uint8_t  u8;
typedef long unsigned int size_t;
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

// TODO: Use restrict keyword? Or just assert?
// TODO: memmove
void Memcpy(void* dest, const void* src, size_t size);

void Memset(void* dest, char val, size_t size);
void Memset(void* dest, u8 val, size_t size);

uint Log2(uint _val);

const u32 StringHash(const char* String); // TODO: run at build time for constant strings?

#endif
