
#ifndef __LIST_H__
#define __LIST_H__

#define null 0

template <class T>
class LIST
{
public:
    struct NODE
    {
        T* Prev;
        T* Next;
    };
public:
    LIST();
    virtual ~LIST();

public:
    bool Add      ( T* Node );
    bool Remove   ( T* Node );
    T*   GetFirst ( void );
    T*   GetNext  ( T* Node );

public:
    T* Head;
};


// Constructor
template < class T > LIST<T>::LIST()  { Head = null; }
template < class T > LIST<T>::~LIST() { }
template < class T > T* LIST<T>::GetFirst() { return Head; }
template < class T > T* LIST<T>::GetNext( T* Node ) { return Node->Next == Head ? null : Node->Next; }

// Public interface
template < class T >
bool LIST<T>::Add( T* Node )
{
    if(Head == null) {
        Head       = Node;
        Head->Prev = Node;
        Head->Next = Node;
    } else {
        Node->Prev = Head->Prev;
        Node->Next = Head->Prev->Next;
        Head->Prev->Next = Node;
        Head->Prev = Node;
    }
    return true;
}
//******************************************************************************
template < class T >
bool LIST<T>::Remove( T* Node )
{
    T* Iter = GetFirst();
    while( Iter != Node && Iter != null ) {
        Iter = GetNext( Iter );
    }
    if( Iter == null ) {
        return false;
    }
    if( Iter == Head ) {
        Head = GetNext( Iter );
    }
    if( Iter->Prev != Iter ) {
        Iter->Prev->Next = Iter->Next;
    } 
    if( Iter->Next != Iter ) {
        Iter->Next->Prev = Iter->Prev;
    }
    Iter->Prev = null;
    Iter->Next = null;
    return true;
}
#endif
