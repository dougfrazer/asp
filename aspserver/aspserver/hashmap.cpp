//*******************************************************************************
// HASH MAP
//   Provides an interface for which to overload the unordered_map hash function
//   so I can hash on a custom type.
//   
// @author Doug Frazer
// February 2012
//*******************************************************************************

#include "hashmap.h"

long HashFunction(const NetworkKey &key)
{
    // TODO: make more intelligent hash functions
    return key.Address.s_addr + key.Port;
}
long HashEquals(const NetworkKey &x, const NetworkKey &y)
{
    return x.Address.s_addr + x.Port == y.Address.s_addr + y.Port;
}
