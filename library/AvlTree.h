
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
    void Insert ( T*    Node );
    void Remove ( u64   Key  );
    void Remove ( T*    Node );
    T*   Find   ( u64   Key  );

public:
    T*   Head;

private:
    void RotateLeft  ( T* Root );
    void RotateRight ( T* Root );
    int  GetBalance  ( T* Node );
    int  Height      ( T* Node );
    void UpdateParent( T* Node, T* NewChild );
    void Rebalance   ( T* Node );

    void Verify      ( T* a, T* b );
};

//******************************************************************************
// Constructor
//******************************************************************************
template < class T >
AVL_TREE<T>::AVL_TREE()
{
    Head = null;
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
    while( *s != null ) {
        p = *s;
        s = Node->Key < (*s)->Key ? &((*s)->Left) : &((*s)->Right);
    }
    *s = Node;
    (*s)->Height = 1;
    (*s)->Parent = p;
    
    Rebalance( p );
}
//******************************************************************************
template < class T >
void AVL_TREE<T>::Rebalance( T* Node )
{
    while( Node != null ) {
        Node->Height = max( Height(Node->Left), Height(Node->Right) ) + 1;
        int balance = GetBalance(Node);
        if( balance > 1 ) {
            if( Height(Node->Left->Right) > Height(Node->Left->Left) ) {
                RotateLeft(Node->Left);
            }
            RotateRight(Node);
        } else if( balance < -1 ) {
            if( Height(Node->Right->Left) > Height(Node->Right->Right) ) {
                RotateRight(Node->Right);
            }
            RotateLeft(Node);
        }
        Node = Node->Parent;
    }
}
//******************************************************************************
template < class T >
void AVL_TREE<T>::UpdateParent( T* Node, T* NewChild )
{
    if( Node->Parent == null ) {
        Head = NewChild;
    } else {
        if( Node->Parent->Right == Node ) {
            Node->Parent->Right = NewChild;
        } else if( Node->Parent->Left == Node ) {
            Node->Parent->Left = NewChild;
        } else {
            assert( false );
        }
    }
}
//******************************************************************************
template < class T >
void AVL_TREE<T>::Verify( T* Node, T* SearchNode )
{
    if( Node != null ) {
        assert( Node != SearchNode );
        assert( Node->Parent != SearchNode );
        Verify( Node->Left,  SearchNode );
        Verify( Node->Right, SearchNode );
    }
}
//******************************************************************************
template < class T >
void AVL_TREE<T>::Remove( u64 Key )
{
    T* Node = Find( Key );
    assert( Node != null );
    while( true ) {
        if( Node->Left == null && Node->Right == null ) {
            UpdateParent( Node, null );
            break;
        } else if( Node->Left == null ) {
            Node->Right->Parent = Node->Parent;
            UpdateParent( Node, Node->Right );
            break;
        } else if( Node->Right == null ) {
            Node->Left->Parent = Node->Parent;
            UpdateParent( Node, Node->Left );
            break;
        } else {
            if( Node->Parent == null ) {
                RotateRight( Node );
            } else if( Node == Node->Parent->Left ) {
                RotateRight( Node );
            } else if( Node == Node->Parent->Right ) {
                RotateLeft( Node );
            } else {
                assert( false );
            }
        }
    }

    if( Node->Parent != null ) {
        Node->Parent->Height = max( Height(Node->Parent->Left), Height(Node->Parent->Right) ) + 1;
    }

    assert( Find(Node->Key) == null );
    Verify( Head, Node );

    Rebalance( Node ); 
}
//******************************************************************************
//
template < class T >
void AVL_TREE<T>::Remove( T* Node )
{
    Remove( Node->Key );
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

    UpdateParent( Root, Pivot );

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

    UpdateParent( Root, Pivot );

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

#endif
