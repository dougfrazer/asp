
#include "ASPLib.h"

#include "stdio.h"

int main()
{
    void* Data[10];
    printf("Test 1: malloc some data...");
    Data[0] = Malloc( 5 );
    Data[1] = Malloc( 20 );
    Data[2] = Malloc( 150 );
    Data[3] = Malloc( 20020 );
    Data[4] = Malloc( 2301 );
    printf("Passed\n");

    printf("Test 2: Free data..."); 
    for(int i = 0; i < 5; i++ ) {
        Free( Data[i] );
    }
    printf("Passed\n");


    return 0;
}
