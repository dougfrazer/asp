//*****************************************************************************
// ASPLib
// ------
//   A collection of simple library functions
//
// (c) April 2013
// @author Doug Frazer
//*****************************************************************************

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

#define null              0
#define countof(x)        sizeof(x)/sizeof(*x)
#define error(...)        assert(false)
#define bitcountof(x)     sizeof(x)*8
#define alignup(p, i)     ( ((uint64)p + i - 1) & ~(i - 1) )
#define aligndown(p, i)   ( ((uint64)p - i + 1) & ~(i - 1) )
#define rotate_left(x, r) ( (x << r) | (x >> (bitcountof(x) - r)) )

// TODO: Use restrict keyword? Or just assert?
// TODO: memmove
void Memcpy(void* dest, const void* src, size_t size);

void Memset(void* dest, char val, size_t size);
void Memset(void* dest, u8 val, size_t size); 

uint Log2(uint _val);

const u32 StringHash(const char* String); // TODO: run at build time for constant strings?
