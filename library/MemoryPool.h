
#include "ASPLib.h"

class MEMORY_POOL
{
public:
    //**************************************************************************
    // BlockSize:
    //   The size of each block to be created.
    //  
    // AllocationSize:
    //   The size the pool will grow when it needs more memory.
    //**************************************************************************
	MEMORY_POOL(u32 BlockSize, u32 AllocationSize);
    virtual ~MEMORY_POOL();

public:
    //**************************************************************************
    // Returns a ptr to a free block of the size passed in at instantiation
    //**************************************************************************
    void* GetBlock();

    //**************************************************************************
    // Frees the block referenced by the ptr
    //**************************************************************************
    void  FreeBlock(void* ptr);

private:
    struct BLOCK
    {
        BLOCK* Next;
    };

    struct SEGMENT
    {
        void*    Memory;
        BLOCK*   FreeBlock;
        SEGMENT* NextFreeSegment;
        
        // Tree Data
        SEGMENT* Parent;
        int      BalanceFactor;
        SEGMENT* Left;
        SEGMENT* Right;
    };
    SEGMENT*  GetNewSegment   ( void );
    SEGMENT*  FindSegment     ( void* ptr );
    void      AddToFreeList   ( SEGMENT* Segment );
    
    void      InsertIntoTree  ( SEGMENT* Segment );
    void      RotateLeft      ( SEGMENT* Segment );
    void      RotateRight     ( SEGMENT* Segment );
    
    // Data
    u32      BlockSize;
    u32      AllocationSize;
    SEGMENT* Segments;
    SEGMENT* FreeSegments;

    // Debug data
    int      NumBlocks;
    int      NumSegments;
    int      MaxDepth;
};
