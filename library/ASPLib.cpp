//*****************************************************************************
// ASPLib
// ------
//   A collection of simple library functions
//
// (c) April 2013
// @author Doug Frazer
//*****************************************************************************

#include "ASPLib.h"
#include "MurmurHash.h"
#include "MemoryPool.h"
#include "stdlib.h"

// DEBUG
#include "string.h"
// END DEBUG

static MEMORY_POOL Pool_8  ( 8,   4096 );
static MEMORY_POOL Pool_16 ( 16,  4096 );
static MEMORY_POOL Pool_32 ( 32,  4096 );
static MEMORY_POOL Pool_64 ( 64,  4096 );
static MEMORY_POOL Pool_128( 128, 4096 );
static MEMORY_POOL Pool_256( 256, 4096 );

//*****************************************************************************
void Memcpy(void* dest, const void* src, size_t size)
{
    char* Dest;
    char* Src;
    char* DestEnd;
    unsigned long int* Dest2;
    unsigned long int* Src2;
    unsigned long int* DestEnd2;

    // Copy over bytes until we are 8-byte aligned
    Dest = (char*)dest;
    Src = (char*)src;
    DestEnd = (char*)alignup(dest, 8);
    while(Dest < DestEnd) {
        *Dest++ = *Src++;
    }

    // Do large copies (8 bytes at a time)
    Dest2 = (u64*)Dest;
    Src2 = (u64*)Src;
    DestEnd2 = (u64*)aligndown(dest + size, 8);
    while(Dest2 < DestEnd2) {
        *Dest2++ = *Src2++;
    }

    // Finish it up (anything on the tail end thats not 8-byte aligned)
    Dest = (char*)Dest2;
    Src = (char*)Src2;
    DestEnd = (char*)dest + size;
    while(Dest < DestEnd) {
        *Dest++ = *Src++;
    }
}
//*****************************************************************************
void Memset(void* dest, char val, size_t size)
{
    char* Dest = (char*)dest;
    char* DestEnd = &Dest[size];
    while(Dest < DestEnd) {
        *Dest++ = val;
    }
}
//*****************************************************************************
void Memset(void* dest, u8 val, size_t size)
{
    char* Dest = (char*)dest;
    char* DestEnd = &Dest[size];
    while(Dest < DestEnd) {
        *Dest++ = val;
    }
}
//*****************************************************************************
uint Log2(unsigned int _val)
{
    // GCC built-in function (bsr - bitscanreverse)
    return bitcountof(_val) - __builtin_clz(_val);
}
//*****************************************************************************
void* Malloc( size_t size )
{
    if( size <= 0 ) {
        return null;
    } else if( size <= 8 ) {
        return Pool_8.GetBlock();
    } else if( size <= 16 ) {
        return Pool_16.GetBlock();
    } else if( size <= 32 ) {
        return Pool_32.GetBlock();
    } else if( size <= 64 ) {
        return Pool_64.GetBlock();
    } else if( size <= 128 ) {
        return Pool_128.GetBlock();
    } else if( size <= 256 ) {
        return Pool_256.GetBlock();
    } else {
        return malloc( size );
    }
}
//*****************************************************************************
void Free( void* ptr )
{
    assert(false); // currently unsupported
}
//*****************************************************************************
void Free( size_t size, void* ptr )
{
    if( size <= 0 ) {
        return;
    } else if( size <= 8 ) {
        Pool_8.FreeBlock( ptr );
    } else if( size <= 16 ) {
        Pool_16.FreeBlock( ptr );
    } else if( size <= 32 ) {
        Pool_32.FreeBlock( ptr );
    } else if( size <= 64 ) {
        Pool_64.FreeBlock( ptr );
    } else if( size <= 128 ) {
        Pool_128.FreeBlock( ptr );
    } else if( size <= 256 ) {
        Pool_256.FreeBlock( ptr );
    } else {
        free( ptr );
    }
}
//*****************************************************************************

// DEBUG FEATURE - see what strings are hashing to
struct String {
    char Name[100];
    u32  Hash;
};
static String StringTable[100];
static u32 StringIndex = 0;
// END DEBUG FEATURE

const u32 StringHash(const char* Str)
{
    uint len = 0;
    while(Str[len] != '\0') {
        len++;
    }
    u32 ret = MurmurHash(Str, len);

    // DEBUG - record recent strings in a circular buffer
    strncpy(&StringTable[StringIndex].Name[0], Str, sizeof(StringTable[StringIndex].Name)); 
    StringTable[StringIndex].Hash = ret;
    StringIndex = StringIndex + 1 == countof(StringTable) ? 0 : StringIndex + 1;
    // END DEBUG
   
    return ret;
}
//*****************************************************************************
