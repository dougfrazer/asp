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
    MaxDepth       = 0;
    
    // Even this is far too generous of an assertion
    assert( BlockSize < AllocationSize / 2 );
    assert( _BlockSize > sizeof(BLOCK) );

    Segments       = GetNewSegment();

    DataSize = AllocationSize - alignup( sizeof(SEGMENT), BlockSize );
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
    while( s != null ) {
        if( s->Memory <= ptr && ( (u8*)(s->Memory) + DataSize ) > ptr ) {
            assert( ( (u8*)ptr - (u8*)(s->Memory)  ) % BlockSize == 0 ); 
            return s;
        }
        s = ptr < s->Memory ? s->Left : s->Right;
    }
    return null;
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
void MEMORY_POOL::InsertIntoTree(SEGMENT* Segment)
{
    assert(Segment->Memory != null);
    SEGMENT** s = &Segments;
    SEGMENT* p = null;
    int Depth = 0;
    while( *s != null ) {
        p = *s;
        s = Segment->Memory < (*s)->Memory ? &((*s)->Left) : &((*s)->Right);
        //s = Segment < (*s)->Left ? &((*s)->Left) : &((*s)->Right);
        Depth++;
    }
    *s = Segment;
    (*s)->Height = 1;
    (*s)->Parent = p;
    if(Depth > MaxDepth) MaxDepth = Depth;

    while( p != null ) {
        p->Height = max( Height(p->Left), Height(p->Right) ) + 1;
        int balance = GetBalanceFactor(p);
        if( balance > 1 ) {
            if( Segment < (p->Left) ) { // Left Left case
                RotateRight(p);
            } else { // Left Right case
                RotateLeft(p->Left);
                RotateRight(p);
            }
        } else if( balance < -1 ) {
            if( Segment < (p->Left) ) { // Right Left case
                RotateRight(p->Right);
                RotateLeft(p);
            } else { // Right Right case
                RotateLeft(p);
            }
        }
        p = p->Parent;
    }
}
//******************************************************************************
void MEMORY_POOL::RotateLeft(SEGMENT* Segment)
{
    assert(Segment->Right != null);
    SEGMENT*  Root       = Segment;
    SEGMENT*  Pivot      = Segment->Right;

    if( Root->Parent == null ) {
        Segments = Pivot;
    } else {
        if( Root->Parent->Left == Root ) {
            Root->Parent->Left = Pivot;
        } else if( Root->Parent->Right == Root ) {
            Root->Parent->Right = Pivot;
        } else {
            assert(false);
        }
    }

    Root->Right          = Pivot->Left;
    Pivot->Left          = Root;

    Pivot->Parent        = Root->Parent;
    Root->Parent         = Pivot;
    if( Root->Right != null ) {
        Root->Right->Parent  = Root;
    }

    Root->Height = max( Height(Root->Left), Height(Root->Right) ) + 1;
    Pivot->Height = max( Height(Pivot->Left), Height(Pivot->Right) ) + 1;
}
//******************************************************************************
void MEMORY_POOL::RotateRight(SEGMENT* Segment)
{
    assert(Segment->Left != null);
    SEGMENT*  Root       = Segment;
    SEGMENT*  Pivot      = Segment->Left;

    if( Root->Parent == null ) {
        Segments = Pivot;
    } else {
        if( Root->Parent->Left == Root ) {
            Root->Parent->Left = Pivot;
        } else if( Root->Parent->Right == Root ) {
            Root->Parent->Right = Pivot;
        } else {
            assert(false);
        }
    }

    Root->Left            = Pivot->Right;                  
    Pivot->Right          = Root;

    Pivot->Parent         = Root->Parent;
    Root->Parent          = Pivot;
    if( Root->Left != null ) {
        Root->Left->Parent    = Root;
    }

    Root->Height = max( Height(Root->Left), Height(Root->Right) ) + 1;
    Pivot->Height = max( Height(Pivot->Left), Height(Pivot->Right) ) + 1;
}
//******************************************************************************
int MEMORY_POOL::GetBalanceFactor(SEGMENT* s)
{
    if( s == null ) return 0;
    return Height(s->Left) - Height(s->Right);
}
//******************************************************************************
int MEMORY_POOL::Height(SEGMENT* x)
{
    return x == null ? 0 : x->Height;
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
