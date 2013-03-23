//*****************************************************************************
// ASPLib
// ------
//   A collection of simple library functions so I don't have to include
//   random STL headers like "string.h" all over the place
//*****************************************************************************

#include "stdint.h"

typedef uint32_t u32;
typedef uint16_t u16;
typedef uint8_t  u8;
typedef long unsigned int size_t;

#define countof(x) sizeof(x)/sizeof(*x)
#define error(...) assert(false)

void Memcpy(void* dest, void* src, size_t size);

void Memset(void* dest, char val, size_t size);
void Memset(void* dest, unsigned int val, size_t size); // only valid for values less than 255
