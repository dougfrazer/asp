#include "List.h"
#include "Test.h"

#include "stdlib.h"
#include "stdio.h"
#include "time.h"

struct LIST_NODE : public LIST<LIST_NODE>::NODE {
    int value;
};


//******************************************************************************
// Test class
//******************************************************************************
class LIST_TEST : public TEST
{
public:
    LIST_TEST(const char* Title) : TEST(Title) {}
    virtual ~LIST_TEST() {}

protected:
    virtual bool RunInternal() = 0;
    virtual void Setup();
    virtual void Cleanup();

    LIST<LIST_NODE> List;
    static const int MAX_NODES = 100;
    LIST_NODE* Nodes[MAX_NODES];
};
void LIST_TEST::Setup()
{
    for(int i = 0; i < MAX_NODES; i++) {
        Nodes[i] = new LIST_NODE();
        Nodes[i]->value = i;
    }
}
void LIST_TEST::Cleanup()
{
    for(int i = 0 ; i < MAX_NODES; i++) {
        delete( Nodes[i] );
    }
}
//******************************************************************************


//******************************************************************************
// Tests
//******************************************************************************
class TEST_1 : public LIST_TEST
{
    public: TEST_1() : LIST_TEST( "Add and remove" ) {}
    protected: bool RunInternal()
    {
        List.Add( Nodes[0] );
        List.Add( Nodes[1] );
        List.Remove( Nodes[1] );
        List.Remove( Nodes[0] );
        return true;
    }
};
static TEST_1 Test1;
//******************************************************************************
class TEST_2 : public LIST_TEST
{
    public: TEST_2() : LIST_TEST( "Add and remove head" ) {}
    protected: bool RunInternal()
    {
        List.Add( Nodes[0] );
        List.Add( Nodes[1] );
        List.Remove( Nodes[0] );
        List.Remove( Nodes[1] );
        List.Add( Nodes[20] );
        List.Remove( Nodes[20] );
        return true;
    }
};
static TEST_2 Test2;
//******************************************************************************
class TEST_3 : public LIST_TEST
{
    struct MANAGER_NODE : public LIST<MANAGER_NODE>::NODE {

    };

    struct CLIENT_NODE : public LIST<CLIENT_NODE>::NODE {

    };

    struct DATA {
        MANAGER_NODE ManagerNode;
        CLIENT_NODE  ClientNode;
    };

    public: TEST_3() : LIST_TEST( "Element in multiple lists" ) {}
    protected: bool RunInternal()
    {
        
        DATA Value;

        LIST<MANAGER_NODE> List1;
        LIST<CLIENT_NODE> List2;
        
        List1.Add( &Value.ManagerNode );
        List2.Add( &Value.ClientNode );
        
        return true;
    }
};
static TEST_3 Test3;
//******************************************************************************
class TEST_RANDOM : public LIST_TEST
{
    public: TEST_RANDOM() : LIST_TEST( "Random add/remove" ) {}
    protected: bool RunInternal()
    {
        for(int i = 0; i < MAX_NODES; i++) {
            Nodes[i]->value = -1;
        }

        for(int i = 1; i < 100000; i++) {
            int index = rand() % MAX_NODES;
            if( Nodes[index]->value == -1 ) {
                Nodes[index]->value = index;
                List.Add( Nodes[index] );
            } else {
                List.Remove( Nodes[index] );
                Nodes[index]->value = -1;
            }
        }

        for(int i = 0; i < MAX_NODES; i++) {
            if( Nodes[i]->value != -1 ) {
                List.Remove( Nodes[i] );
            }
        }
        return List.GetFirst() == null;
    }
};
static TEST_RANDOM TestRandom;
//******************************************************************************


//******************************************************************************
// Main function
//******************************************************************************
int main()
{
    int counter = 0;
    int PassedCounter = 0;
    srand(time(null));

    printf("Starting Test suite...\n");
    printf("------------------------------------------------------------\n");
    TEST* TestIter = TEST::GetFirst();
    while( TestIter != null ) {
        printf("%3d: ", ++counter);
        if(TestIter->Run()) PassedCounter++;
        TestIter = TEST::GetNext( TestIter );
    }
    printf("Finished test suite...\n");
    printf("%d / %d passed\n", PassedCounter, counter);
}
