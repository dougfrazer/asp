#include "ASPLib.h"

void Memcpy(void* dest, void* src, size_t size)
{
    char* Dest = (char*)dest;
    char* Src = (char*)src;
    char* DestEnd = &Dest[size];
    while(Dest < DestEnd) {
        *Dest++ = *Src++;
    }
}

void Memset(void* dest, char val, size_t size)
{
    char* Dest = (char*)dest;
    char* DestEnd = &Dest[size];
    while(Dest < DestEnd) {
        *Dest++ = val;
    }
}

void Memset(void* dest, unsigned int val, size_t size)
{
    char* Dest = (char*)dest;
    char* DestEnd = &Dest[size];
    while(Dest < DestEnd) {
        *Dest++ = val & 0xFF;
    }
}
