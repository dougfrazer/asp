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
#include "stdlib.h"

#include "MemoryPool.h"
#include "AvlTree.h"

// DEBUG
#include "string.h"
// END DEBUG

struct ALLOCATION : NODE {
    void*  ptr;
    size_t size;
};
static MEMORY_POOL Pool_8  ( 8,   4096 );
static MEMORY_POOL Pool_16 ( 16,  4096 );
static MEMORY_POOL Pool_32 ( 32,  4096 );
static MEMORY_POOL Pool_64 ( 64,  4096 );
static MEMORY_POOL Pool_128( 128, 4096 );
static MEMORY_POOL Pool_256( 256, 4096 );
static AVL_TREE<NODE> Allocations;

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
    }
    
    assert( sizeof(ALLOCATION) == 56 );
    ALLOCATION* NewAllocation = (ALLOCATION*)Pool_64.GetBlock();

    if( size <= 8 ) {
        NewAllocation->ptr = Pool_8.GetBlock();
    } else if( size <= 16 ) {
        NewAllocation->ptr = Pool_16.GetBlock();
    } else if( size <= 32 ) {
        NewAllocation->ptr = Pool_32.GetBlock();
    } else if( size <= 64 ) {
        NewAllocation->ptr = Pool_64.GetBlock();
    } else if( size <= 128 ) {
        NewAllocation->ptr = Pool_128.GetBlock();
    } else if( size <= 256 ) {
        NewAllocation->ptr = Pool_256.GetBlock();
    } else {
        NewAllocation->ptr = malloc( size );
    }

    NewAllocation->Key = (u64)( NewAllocation->ptr );
    Allocations.Insert( NewAllocation );
    return NewAllocation->ptr;
}
//*****************************************************************************
static void FreeInternal( size_t size, void* ptr )
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
void Free( void* ptr )
{
    ALLOCATION* Alloc = (ALLOCATION*)Allocations.Find( (u64)ptr );
    assert( Alloc != null );
    assert( Alloc->size > 0 );

    FreeInternal( Alloc->size, ptr );

    Allocations.Remove( (u64)ptr );
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
