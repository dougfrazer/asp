//******************************************************************************
// Memory Pool
//
// (c) June 2013
// @author Doug Frazer
//******************************************************************************

#include "MemoryPool.h"

#include "stdlib.h"
#include "stdio.h"

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
    NewSegment->Parent = null;
    NewSegment->BalanceFactor = 0;
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
    SEGMENT* Parent = null;
    SEGMENT** s = &Segments;
    int Depth = 0;
    while( *s != null ) {
        Parent = *s;
        Depth++;
        s = Segment->Memory < (*s)->Left ? &((*s)->Left) : &((*s)->Right);
    }
    *s = Segment;
    Segment->Parent = Parent;
    if(Depth > MaxDepth) MaxDepth = Depth;

    // rotate the tree to keep it balanced
    SEGMENT* p = Parent;
    if(Parent != null) {
        if(*s == Parent->Left) {
            while(p != null) {
                p->BalanceFactor++;
                p = p->Parent;
            }
            if( Parent->Parent != null && Parent->Parent->BalanceFactor > 1 ) {
                RotateRight(Parent->Parent);
            }
        } else {
            while(p != null) {
                p->BalanceFactor--;
                p = p->Parent;
            }
            if( Parent->Parent != null && Parent->Parent->BalanceFactor < -1 ) {
                RotateLeft(Parent->Parent);
            }
        }
    }
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
void MEMORY_POOL::RotateLeft(SEGMENT* Segment)
{
    printf("Rotating Left (%d segments, %d blocks, MaxDepth=%d)\n", NumSegments, NumBlocks, MaxDepth);
    assert(Segment->Right != null);
    SEGMENT*  Copy       = Segment;
    SEGMENT*  RightRight = Segment->Right->Right;
    SEGMENT*  RightLeft  = Segment->Right->Left;
    SEGMENT*  Left       = Segment->Left;

    if(Segment->Parent == null) {
        Segments = Segment->Right;
    } else {
        if(Segment->Parent->Left == Segment) {
            Segment->Parent->Left  = Segment->Right;
        } else if(Segment->Parent->Right == Segment) {
            Segment->Parent->Right = Segment->Right;
        } else {
            assert(false);
        }
    }
    Segment                = Segment->Right;
    Segment->Left          = Copy;
    Segment->Parent        = Copy->Parent;
    Segment->Left->Parent  = Segment;
    Segment->Left->Left    = Left;
    Segment->Left->Right   = RightLeft;
    Segment->Right         = RightRight;
    Segment->BalanceFactor++;
    Segment->Left->BalanceFactor++;
}
//******************************************************************************
void MEMORY_POOL::RotateRight(SEGMENT* Segment)
{
    printf("Rotating Right (%d segments, %d blocks, MaxDepth=%d)\n", NumSegments, NumBlocks, MaxDepth);
    assert(Segment->Left != null);
    SEGMENT*  Copy       = Segment;
    SEGMENT*  LeftLeft   = Segment->Left->Left;
    SEGMENT*  LeftRight  = Segment->Left->Right;
    SEGMENT*  Right      = Segment->Right;

    if(Segment->Parent == null) {
        Segments = Segment->Left;
    } else {
        if(Segment->Parent->Left == Segment) {
            Segment->Parent->Left  = Segment->Right;
        } else if(Segment->Parent->Right == Segment) {
            Segment->Parent->Right = Segment->Right;
        } else {
            assert(false);
        }
    }
    Segment                = Segment->Left;
    Segment->Right         = Copy;
    Segment->Parent        = Copy->Parent;
    Segment->Right->Parent = Segment;
    Segment->Right->Left   = LeftRight;
    Segment->Right->Right  = Right;
    Segment->Left          = LeftLeft;
    Segment->BalanceFactor--;
    Segment->Right->BalanceFactor--;
}
//******************************************************************************
