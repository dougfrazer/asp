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

#include "ASPTypes.h"

// TODO: error handling module
#include "assert.h"

// TODO: Use restrict keyword? Or just assert?
// TODO: memmove
void Memcpy(void* dest, const void* src, size_t size);

void Memset(void* dest, char val, size_t size);
void Memset(void* dest, u8 val, size_t size);

uint Log2(uint _val);

const u32 StringHash(const char* String); // TODO: run at build time for constant strings?

void* Malloc( size_t size );
void  Free( void* ptr );
void  Free( size_t size, void* ptr );

#endif
