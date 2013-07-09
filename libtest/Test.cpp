#include "Test.h"

#include "stdlib.h"
#include "stdio.h"

static TEST* Head;
 
TEST::TEST(const char* Title)
{
    snprintf( Buffer, 100, "%-60s", Title );
    
    // add it to a cyclic list
    if(Head == NULL) {
        Head       = this;
        Head->Prev = this;
        Head->Next = this;
    } else {
        this->Prev = Head->Prev;
        this->Next = Head->Prev->Next;
        Head->Prev->Next = this;
        Head->Prev = this;
    }
}

bool TEST::Run()
{
    printf( "%s", Buffer );
    Setup();
    Passed = false;
    Passed = RunInternal();
    printf( "%s\n", Passed ? "Passed" : "Failed" );
    Cleanup();
    return Passed;
}

TEST* TEST::GetFirst()
{
    return Head;
}

TEST* TEST::GetNext( TEST* Test )
{
    return Test->Next == Head ? NULL : Test->Next;
}
