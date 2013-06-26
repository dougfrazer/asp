#include "MemoryPool.h"
#include "stdio.h"

#include "stdlib.h"
#include "time.h"

int main()
{
    printf("Starting MemoryPoolTest\n");
    struct STUFF {
        int one;
        u64 two;
        u8  three;
    };
    MEMORY_POOL Pool(sizeof(STUFF), 1024);

    srand(time(null));

    printf("Starting test 1: get a block...");
    STUFF* Blah = (STUFF*)Pool.GetBlock();
    Blah->one = 5;
    Blah->two = 17;
    Blah->three = 123;
    printf("Passed\n");

    printf("Starting test 2: free a block...");
    Pool.FreeBlock(Blah);
    printf("Passed\n");

    printf("Starting test 3: get, free, get, expect same block...");
    Blah = (STUFF*)Pool.GetBlock();
    STUFF* Temp = Blah;
    Pool.FreeBlock(Blah);
    Blah = (STUFF*)Pool.GetBlock();
    Pool.FreeBlock(Blah);
    printf("Got ptr %p and %p... %s\n", Temp, Blah, Temp == Blah ? "Passed" : "Failed");

    printf("Starting test 4: free a block twice in a row...");
    Blah = (STUFF*)Pool.GetBlock();
    Pool.FreeBlock(Blah);
    Pool.FreeBlock(Blah);
    printf("Passed\n");

    static const uint NUM_STUFF = 10000;
    printf("Starting test 5: randomly create and free %d times...", NUM_STUFF*10);
    STUFF* LoadsOfStuff[NUM_STUFF];
    Memset( &LoadsOfStuff[0], '\0', sizeof(LoadsOfStuff) );
    int BlockCount = 0;
    for(int i = 0; i < NUM_STUFF * 10; i++) {
        int index = rand()%NUM_STUFF;
        if(rand() % 2 == 0) {
            if(LoadsOfStuff[index] == null) {
                //printf("[%d] Get\n", BlockCount++);
                LoadsOfStuff[index] = (STUFF*)Pool.GetBlock(); 
            }
        } else {
            if(LoadsOfStuff[index] != null) {
                //printf("[%d] Free\n", --BlockCount);
                Pool.FreeBlock(LoadsOfStuff[index]);
                LoadsOfStuff[index] = null;
            }
        }
    }
    printf("Passed\n");

    printf("Done with MemoryPoolTest\n");
    

}
