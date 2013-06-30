
#ifndef __AVL_TREE_H__
#define __AVL_TREE_H__

#include "ASPTypes.h"
#include "assert.h"

template <class T>
class AVL_TREE
{
public:
    struct NODE
    {
        u64   Key;
        int   Height;
        T* Parent;
        T* Left;
        T* Right;
    };
public:
    AVL_TREE();
    virtual ~AVL_TREE();

public:
    void Insert ( T* Node );
    void Remove ( u64   Key  );
    T*   Find   ( u64   Key  );

private:
    void RotateLeft ( T* Root );
    void RotateRight( T* Root );
    int  GetBalance ( T* Node );
    int  Height     ( T* Node );

    T* Head;

    void TestInternal ( void );
};

//******************************************************************************
// Constructor
//******************************************************************************
template < class T >
AVL_TREE<T>::AVL_TREE()
{
    Head = null;
//    TestInternal();
}
//******************************************************************************
template < class T >
AVL_TREE<T>::~AVL_TREE()
{

}
//******************************************************************************
//******************************************************************************

//******************************************************************************
// Public Interface
//******************************************************************************
template < class T >
void AVL_TREE<T>::Insert( T* Node )
{
    T** s  = &Head;
    T*  p  = null;
    T*  In = Node;
    while( *s != null ) {
        p = *s;
        s = In->Key < (*s)->Key ? &((*s)->Left) : &((*s)->Right);
    }
    *s = In;
    (*s)->Height = 1;
    (*s)->Parent = p;

    while( p != null ) {
        p->Height = max( Height(p->Left), Height(p->Right) ) + 1;
        int balance = GetBalance(p);
        if( balance > 1 ) {
            if( In->Key < p->Left->Key ) { // Left Left case
                RotateRight(p);
            } else { // Left Right case
                RotateLeft(p->Left);
                RotateRight(p);
            }
        } else if( balance < -1 ) {
            if( In->Key >= p->Right->Key ) { // Right Right case
                RotateLeft(p);
            } else { // Right Left case
                RotateRight(p->Right);
                RotateLeft(p);
            }
        }
        p = p->Parent;
    }
}
//******************************************************************************
template < class T >
void AVL_TREE<T>::Remove( u64 Key )
{
    // TODO
}
//******************************************************************************
template < class T >
T* AVL_TREE<T>::Find( u64 Key )
{
    T* i = Head;
    while( i != null ) {
        if(i->Key == Key) break;
        else if( Key < i->Key ) i = i->Left;
        else i = i->Right;
    }
    return i;
}
//******************************************************************************
//******************************************************************************


//******************************************************************************
// Private Interface
//******************************************************************************
template < class T >
void AVL_TREE<T>::RotateLeft( T* Root )
{
    T* Pivot = Root->Right;

    if( Root->Parent == null ) {
        Head = Pivot;
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
template < class T >
void AVL_TREE<T>::RotateRight( T* Root )
{
    T* Pivot = Root->Left;

    if( Root->Parent == null ) {
        Head = Pivot;
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
template < class T >
int AVL_TREE<T>::GetBalance( T* Node )
{
    if( Node == null ) return 0;
    return Height( Node->Left ) - Height( Node->Right );
}
//******************************************************************************
template < class T >
int AVL_TREE<T>::Height( T* Node )
{
    return Node == null ? 0 : Node->Height;
}
//******************************************************************************
//******************************************************************************



//******************************************************************************
// Test code
//******************************************************************************
template < class T >
void AVL_TREE<T>::TestInternal()
{
    assert(Head == null);

    const uint NUM_TEST_NODES = 100;
    NODE* TestNodes[NUM_TEST_NODES];
    for(int i = 1; i < NUM_TEST_NODES; i++) {
        TestNodes[i] = new NODE();
        TestNodes[i]->Key = i;
    }
    
    // Test Left-Left rotation
    Insert(TestNodes[30]);
    Insert(TestNodes[40]);
    Insert(TestNodes[50]);

    // Test Right-Right rotation
    Insert(TestNodes[20]);
    Insert(TestNodes[10]);

    // Test Right-Left rotation
    Insert(TestNodes[25]);

    // Test Left-Right rotation
    Insert(TestNodes[45]);
    Insert(TestNodes[44]);
    Insert(TestNodes[42]);
    Insert(TestNodes[41]);

    assert(Head->Key == 30);
    assert(Head->Left->Key == 20);
    assert(Head->Left->Left->Key == 10);
    assert(Head->Left->Right->Key == 25);
    assert(Head->Right->Key == 42);
    assert(Head->Right->Left->Key == 40);
    assert(Head->Right->Left->Right->Key == 41);
    assert(Head->Right->Right->Key == 45);
    assert(Head->Right->Right->Left->Key == 44);
    assert(Head->Right->Right->Right->Key == 50);
}
//******************************************************************************

#endif
