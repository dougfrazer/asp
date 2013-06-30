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
// LoadFactor:
//    Load factor is defined as Size/NumBuckets.  This argument specifies
//    when we will rehash, when our current LoadFactor exceeds this value.
//
// (c) March 2013
//
// @author Doug Frazer
//*****************************************************************************

#include "ASPLib.h"

class HASH_MAP
{
public:
	HASH_MAP(uint   _NumBuckets, 
             uint   _KeyLen, 
             float  _LoadFactor = DEFAULT_LOAD_FACTOR);
	~HASH_MAP();

public:
	bool  Insert    ( void* key, void* val);
	bool  Remove    ( void* key );
	void* GetValue  ( void* key );
	void* Find      ( void* key ) {return GetValue(key);}

private:
	struct BUCKET {
		u32     hash;
		void*   val;
		BUCKET* next;
	};

	BUCKET**  Buckets;
	uint      NumBuckets;
	uint      KeyLen;

	uint    Size;
	float   LoadFactor;

private:
	static const float DEFAULT_LOAD_FACTOR;
	BUCKET** FindInternal(uint Hash, BUCKET** Prev = null);
	bool InsertInternal(uint Hash, void* key);
	bool Resize(uint NewNumBuckets);
};
