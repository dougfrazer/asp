#include "AvlTree.h"

#include "stdlib.h"
#include "stdio.h"

struct SUPER_NODE : public AVL_TREE<SUPER_NODE>::NODE {
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
        AVL_TREE<SUPER_NODE> Tree;
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
        AVL_TREE<SUPER_NODE> Tree;
        SUPER_NODE* Test = new SUPER_NODE();
        Tree.Insert( Test );
        Tree.Remove( Test->Key );
        delete(Test);
        Passed = true;
    }
    printf("%s\n", Passed ? "passed" : "failed");
    
    snprintf( Buffer, 100, "%-100s", "Starting Test 3: Insert two elements, remove first" );
	printf( "%s", Buffer );
    Passed = false;
    {
        AVL_TREE<SUPER_NODE> Tree;
        SUPER_NODE* Test1 = new SUPER_NODE();
        SUPER_NODE* Test2 = new SUPER_NODE();
        Tree.Insert( Test1 );
        Tree.Insert( Test2 );
        Tree.Remove( Test1->Key );
        delete( Test1 );
        delete( Test2 );
        Passed = true;
    }
    printf("%s\n", Passed ? "passed" : "failed");
    
    snprintf( Buffer, 100, "%-100s", "Starting Test 4: Insert two elements, writing values" );
	printf( "%s", Buffer );
    Passed = false;
    {
        Passed = true;
        AVL_TREE<SUPER_NODE> Tree;
        SUPER_NODE* Test1 = new SUPER_NODE();
        SUPER_NODE* Test2 = new SUPER_NODE();
        Test1->Key = 10;
        Test2->Key = 20;
        Test1->SuperInt = 5000;
        Test2->SuperInt = 10023;
        Tree.Insert( Test1 );
        Tree.Insert( Test2 );
        SUPER_NODE* Node1 = Tree.Find( 10 );
        SUPER_NODE* Node2 = Tree.Find( 20 );
        Passed &= Node1->SuperInt == 5000;
        Passed &= Node2->SuperInt == 10023;
    }
    printf("%s\n", Passed ? "passed" : "failed");

    snprintf( Buffer, 100, "%-100s", "Starting Test 5: Test all rotation possibilities" );
	printf( "%s", Buffer );
    Passed = false;
    {
        AVL_TREE<SUPER_NODE> Tree;
        const uint NUM_NODES = 100;
        SUPER_NODE* TestNodes[NUM_NODES];
        for(uint i = 1; i < NUM_NODES; i++) {
            TestNodes[i] = new SUPER_NODE();
            TestNodes[i]->Key = i;
            TestNodes[i]->SuperInt = i;
        }
        
        // Test Left-Left rotation
        Tree.Insert(TestNodes[30]);
        Tree.Insert(TestNodes[40]);
        Tree.Insert(TestNodes[50]);

        // Test Right-Right rotation
        Tree.Insert(TestNodes[20]);
        Tree.Insert(TestNodes[10]);

        // Test Right-Left rotation
        Tree.Insert(TestNodes[25]);

        // Test Left-Right rotation
        Tree.Insert(TestNodes[45]);
        Tree.Insert(TestNodes[44]);
        Tree.Insert(TestNodes[42]);
        Tree.Insert(TestNodes[41]);

        assert(Tree.Head->Key == 30);
        assert(Tree.Head->Left->Key == 20);
        assert(Tree.Head->Left->Left->Key == 10);
        assert(Tree.Head->Left->Right->Key == 25);
        assert(Tree.Head->Right->Key == 42);
        assert(Tree.Head->Right->Left->Key == 40);
        assert(Tree.Head->Right->Left->Right->Key == 41);
        assert(Tree.Head->Right->Right->Key == 45);
        assert(Tree.Head->Right->Right->Left->Key == 44);
        assert(Tree.Head->Right->Right->Right->Key == 50);
        Passed = true;
    }
    printf("%s\n", Passed ? "passed" : "failed");
}
