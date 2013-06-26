//******************************************************************************
// Memory Pool
//
// (c) June 2013
// @author Doug Frazer
//******************************************************************************

#include "MemoryPool.h"

#include "stdlib.h"

//******************************************************************************
MEMORY_POOL::MEMORY_POOL(u32 _BlockSize, u32 _AllocationSize)
{
    BlockSize      = max(sizeof(BLOCK), _BlockSize);
    AllocationSize = _AllocationSize;
    Segments       = null;
    FreeSegments   = null;
    NumBlocks      = 0;
    NumSegments    = 0;
    
    // Even this is far too generous of an assertion
    assert( BlockSize < AllocationSize / 2 );
    assert( _BlockSize > sizeof(BLOCK) );
    
    Segments       = GetNewSegment();
}
//******************************************************************************
MEMORY_POOL::~MEMORY_POOL()
{
    // pre-order traverse the BST and free
}
//******************************************************************************

//******************************************************************************
// Public Interface
//******************************************************************************
void* MEMORY_POOL::GetBlock()
{
    SEGMENT* Segment = FreeSegments;
    if(Segment->FreeBlock == null) {
        FreeSegments = FreeSegments->NextFreeSegment;
        Segment = FreeSegments;
        if(Segment == null) {
            Segment = GetNewSegment();
        }
    }
    BLOCK* ret = Segment->FreeBlock;
    Segment->FreeBlock = Segment->FreeBlock->Next;
    NumBlocks++;
    return ret;
}
//******************************************************************************
void MEMORY_POOL::FreeBlock(void* ptr)
{
    SEGMENT* Segment = FindSegment( ptr );
    BLOCK* PrevBlock = Segment->FreeBlock;
    Segment->FreeBlock = (BLOCK*)ptr;
    ((BLOCK*)(Segment->FreeBlock))->Next = PrevBlock;

    AddToFreeList(Segment);

    NumBlocks--;
}
//******************************************************************************

//******************************************************************************
// Private Interface
//******************************************************************************
MEMORY_POOL::SEGMENT* MEMORY_POOL::GetNewSegment()
{
    SEGMENT* NewSegment = (SEGMENT*)malloc( AllocationSize );
    NewSegment->Memory = (void*)alignup( (u8*)(NewSegment) + sizeof(SEGMENT), BlockSize );
    NewSegment->FreeBlock = (BLOCK*)NewSegment->Memory;
    NewSegment->NextFreeSegment = null;
    NewSegment->Left = null;
    NewSegment->Right = null;
    InsertIntoTree(NewSegment);

    BLOCK* Block = null;
    BLOCK* NextBlock = null;
    int NumBlocks = ( AllocationSize - alignup( sizeof(SEGMENT), BlockSize ) ) / BlockSize;
    for( int i = 0; i < NumBlocks - 1; i++ ) {
        Block     = (BLOCK*)( (u8*)(NewSegment->Memory) + BlockSize*( i ) );
        NextBlock = (BLOCK*)( (u8*)(NewSegment->Memory) + BlockSize*( i + 1 ) );
        Block->Next = NextBlock;
    }
    Block = (BLOCK*)( (u8*)(NewSegment->Memory) + BlockSize*( NumBlocks - 1 ) );
    Block->Next = null;
    
    AddToFreeList(NewSegment);

    NumSegments++;

    return NewSegment;
}
//******************************************************************************
MEMORY_POOL::SEGMENT* MEMORY_POOL::FindSegment(void* ptr)
{
    SEGMENT* s = Segments;
    u32 ActualAllocationSize = AllocationSize - alignup( sizeof(SEGMENT), BlockSize );
    while( s != null ) {
        if( s->Memory <= ptr && (u8*)(s->Memory) + ActualAllocationSize > ptr ) {
            assert( ( (u8*)(s->Memory) - (u8*)ptr ) % BlockSize == 0 ); 
            return s;
        }
        s = s->Memory > ptr ? s->Left : s->Right;
    }
    return null;
}
//******************************************************************************
void MEMORY_POOL::InsertIntoTree(SEGMENT* Segment)
{
    assert(Segment->Memory != null);
    SEGMENT** s = &Segments;
    while( *s != null ) {
        // TODO: this binary search will be heavily weighted to one side because
        //       allocation will all be in sequential address order
        s = Segment->Memory < (*s)->Left ? &((*s)->Left) : &((*s)->Right);
    }
    *s = Segment;
}
//******************************************************************************
void MEMORY_POOL::AddToFreeList(SEGMENT* Segment)
{
    SEGMENT** Prev = &FreeSegments;
    while( *Prev != null ) {
        if( *Prev == Segment ) return;
        *Prev = (*Prev)->NextFreeSegment;
    }
    (*Prev) = Segment;
}
//******************************************************************************
