//*****************************************************************************
// Hash Map
// --------
//   See HashMap.h
//
// (c) March 2013
//
// @author Doug Frazer
//*****************************************************************************

#include "HashMap.h"

#include "assert.h"
#include "string.h"

#define error(...) assert(false)

//*****************************************************************************
// Constructor/Destructor
//*****************************************************************************
HASH_MAP::HASH_MAP(unsigned int      _NumBuckets, 
                   unsigned int      _KeyLen, 
                   HashMap_AllocFn*  _AllocFn, 
                   HashMap_FreeFn*   _FreeFn, 
                   float             _LoadFactor)
{
    AllocFn      = _AllocFn;
    FreeFn       = _FreeFn;
	NumBuckets   = _NumBuckets;
	KeyLen       = _KeyLen;
	LoadFactor   = _LoadFactor;
}
//*****************************************************************************
HASH_MAP::~HASH_MAP()
{	
}
//*****************************************************************************


//*****************************************************************************
// Public Interface
//*****************************************************************************
void HASH_MAP::Init()
{
	Buckets = (BUCKET**)AllocFn(NumBuckets*sizeof(BUCKET*));
	if(Buckets == NULL) return;

	memset(Buckets, 0, NumBuckets * sizeof(BUCKET*));
}
//*****************************************************************************
void HASH_MAP::Deinit()
{
	BUCKET* Bucket;
	BUCKET* NextBucket;

	if(Buckets == NULL) return;
	for(unsigned int i = 0; i < NumBuckets; i++) {
		Bucket = Buckets[i];
		while(Bucket != NULL) {
			NextBucket = Bucket->next;
			FreeFn(Bucket);
			Bucket = NextBucket;
		}
	}
	FreeFn(Buckets);
	memset(Buckets, 0, NumBuckets * sizeof(BUCKET*));
}
//*****************************************************************************
bool HASH_MAP::Insert(void* key, void* val)
{
	return InsertInternal(MurmurHash(key, KeyLen), val);
}
//*****************************************************************************
bool HASH_MAP::Remove(void* key)
{
	BUCKET** Bucket;
	BUCKET*  Removed;

	Bucket = FindInternal(MurmurHash(key, KeyLen));
	if(Bucket == NULL) {
		error("Out of memory");
		return false;
	}
	if(*Bucket == NULL) {
		error("Tried to remove a value that doesn't exist");
		return false;
	}

	Removed = *Bucket;
	*Bucket = Removed->next;
	FreeFn(Removed);
	Size--;
	return true;
}
//*****************************************************************************
void* HASH_MAP::GetValue(void* key)
{
	BUCKET** Bucket;

	Bucket = FindInternal(MurmurHash(key, KeyLen));
	
	return Bucket == NULL ? NULL : *Bucket == NULL ? NULL : (*Bucket)->val;
}
//*****************************************************************************


