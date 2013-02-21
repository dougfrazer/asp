//*******************************************************************************
// HASH MAP
//   Provides an interface for which to overload the unordered_map hash function
//   so I can hash on a custom type.
//   
// @author Doug Frazer
// February 2012
//*******************************************************************************

#include "hashmap.h"

//*******************************************************************************
// Murmur Hash
//   Slightly modified to work with the custom data type
//
// By Austin Appleby (aappleby (AT) gmail)
// https://sites.google.com/site/murmurhash/
//*******************************************************************************
long HashFunction(const NetworkKey &key)
{
    int len = 6;
    unsigned int seed = (0xdeadbeef * 16);
    uint64_t m = 0xc6a4a7935bd1e995;
    const int r = 47;

    uint64_t h = seed ^ (len * m);
    uint64_t value = key.Port;
    value <<= 32;
    value += key.Address.s_addr;

    const uint64_t * data = &value;
    
    const uint64_t * end = data + (len/8);

    while(data != end)
    {
        uint64_t k = *data++;

        k *= m; 
        k ^= k >> r; 
        k *= m; 

        h ^= k;
        h *= m; 
    }

    const unsigned char * data2 = (const unsigned char*)data;

    switch(len & 7)
    {
    case 7: h ^= uint64_t(data2[6]) << 48;
    case 6: h ^= uint64_t(data2[5]) << 40;
    case 5: h ^= uint64_t(data2[4]) << 32;
    case 4: h ^= uint64_t(data2[3]) << 24;
    case 3: h ^= uint64_t(data2[2]) << 16;
    case 2: h ^= uint64_t(data2[1]) << 8;
    case 1: h ^= uint64_t(data2[0]);
            h *= m;
    };

    h ^= h >> r;
    h *= m;
    h ^= h >> r;

    return h;
}

//*******************************************************************************
// Equals function
//*******************************************************************************
long HashEquals(const NetworkKey &x, const NetworkKey &y)
{
    return x.Address.s_addr == y.Address.s_addr && x.Port == y.Port;
}
