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
#include "ASPLib.h"
#include "MurmurHash.h"


//*****************************************************************************
// Constructor/Destructor
//*****************************************************************************
HASH_MAP::HASH_MAP(uint      _NumBuckets, 
                   uint      _KeyLen, 
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
    if(Buckets == null) return;

    Memset(Buckets, (u8)0, NumBuckets * sizeof(BUCKET*));
}
//*****************************************************************************
void HASH_MAP::Deinit()
{
    BUCKET* Bucket;
    BUCKET* NextBucket;

    if(Buckets == null) return;
    for(uint i = 0; i < NumBuckets; i++) {
        Bucket = Buckets[i];
        while(Bucket != null) {
            NextBucket = Bucket->next;
            FreeFn(Bucket);
            Bucket = NextBucket;
        }
    }
    FreeFn(Buckets);
    Memset(Buckets, (u8)0, NumBuckets * sizeof(BUCKET*));
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
    if(Bucket == null) {
        error("Out of memory");
        return false;
    }
    if(*Bucket == null) {
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

    return Bucket == null ? null : *Bucket == null ? null : (*Bucket)->val;
}
//*****************************************************************************


//*****************************************************************************
// Private Interface
//*****************************************************************************
HASH_MAP::BUCKET** HASH_MAP::FindInternal(u32 Hash, BUCKET** Prev)
{
    uint Index;
    BUCKET**     Iter;

    if(Buckets == null) return null;

    Index = Hash % NumBuckets;
    Iter = &Buckets[Index];
    if(Prev != null) *Prev = null;

    while(*Iter != null) {
        if((*Iter)->hash == Hash) return Iter;
        if(Prev != null) *Prev = *Iter;
        Iter = &(*Iter)->next;
    }
    return Iter;
}
//*****************************************************************************
bool HASH_MAP::InsertInternal(u32 Hash, void* val)
{
    BUCKET*  Prev;
    BUCKET** Bucket;

    Bucket = FindInternal(Hash, &Prev);
    if(Bucket == null) {
        error("Out of memory");
        return false;
    }
    if(*Bucket != null) {
        error("Key already exists in map (or another key hashed to same value)");
        return false;
    }
    *Bucket = (BUCKET*)AllocFn(sizeof(BUCKET));
    if(*Bucket == null) {
        return false;
    }
    (*Bucket)->hash = Hash;
    (*Bucket)->val = val;
    (*Bucket)->next = null;
    if(Prev != null) Prev->next = *Bucket;
    Size++;

    if(Size/(float)NumBuckets >= LoadFactor) {
        if(!Resize(NumBuckets * 2)) {
            return false;
        }
    }
    return true;
}
//*****************************************************************************
bool HASH_MAP::Resize(uint NewNumBuckets)
{
    BUCKET** NewBuckets;
    BUCKET** OldBuckets;
    BUCKET*  Bucket;
    BUCKET*  NextBucket;
    uint     OldNumBuckets;

    NewBuckets = (BUCKET**)AllocFn(NewNumBuckets*sizeof(BUCKET*));
    if(NewBuckets == null) {
        error("Failed to alloc new space for resizing");
        return false;
    }
    Memset(NewBuckets, (u8)0, NewNumBuckets*sizeof(BUCKET*));

    OldBuckets = Buckets;
    OldNumBuckets = NumBuckets;

    Buckets = NewBuckets;
    NumBuckets = NewNumBuckets;
    Size = 0;
    for(uint i = 0; i < OldNumBuckets; i++) {
        Bucket = OldBuckets[i];
        while(Bucket != null) {
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
