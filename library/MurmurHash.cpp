//*****************************************************************************
// Murmur Hash
// -----------
//   See http://code.google.com/p/smhasher/wiki/MurmurHash3
//
// (c) April 2013
// @author Doug Frazer
//*****************************************************************************

#include "MurmurHash.h"

//*****************************************************************************
// Function
//*****************************************************************************
u32 MurmurHash(const void* key, const uint len)
{
    const u32 seed    = 0xdeadbeef;
    const u8 * data   = (const u8*)key;
    const int nblocks = len / 4;

    u32 h1 = seed;

    const u32 c1 = 0xcc9e2d51;
    const u32 c2 = 0x1b873593;

    //----------
    // body

    const u32 * blocks = (const u32 *)(data + nblocks*4);

    for(int i = -nblocks; i < 0; i++)
    {
        u32 k1 = getblock(blocks,i);

        k1 *= c1;
        k1 = rotate_left(k1,15);
        k1 *= c2;

        h1 ^= k1;
        h1 = rotate_left(h1,13);
        h1 = h1*5+0xe6546b64;
    }

    //----------
    // tail

    const u8 * tail = (const u8*)(data + nblocks*4);

    u32 k1 = 0;

    switch(len & 7)
    {
        case 3: k1 ^= tail[2] << 16;
        case 2: k1 ^= tail[1] << 8;
        case 1: k1 ^= tail[0];
        k1 *= c1; k1 = rotate_left(k1,15); k1 *= c2; h1 ^= k1;
    };

    //----------
    // finalization

    h1 ^= len;
    h1 = fmix(h1);
    return h1;
}
//*****************************************************************************
