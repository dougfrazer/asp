//*****************************************************************************
// Hash Map
// --------
//   This is a simple hash map structure.
//
//   If multiple items hash to the same element, they will be appended to a
//   linked list in that bucket.
//
// NumBuckets:
//    The original number of buckets to instantiate the list with
// KeyLen:
//    The length of the data referenced by keys when inserted.  Note
//    the void* passed in for Insert() will be dereferenced and KeyLen
//    bytes will be read and hashed.
// Buffer:
//    Pre-allocated space for which to use for this hash map.
// Size:
//    Size of the buffer.
// LoadFactor:
//    Load factor is defined as Size/NumBuckets.  This argument specifies
//    when we will rehash, when our current LoadFactor exceeds this value.
// Hash:
//    If you have some need for a custom hash function, it can be passed
//    in here.  Note that the default hash function provided is a widely
//    used, quick, and uniform hash function and should serve most needs.
//
// (c) March 2013
//
// @author Doug Frazer
//*****************************************************************************

#include "stdint.h"
#include "stddef.h"

typedef void* (HashMap_AllocFn)(size_t Size);
typedef void  (HashMap_FreeFn)(void* ptr);

class HASH_MAP
{
public:
	HASH_MAP(unsigned int     _NumBuckets, 
             unsigned int     _KeyLen, 
             HashMap_AllocFn* _AllocFn, 
             HashMap_FreeFn*  _FreeFn, 
             float            _LoadFactor = DEFAULT_LOAD_FACTOR);
	~HASH_MAP();

public:
	void  Init();
	void  Deinit();
	bool  Insert(void* key, void* val);
	bool  Remove(void* key);
	void* GetValue(void* key);
	void* Find(void* key) {return GetValue(key);}

private:
	struct BUCKET {
		unsigned int hash;
		void*        val;
		BUCKET*      next;
	};

	BUCKET**       Buckets;
	unsigned int   NumBuckets;
	unsigned int   KeyLen;

	unsigned int   Size;
	float          LoadFactor;

    HashMap_AllocFn* AllocFn;
    HashMap_FreeFn*  FreeFn;

private: // default hash function
	static unsigned int MurmurHash(const void * Key, const unsigned int KeyLen);

private:
	static const float DEFAULT_LOAD_FACTOR = 1.0;
	BUCKET** FindInternal(unsigned int Hash, BUCKET** Prev = NULL);
	bool InsertInternal(unsigned int Hash, void* key);
	bool Resize(unsigned int NewNumBuckets);
};
