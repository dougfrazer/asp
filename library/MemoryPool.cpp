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
    
    BlockSize = alignup( _BlockSize, sizeof(BLOCK) );
    DataSize  = AllocationSize - alignup( sizeof(SEGMENT), BlockSize );

    Segments  = GetNewSegment();
}
//******************************************************************************
MEMORY_POOL::~MEMORY_POOL()
{
    // free the memory
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
    // TODO: check for cyclic lists more completely?
    if(ptr == Segment->FreeBlock) {
        return;
    }
    BLOCK* PrevBlock = Segment->FreeBlock;
    Segment->FreeBlock = (BLOCK*)ptr;
    Segment->FreeBlock->Next = PrevBlock;

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
    NewSegment->Key = (u64)(NewSegment->Memory);
    SegmentTree.Insert(NewSegment);

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
    SEGMENT* Segment = (SEGMENT*)aligndown( ptr, AllocationSize );
    void* Memory = (void*)alignup( (u8*)(Segment) + sizeof(SEGMENT), BlockSize );
    return static_cast<SEGMENT*>(SegmentTree.Find( (u64)(Memory) ));
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






//******************************************************************************
// Test code
//******************************************************************************
void MEMORY_POOL::TestInternal()
{
#if 0
    assert(Segments == null);

    const uint NUM_TEST_SEGS = 100;
    SEGMENT* TestSegs[NUM_TEST_SEGS];
    for(int i = 1; i < NUM_TEST_SEGS; i++) {
        TestSegs[i] = new SEGMENT();
        TestSegs[i]->Memory = (void*)i;
    }
    
    // Test Left-Left rotation
    InsertIntoTree(TestSegs[30]);
    InsertIntoTree(TestSegs[40]);
    InsertIntoTree(TestSegs[50]);

    // Test Right-Right rotation
    InsertIntoTree(TestSegs[20]);
    InsertIntoTree(TestSegs[10]);

    // Test Right-Left rotation
    InsertIntoTree(TestSegs[25]);

    // Test Left-Right rotation
    InsertIntoTree(TestSegs[45]);
    InsertIntoTree(TestSegs[44]);
    InsertIntoTree(TestSegs[42]);
    InsertIntoTree(TestSegs[41]);

    assert((int)Segments->Memory == 30);
    assert((int)Segments->Left->Memory == 20);
    assert((int)Segments->Left->Left->Memory == 10);
    assert((int)Segments->Left->Right->Memory == 25);
    assert((int)Segments->Right->Memory == 42);
    assert((int)Segments->Right->Left->Memory == 40);
    assert((int)Segments->Right->Left->Right->Memory == 41);
    assert((int)Segments->Right->Right->Memory == 45);
    assert((int)Segments->Right->Right->Left->Memory == 44);
    assert((int)Segments->Right->Right->Right->Memory == 50);
#endif
}
//******************************************************************************
