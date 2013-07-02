#include "AvlTree.h"

#include "stdlib.h"
#include "stdio.h"
#include "time.h"

struct SUPER_NODE : public AVL_TREE<SUPER_NODE>::NODE {
    int SuperInt;
};


//******************************************************************************
class TEST
{
public:
    TEST(const char* Title);
    virtual ~TEST() {};

public:
    bool Run();
    TEST* Next;

protected:
    virtual bool RunInternal() = 0;

    // Data
    bool Passed;
    AVL_TREE<SUPER_NODE> Tree;
    static const int MAX_NODES = 100;
    SUPER_NODE* Nodes[MAX_NODES];

private:
    char Buffer[100];
};

static TEST* Tests = null;

TEST::TEST(const char* Title)
{
    snprintf( Buffer, 100, "%-60s", Title );
    if( Tests == null ) {
        Tests = this;
    } else {
        TEST* Temp = Tests;
        while( Temp->Next != null ) {
            Temp = Temp->Next;
        }
        Temp->Next = this;
    }
}

bool TEST::Run()
{
    printf( "%s", Buffer );
    for(int i = 0; i < MAX_NODES; i++) {
        Nodes[i] = new SUPER_NODE();
        Nodes[i]->Key = i;
    }
    Passed = false;
    Passed = RunInternal();
    printf( "%s\n", Passed ? "Passed" : "Failed" );
    for(int i = 0; i < MAX_NODES; i++) {
        delete( Nodes[i] );
    }
    return Passed;
}
//******************************************************************************
    
