//*****************************************************************************
// Murmur Hash
// -----------
//   See http://code.google.com/p/smhasher/wiki/MurmurHash3
//
// (c) April 2013
// @author Doug Frazer
//*****************************************************************************

#include "ASPLib.h"

inline u32 getblock ( const u32 * p, int i ) { return p[i]; }
inline u32 fmix ( u32 h )
{
    h ^= h >> 16;
    h *= 0x85ebca6b;
    h ^= h >> 13;
    h *= 0xc2b2ae35;
    h ^= h >> 16;

    return h;
}

u32 MurmurHash(const void* key, const uint len);
