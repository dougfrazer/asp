#include "AvlTree.h"

#include "stdlib.h"
#include "stdio.h"
#include "time.h"

struct SUPER_NODE : public AVL_TREE<SUPER_NODE>::NODE {
    int SuperInt;
};

int main()
{
    char Buffer[100];
    srand(time(null));

    {
        snprintf( Buffer, 100, "%-100s", "1: Insert a single element" );
        printf( "%s", Buffer );
        bool Passed = false;
        AVL_TREE<SUPER_NODE> Tree;
        SUPER_NODE* Test = new SUPER_NODE();
        Test->Key = 100;
        Tree.Insert( Test );
        Passed = Tree.Head == Test;
        printf("%s\n", Passed ? "passed" : "failed");
    }
    
    {
        snprintf( Buffer, 100, "%-100s", "2: Remove a single element" );
        printf( "%s", Buffer );
        bool Passed = false;
        AVL_TREE<SUPER_NODE> Tree;
        SUPER_NODE* Test = new SUPER_NODE();
        Test->Key = 100;
        Tree.Insert( Test );
        Tree.Remove( Test->Key );
        Passed = Tree.Head == null;
        printf("%s\n", Passed ? "passed" : "failed");
    }
    
    {
        snprintf( Buffer, 100, "%-100s", "3: Insert two elements, remove first" );
        printf( "%s", Buffer );
        bool Passed = false;
        AVL_TREE<SUPER_NODE> Tree;
        SUPER_NODE* Test1 = new SUPER_NODE();
        SUPER_NODE* Test2 = new SUPER_NODE();
        Test1->Key = 100;
        Test2->Key = 200;
        Tree.Insert( Test1 );
        Tree.Insert( Test2 );
        Tree.Remove( Test1->Key );
        Passed =  Tree.Head == Test2;
        Passed &= Tree.Head->Left == null;
        Passed &= Tree.Head->Right == null;
        printf("%s\n", Passed ? "passed" : "failed");
    }
    
    {
        snprintf( Buffer, 100, "%-100s", "4: Insert two elements, writing values" );
        printf( "%s", Buffer );
        bool Passed = false;
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
        Passed =  Node1->SuperInt == 5000;
        Passed &= Node2->SuperInt == 10023;
        printf("%s\n", Passed ? "passed" : "failed");
    }

    {
        snprintf( Buffer, 100, "%-100s", "5: Test all rotation possibilities" );
        printf( "%s", Buffer );
        bool Passed = false;
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

        Passed = Tree.Head->Key == 30;
        Passed &= Tree.Head->Left->Key == 20;
        Passed &= Tree.Head->Left->Left->Key == 10;
        Passed &= Tree.Head->Left->Right->Key == 25;
        Passed &= Tree.Head->Right->Key == 42;
        Passed &= Tree.Head->Right->Left->Key == 40;
        Passed &= Tree.Head->Right->Left->Right->Key == 41;
        Passed &= Tree.Head->Right->Right->Key == 45;
        Passed &= Tree.Head->Right->Right->Left->Key == 44;
        Passed &= Tree.Head->Right->Right->Right->Key == 50;
        printf("%s\n", Passed ? "passed" : "failed");
    }

    {
        snprintf( Buffer, 100, "%-100s", "6: Remove an only child" );
        printf( "%s", Buffer );
        bool Passed = false;
        AVL_TREE<SUPER_NODE> Tree;
        SUPER_NODE* Node = new SUPER_NODE();
        Node->Key = 100;
        Tree.Insert( Node );
        Passed = Tree.Head == Node; 
        Tree.Remove( Node );
        Passed &= Tree.Head == null;
        printf("%s\n", Passed ? "passed" : "failed");
    }

    {
        snprintf( Buffer, 100, "%-100s", "7: Remove single child to the left" );
        printf( "%s", Buffer );
        bool Passed = false;
        AVL_TREE<SUPER_NODE> Tree;
        SUPER_NODE* Node = new SUPER_NODE();
        SUPER_NODE* Left = new SUPER_NODE();
        Node->Key = 100;
        Left->Key = 50;
        Tree.Insert( Node );
        Tree.Insert( Left );
        Tree.Remove( Left );
        Passed = Tree.Head == Node;
        Passed &= Tree.Head->Left == null;
        Passed &= Tree.Head->Right == null;
        printf("%s\n", Passed ? "passed" : "failed");
    }

    {
        snprintf( Buffer, 100, "%-100s", "8: Remove single child to the right" );
        printf( "%s", Buffer );
        bool Passed = false;
        AVL_TREE<SUPER_NODE> Tree;
        SUPER_NODE* Node = new SUPER_NODE();
        SUPER_NODE* Right = new SUPER_NODE();
        Node->Key = 100;
        Right->Key = 200;
        Tree.Insert( Node );
        Tree.Insert( Right );
        Tree.Remove( Right );
        Passed = Tree.Head == Node;
        Passed &= Tree.Head->Left == null;
        Passed &= Tree.Head->Right == null;
        printf("%s\n", Passed ? "passed" : "failed");
    }

    {
        snprintf( Buffer, 100, "%-100s", "9: Remove children from right-heavy tree" );
        printf( "%s", Buffer );
        bool Passed = false;
        AVL_TREE<SUPER_NODE> Tree;
        const int MAX_NODES = 100;
        SUPER_NODE* Nodes[MAX_NODES];
        for(int i = 0; i < MAX_NODES; i++) {
            Nodes[i] = new SUPER_NODE();
            Nodes[i]->Key = i;
        }
        Tree.Insert( Nodes[30] );
        Tree.Insert( Nodes[40] );
        Tree.Insert( Nodes[50] );
        Tree.Insert( Nodes[55] );
        Tree.Insert( Nodes[45] );
        Tree.Remove( Nodes[45] );
        Tree.Remove( Nodes[55] );

        Passed =  Tree.Head == Nodes[40];
        Passed &= Tree.Head->Left == Nodes[30];
        Passed &= Tree.Head->Right == Nodes[50];
        Passed &= Tree.Head->Right->Right == null;
        Passed &= Tree.Head->Right->Left == null;
        
        for(int i =0 ; i < MAX_NODES; i++) {
            delete(Nodes[i]);
        }
        printf("%s\n", Passed ? "passed" : "failed");
    }

    {
        snprintf( Buffer, 100, "%-100s", "10: Remove children from a left-heavy tree" );
        printf( "%s", Buffer );
        bool Passed = false;
        AVL_TREE<SUPER_NODE> Tree;
        const int MAX_NODES = 100;
        SUPER_NODE* Nodes[MAX_NODES];
        for(int i = 0; i < MAX_NODES; i++) {
            Nodes[i] = new SUPER_NODE();
            Nodes[i]->Key = i;
        }
        Tree.Insert( Nodes[30] );
        Tree.Insert( Nodes[40] );
        Tree.Insert( Nodes[50] );
        Tree.Insert( Nodes[25] );
        Tree.Insert( Nodes[35] );
        Tree.Remove( Nodes[35] );
        Tree.Remove( Nodes[25] );
        
        Passed =  Tree.Head == Nodes[40];
        Passed &= Tree.Head->Left == Nodes[30];
        Passed &= Tree.Head->Right == Nodes[50];
        Passed &= Tree.Head->Left->Left == null;
        Passed &= Tree.Head->Left->Right == null;
        
        printf("%s\n", Passed ? "passed" : "failed");
    }
    {
        snprintf( Buffer, 100, "%-100s", "11: Remove a node that has two children on left" );
        printf( "%s", Buffer );
        bool Passed = false;
        
        AVL_TREE<SUPER_NODE> Tree;
        const int MAX_NODES = 100;
        SUPER_NODE* Nodes[MAX_NODES];
        for(int i = 0; i < MAX_NODES; i++) {
            Nodes[i] = new SUPER_NODE();
            Nodes[i]->Key = i;
        }
        Tree.Insert( Nodes[30] );
        Tree.Insert( Nodes[40] );
        Tree.Insert( Nodes[50] );
        Tree.Insert( Nodes[25] );
        Tree.Insert( Nodes[35] );
        Tree.Remove( Nodes[30] );
        
        Passed =  Tree.Head == Nodes[40];
        Passed &= Tree.Head->Left == Nodes[25];
        Passed &= Tree.Head->Left->Right == Nodes[35];
        Passed &= Tree.Head->Right == Nodes[50];
        
        printf("%s\n", Passed ? "passed" : "failed");
    }

    {
        snprintf( Buffer, 100, "%-100s", "12: Remove a node that has two children on right" );
        printf( "%s", Buffer );
        bool Passed = false;
        
        AVL_TREE<SUPER_NODE> Tree;
        const int MAX_NODES = 100;
        SUPER_NODE* Nodes[MAX_NODES];
        for(int i = 0; i < MAX_NODES; i++) {
            Nodes[i] = new SUPER_NODE();
            Nodes[i]->Key = i;
        }
        Tree.Insert( Nodes[30] );
        Tree.Insert( Nodes[40] );
        Tree.Insert( Nodes[50] );
        Tree.Insert( Nodes[45] );
        Tree.Insert( Nodes[55] );
        Tree.Remove( Nodes[50] );
        
        Passed =  Tree.Head == Nodes[40];
        Passed &= Tree.Head->Left == Nodes[30];
        Passed &= Tree.Head->Right == Nodes[55];
        Passed &= Tree.Head->Right->Left == Nodes[45];
        
        printf("%s\n", Passed ? "passed" : "failed");
    }
    
    {
        snprintf( Buffer, 100, "%-100s", "13: Remove parent from full tree" );
        printf( "%s", Buffer );
        bool Passed = false;
        
        AVL_TREE<SUPER_NODE> Tree;
        const int MAX_NODES = 100;
        SUPER_NODE* Nodes[MAX_NODES];
        for(int i = 0; i < MAX_NODES; i++) {
            Nodes[i] = new SUPER_NODE();
            Nodes[i]->Key = i;
        }
        Tree.Insert( Nodes[30] );
        Tree.Insert( Nodes[40] );
        Tree.Insert( Nodes[50] );
        Tree.Insert( Nodes[45] );
        Tree.Insert( Nodes[55] );
        Tree.Insert( Nodes[25] );
        Tree.Insert( Nodes[35] );

        Passed = Tree.Head == Nodes[40];

        Tree.Remove( Nodes[40] );
        
        Passed &= Tree.Head == Nodes[35];
        Passed &= Tree.Head->Left == Nodes[30];
        Passed &= Tree.Head->Left->Left == Nodes[25];
        Passed &= Tree.Head->Right == Nodes[50];
        Passed &= Tree.Head->Right->Right == Nodes[55];
        Passed &= Tree.Head->Right->Left == Nodes[45];
        
        printf("%s\n", Passed ? "passed" : "failed");
    }

    {
        snprintf( Buffer, 100, "%-100s", "14: Add / Remove iterations" );
        printf( "%s", Buffer );
        bool Passed = false;
        
        printf("\n");
        AVL_TREE<SUPER_NODE> Tree;
        const int MAX_NODES = 100;
        SUPER_NODE* Nodes[MAX_NODES];
        for(int i = 0; i < MAX_NODES; i++) {
            Nodes[i] = new SUPER_NODE();
            Nodes[i]->Key = i;
            Nodes[i]->SuperInt = -1;
        }

        for(int i = 1; i < 1000; i++) {
            int index = rand() % MAX_NODES;
            if( Nodes[index]->SuperInt == -1 ) {
                printf("\tInsert %d\n", index);
                Tree.Insert( Nodes[index] );
                Nodes[index]->SuperInt = index;
            } else {
                printf("\tRemove %d\n", index);
                Tree.Remove( Nodes[index] );
                Nodes[index]->SuperInt = -1;
            }
        }

        for(int i = 0; i < MAX_NODES; i++) {
            if( Nodes[i]->SuperInt != 0 ) {
                Tree.Remove( Nodes[i] );
            }
        }

        Passed = Tree.Head == null;
        
        printf("%s\n", Passed ? "passed" : "failed");
    }

}