//******************************************************************************
class TEST_1 : TEST 
{
    public: TEST_1() : TEST( "Insert a single element" ) {}
    protected: bool RunInternal()
    {
        Tree.Insert( Nodes[0] );
        return Tree.Head == Nodes[0];
    }
};
static TEST_1 Test1;
//******************************************************************************
class TEST_2 : TEST
{
    public: TEST_2() : TEST( "Remove a single element" ) { }
    protected: bool RunInternal()
    {
        Tree.Insert( Nodes[0] );
        Tree.Remove( Nodes[0]->Key );
        return Tree.Head == null;
    }
};
static TEST_2 Test2;
//******************************************************************************
class TEST_3 : TEST
{
    public: TEST_3() : TEST( "Insert two elements, remove first" ) { }
    protected: bool RunInternal()
    {
        Tree.Insert( Nodes[0] );
        Tree.Insert( Nodes[1] );
        Tree.Remove( Nodes[0]->Key );
        Passed =  Tree.Head == Nodes[1];
        Passed &= Tree.Head->Left == null;
        Passed &= Tree.Head->Right == null;
        return Passed;
    }
};
static TEST_3 Test3;
//******************************************************************************
class TEST_4 : TEST
{
    public: TEST_4() : TEST( "Insert two elements, writing values" ) { }
    protected: bool RunInternal()
    {
        Nodes[10]->SuperInt = 5000;
        Nodes[20]->SuperInt = 10023;
        Tree.Insert( Nodes[10] );
        Tree.Insert( Nodes[20] );
        SUPER_NODE* Node1 = Tree.Find( 10 );
        SUPER_NODE* Node2 = Tree.Find( 20 );
        Passed =  Node1->SuperInt == 5000;
        Passed &= Node2->SuperInt == 10023;
        return Passed;
    }
};
static TEST_4 Test4;
//******************************************************************************
class TEST_5 : TEST
{
    public: TEST_5() : TEST( "Tree rotations" ) { }
    protected: bool RunInternal()
    {
        // Test Left-Left rotation
        Tree.Insert(Nodes[30]);
        Tree.Insert(Nodes[40]);
        Tree.Insert(Nodes[50]);

        // Test Right-Right rotation
        Tree.Insert(Nodes[20]);
        Tree.Insert(Nodes[10]);

        // Test Right-Left rotation
        Tree.Insert(Nodes[25]);

        // Test Left-Right rotation
        Tree.Insert(Nodes[45]);
        Tree.Insert(Nodes[44]);
        Tree.Insert(Nodes[42]);
        Tree.Insert(Nodes[41]);

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
        return Passed;
    }
};
static TEST_5 Test5;
//******************************************************************************
class TEST_6 : TEST
{
    public: TEST_6() : TEST( "Remove an only child" ) { }
    protected: bool RunInternal()
    {
        Tree.Insert( Nodes[0] );
        Passed = Tree.Head == Nodes[0]; 
        Tree.Remove( Nodes[0] );
        Passed &= Tree.Head == null;
        return Passed;
    }
};
static TEST_6 Test6;
//******************************************************************************
class TEST_7 : TEST
{
    public: TEST_7() : TEST( "Remove an only child to the left" ) { }
    protected: bool RunInternal()
    {
        Tree.Insert( Nodes[10] );
        Tree.Insert( Nodes[5] );
        Tree.Remove( Nodes[5] );
        Passed = Tree.Head == Nodes[10];
        Passed &= Tree.Head->Left == null;
        Passed &= Tree.Head->Right == null;
        return Passed;
    }
};
static TEST_7 Test7;
//******************************************************************************
class TEST_8 : TEST
{
    public: TEST_8() : TEST( "Remove an only child to the right" ) { }
    protected: bool RunInternal()
    {
        Tree.Insert( Nodes[10] );
        Tree.Insert( Nodes[15] );
        Tree.Remove( Nodes[15] );
        Passed = Tree.Head == Nodes[10];
        Passed &= Tree.Head->Left == null;
        Passed &= Tree.Head->Right == null;
        return Passed;
    }
};
static TEST_8 Test8;
//******************************************************************************
class TEST_9 : TEST
{
    public: TEST_9() : TEST( "Remove a child from a right-heavy tree" ) { }
    protected: bool RunInternal()
    {
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
        return Passed;
    }
};
static TEST_9 Test9;
//******************************************************************************
class TEST_10 : TEST
{
    public: TEST_10() : TEST( "Remove a child from a left-heavy tree" ) { }
    protected: bool RunInternal()
    {
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
        return Passed;
    }
};
static TEST_10 Test10;
//******************************************************************************
class TEST_11 : TEST
{
    public: TEST_11() : TEST( "Remove a node that has two children on the left" ) { }
    protected: bool RunInternal()
    {
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
        return Passed;
    }
};
static TEST_11 Test11;
//******************************************************************************
class TEST_12 : TEST
{
    public: TEST_12() : TEST( "Remove a node that has two children on the right" ) { }
    protected: bool RunInternal()
    {
        Tree.Insert( Nodes[30] );
        Tree.Insert( Nodes[40] );
        Tree.Insert( Nodes[50] );
        Tree.Insert( Nodes[45] );
        Tree.Insert( Nodes[55] );
        Tree.Remove( Nodes[50] );
        
        Passed =  Tree.Head == Nodes[40];
        Passed &= Tree.Head->Left == Nodes[30];
        Passed &= Tree.Head->Right == Nodes[45];
        Passed &= Tree.Head->Right->Right == Nodes[55];
        return Passed;
    }
};
static TEST_12 Test12;
//******************************************************************************
class TEST_13 : TEST
{
    public: TEST_13() : TEST( "Random test" ) { }
    protected: bool RunInternal()
    {
        Tree.Insert( Nodes[92] );
        Tree.Insert( Nodes[74] );
        Tree.Insert( Nodes[4] );
        Tree.Remove( Nodes[74] );
        Tree.Insert( Nodes[76] );
        Tree.Insert( Nodes[32] );
        Tree.Insert( Nodes[13] );
        Tree.Insert( Nodes[65] );
        
        Passed =  Tree.Head == Nodes[32];
        Passed &= Tree.Head->Left == Nodes[13];
        Passed &= Tree.Head->Left->Left == Nodes[4];
        Passed &= Tree.Head->Right == Nodes[76];
        Passed &= Tree.Head->Right->Right == Nodes[92];
        Passed &= Tree.Head->Right->Left == Nodes[65];
        
        Tree.Remove( Nodes[13] );
        Tree.Insert( Nodes[66] );
        
        Passed &= Tree.Head == Nodes[65];
        Passed &= Tree.Head->Left == Nodes[32];
        Passed &= Tree.Head->Left->Left == Nodes[4];
        Passed &= Tree.Head->Right == Nodes[76];
        Passed &= Tree.Head->Right->Right == Nodes[92];
        Passed &= Tree.Head->Right->Left == Nodes[66];
        return Passed;
    }
};
static TEST_13 Test13;
//******************************************************************************
class TEST_14 : TEST
{
    public: TEST_14() : TEST( "Remove a parent from a full tree" ) { }
    protected: bool RunInternal()
    {
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
        return Passed;
    }
};
static TEST_14 Test14;
//******************************************************************************
class TEST_15 : TEST
{
    public: TEST_15() : TEST( "Add/Remove iterations" ) {}
    protected: bool RunInternal()
    {
        printf("\n");
        for(int i = 0; i < MAX_NODES; i++) {
            Nodes[i]->SuperInt = -1;
        }

        for(int i = 1; i < 1000; i++) {
            int index = rand() % MAX_NODES;
            if( Nodes[index]->SuperInt == -1 ) {
                printf("\tInsert %d\n", index);
                //Nodes[index]->Key = index;
                Nodes[index]->SuperInt = index;
                Tree.Insert( Nodes[index] );
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

        return Tree.Head == null;
    }
};
static TEST_15 Test15;
//******************************************************************************




//******************************************************************************
int main()
{
    int counter = 0;
    int PassedCounter = 0;
    srand(time(null));

    printf("Starting Test suite...\n");
    printf("------------------------------------------------------------\n");
    TEST* TestIter = Tests;
    while( TestIter != null ) {
        printf("%3d: ", ++counter);
        if(TestIter->Run()) PassedCounter++;
        TestIter = TestIter->Next;
    }
    printf("Finished test suite...\n");
    printf("%d / %d passed\n", PassedCounter, counter);
}
//******************************************************************************
