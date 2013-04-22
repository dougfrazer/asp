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

// DEBUG
#include "string.h"
// END DEBUG

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
