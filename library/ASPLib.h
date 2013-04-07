//*****************************************************************************
// ASPLib
// ------
//   A collection of simple library functions so I don't have to include
//   random STL headers like "string.h" all over the place
//*****************************************************************************

#include "stdint.h"

typedef unsigned long int uint64;
typedef unsigned int uint;
typedef uint64_t u64;
typedef uint32_t u32;
typedef uint16_t u16;
typedef uint8_t  u8;
typedef long unsigned int size_t;

#define countof(x)      sizeof(x)/sizeof(*x)
#define error(...)      assert(false)
#define alignup(p, i)   ( ((uint64)p + i - 1) & ~(i - 1) )
#define aligndown(p, i) ( ((uint64)p - i + 1) & ~(i - 1) )

// TODO: Use restrict keyword? Or just assert?
void Memcpy(void* dest, const void* src, size_t size);

void Memset(void* dest, char val, size_t size);
void Memset(void* dest, unsigned int val, size_t size); // only valid for values less than 255

unsigned int Log2(unsigned int _val);
