#include "AvlTree.h"

#include "stdlib.h"
#include "stdio.h"

struct SUPER_NODE : public NODE {
    int SuperInt;
};

int main()
{
    char Buffer[100];
    bool Passed = false;

    snprintf( Buffer, 100, "%-100s", "Starting Test 1: Insert a single element" );
	printf( "%s", Buffer );
    Passed = false;
    {
        AVL_TREE<NODE> Tree;
        SUPER_NODE* Test = new SUPER_NODE();
        Tree.Insert( Test );
        delete(Test);
        Passed = true;
    }
    printf("%s\n", Passed ? "passed" : "failed");
    
    snprintf( Buffer, 100, "%-100s", "Starting Test 2: Remove a single element" );
	printf( "%s", Buffer );
    Passed = false;
    {
        AVL_TREE<NODE> Tree;
        SUPER_NODE* Test = new SUPER_NODE();
        Tree.Insert( Test );
        Tree.Remove( Test );
        delete(Test);
        Passed = true;
    }
    printf("%s\n", Passed ? "passed" : "failed");
    
    snprintf( Buffer, 100, "%-100s", "Starting Test 3: Insert two elements, remove first" );
	printf( "%s", Buffer );
    Passed = false;
    {
        AVL_TREE<NODE> Tree;
        SUPER_NODE* Test1 = new SUPER_NODE();
        SUPER_NODE* Test2 = new SUPER_NODE();
        Tree.Insert( Test1 );
        Tree.Insert( Test2 );
        Tree.Remove( Test1 );
        delete( Test1 );
        delete( Test2 );
        Passed = true;
    }
    printf("%s\n", Passed ? "passed" : "failed");
    
    snprintf( Buffer, 100, "%-100s", "Starting Test 4: Insert two elements, writing values" );
	printf( "%s", Buffer );
    Passed = false;
    {
        AVL_TREE<NODE> Tree;
        SUPER_NODE* Test1 = new SUPER_NODE();
        SUPER_NODE* Test2 = new SUPER_NODE();
        Test1->Key = 10;
        Test2->Key = 20;
        Test1->SuperInt = 5000;
        Test2->SuperInt = 10023;
        Tree.Insert( Test1 );
        Tree.Insert( Test2 );
        SUPER_NODE* Node1 = static_cast<SUPER_NODE*>(Tree.Find( 10 ));
        SUPER_NODE* Node2 = static_cast<SUPER_NODE*>(Tree.Find( 20 ));
        Passed |= Node1->SuperInt == 5000;
        Passed |= Node2->SuperInt == 10023;
    }
    printf("%s\n", Passed ? "passed" : "failed");
}