//*****************************************************************************
// Private Interface
//*****************************************************************************
HASH_MAP::BUCKET** HASH_MAP::FindInternal(uint32_t Hash, BUCKET** Prev)
{
	unsigned int Index;
	BUCKET**     Iter;

	if(Buckets == NULL) return NULL;

	Index = Hash % NumBuckets;
	Iter = &Buckets[Index];
	if(Prev != NULL) *Prev = NULL;

	while(*Iter != NULL) {
		if((*Iter)->hash == Hash) return Iter;
		if(Prev != NULL) *Prev = *Iter;
		Iter = &(*Iter)->next;
	}
	return Iter;
}
//*****************************************************************************
bool HASH_MAP::InsertInternal(uint32_t Hash, void* val)
{
	BUCKET*  Prev;
	BUCKET** Bucket;

	Bucket = FindInternal(Hash, &Prev);
	if(Bucket == NULL) {
		error("Out of memory");
		return false;
	}
	if(*Bucket != NULL) {
		error("Key already exists in map (or another key hashed to same value)");
		return false;
	}
	*Bucket = (BUCKET*)AllocFn(sizeof(BUCKET));
	if(*Bucket == NULL) {
		return false;
	}
	(*Bucket)->hash = Hash;
	(*Bucket)->val = val;
	(*Bucket)->next = NULL;
	if(Prev != NULL) Prev->next = *Bucket;
	Size++;

	if(Size/(float)NumBuckets >= LoadFactor) {
		if(!Resize(NumBuckets * 2)) {
			return false;
		}
	}
	return true;
}
//*****************************************************************************
bool HASH_MAP::Resize(unsigned int NewNumBuckets)
{
	BUCKET** NewBuckets;
    BUCKET** OldBuckets;
	BUCKET*  Bucket;
	BUCKET*  NextBucket;
	unsigned int     OldNumBuckets;

    NewBuckets = (BUCKET**)AllocFn(NewNumBuckets*sizeof(BUCKET*));
    if(NewBuckets == NULL) {
        error("Failed to alloc new space for resizing");
        return false;
    }
    memset(NewBuckets, 0, NewNumBuckets*sizeof(BUCKET*));

    OldBuckets = Buckets;
    OldNumBuckets = NumBuckets;

    Buckets = NewBuckets;
    NumBuckets = NewNumBuckets;
    Size = 0;
	for(unsigned int i = 0; i < OldNumBuckets; i++) {
		Bucket = OldBuckets[i];
		while(Bucket != NULL) {
			NextBucket = Bucket->next;
			if(!InsertInternal(Bucket->hash, Bucket->val)) {
                error("Failed to insert bucket");
                return false;
            }
			FreeFn(Bucket);
			Bucket = NextBucket;
		}
	}
	FreeFn(OldBuckets);
    return true;
}
//*****************************************************************************

//*****************************************************************************
// Default Hash Function: Murmur Hash
//
// See http://code.google.com/p/smhasher/wiki/MurmurHash3
//*****************************************************************************

//*****************************************************************************
// Definitions
//*****************************************************************************
inline uint32_t rotl32 ( uint32_t x, int r ) { return (x << r) | (x >> (32 - r)); }
inline uint32_t getblock ( const uint32_t * p, int i ) { return p[i]; }
inline uint32_t fmix ( uint32_t h )
{
	h ^= h >> 16;
	h *= 0x85ebca6b;
	h ^= h >> 13;
	h *= 0xc2b2ae35;
	h ^= h >> 16;

	return h;
}
//*****************************************************************************
// Function
//*****************************************************************************
uint32_t HASH_MAP::MurmurHash(const void* key, const unsigned int len)
{
	const uint32_t seed    = 0xdeadbeef;
	const uint8_t * data   = (const uint8_t*)key;
	const int nblocks = len / 4;

	uint32_t h1 = seed;

	const uint32_t c1 = 0xcc9e2d51;
	const uint32_t c2 = 0x1b873593;

	//----------
	// body

	const uint32_t * blocks = (const uint32_t *)(data + nblocks*4);

	for(int i = -nblocks; i < 0; i++)
	{
		uint32_t k1 = getblock(blocks,i);

		k1 *= c1;
		k1 = rotl32(k1,15);
		k1 *= c2;

		h1 ^= k1;
		h1 = rotl32(h1,13);
		h1 = h1*5+0xe6546b64;
	}

	//----------
	// tail

	const uint8_t * tail = (const uint8_t*)(data + nblocks*4);

	uint32_t k1 = 0;

	switch(len & 7)
	{
	case 3: k1 ^= tail[2] << 16;
	case 2: k1 ^= tail[1] << 8;
	case 1: k1 ^= tail[0];
		k1 *= c1; k1 = rotl32(k1,15); k1 *= c2; h1 ^= k1;
	};

	//----------
	// finalization

	h1 ^= len;
	h1 = fmix(h1);
	return h1;
}
//*****************************************************************************
