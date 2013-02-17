//*******************************************************************************
// HASH MAP
//   Provides an interface for whichi to overload the unordered_map hash function
//   so I can hash on a custom type.
//   
// @author Doug Frazer
// February 2012
//*******************************************************************************

#include <netinet/in.h>

typedef struct {
    in_addr        Address;
    unsigned short Port;
} NetworkKey;

long HashFunction(const NetworkKey &key);
long HashEquals(const NetworkKey &x, const NetworkKey &y);

//*****************************************************************************
// Overload the hash function operator
//*****************************************************************************
typedef struct {
    long operator()(const NetworkKey &key) const { return HashFunction(key); }
} NetworkKeyHash;

//*****************************************************************************
// Overload the equals function operator
//*****************************************************************************
typedef struct {
    long operator()(const NetworkKey &x, const NetworkKey &y) const { return HashEquals(x, y); }
} NetworkKeyEq;
