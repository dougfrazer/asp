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
    while(dest < DestEnd) {
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
const u32 StringHash(const char* String)
{
    uint len = 0;
    while(String[len] != '\0') {
        len++;
    }
    return MurmurHash(String, len);
}
//*****************************************************************************
