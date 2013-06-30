#include "MemoryPool.h"
#include "stdio.h"

#include "stdlib.h"
#include "time.h"
#include "string.h"

int main()
{
    printf("Starting MemoryPoolTest\n");
    struct STUFF {
        int one;
        u64 two;
        u8  three;
    };
	static const uint NUM_STUFF = 1000000;
	STUFF** LoadsOfStuff        = (STUFF**)malloc(NUM_STUFF*sizeof(STUFF));;
	int     BlockCount          = 0;
	STUFF*  Blah                = null;
    char    Buffer[100];
    srand(time(null));
	
    snprintf( Buffer, 100, "%-100s", "Starting Test 1: Get a block..." );
	printf( "%s", Buffer );
	{
		MEMORY_POOL Pool(sizeof(STUFF), 1024);
		Blah = (STUFF*)Pool.GetBlock();
		Blah->one = 5;
		Blah->two = 17;
		Blah->three = 123;
		printf("Passed\n");
	}

    snprintf( Buffer, 100, "%-100s", "Starting test 2: Free a block..." );
	printf( "%s", Buffer );
	{
		MEMORY_POOL Pool(sizeof(STUFF), 1024);
		Blah = (STUFF*)Pool.GetBlock();
		Pool.FreeBlock(Blah);
		printf("Passed\n");
	}

	snprintf( Buffer, 100, "%-100s", "Starting test 3: get, free, get, expect same block..." );
	printf( "%s", Buffer );
	{
		MEMORY_POOL Pool(sizeof(STUFF), 1024);
		Blah = (STUFF*)Pool.GetBlock();
		STUFF* Temp = Blah;
		Pool.FreeBlock(Blah);
		Blah = (STUFF*)Pool.GetBlock();
		Pool.FreeBlock(Blah);
		printf("%s\n", Temp == Blah ? "Passed" : "Failed");
	}

	snprintf( Buffer, 100, "%-100s", "Starting test 4: free a block twice in a row..." );
	printf( "%s", Buffer );
	{
		MEMORY_POOL Pool(sizeof(STUFF), 1024);
		Blah = (STUFF*)Pool.GetBlock();
		Pool.FreeBlock(Blah);
		Pool.FreeBlock(Blah);
		printf("Passed\n");
	}
   
    char TempBuffer[100];
    snprintf( TempBuffer, 100, "Starting test 5: randomly create and free %d times...", NUM_STUFF*10 );
	snprintf( Buffer, 100, "%-100s", TempBuffer );
	printf( "%s", Buffer );
	{
		MEMORY_POOL Pool(sizeof(STUFF), 1024);
		memset( &LoadsOfStuff[0], '\0', NUM_STUFF*sizeof(STUFF) );
		for(int i = 0; i < NUM_STUFF * 10; i++) {
			int index = rand()%NUM_STUFF;
			if(LoadsOfStuff[index] == null) {
				LoadsOfStuff[index] = (STUFF*)Pool.GetBlock(); 
			} else {
				Pool.FreeBlock(LoadsOfStuff[index]);
				LoadsOfStuff[index] = null;
			}
		}
		printf("Passed\n");
	}
	
	snprintf( Buffer, 100, "%-100s", "Starting test 6: allocate a ton of stuff..." );
	printf( "%s", Buffer );
	{
		MEMORY_POOL Pool(sizeof(STUFF), 1024);
		memset( &LoadsOfStuff[0], '\0', sizeof(STUFF)*NUM_STUFF );
		for(int i = 0; i < NUM_STUFF; i++) {
			LoadsOfStuff[i] = (STUFF*)Pool.GetBlock();
			LoadsOfStuff[i]->one = -i;
			LoadsOfStuff[i]->two = i;
			LoadsOfStuff[i]->three = i & 0xFF;
		}
		for(int i = 0; i < NUM_STUFF; i++) {
			Pool.FreeBlock(LoadsOfStuff[i]);
		}
		printf("Passed\n");
	}

	snprintf( Buffer, 100, "%-100s", "Starting test 7: allocate a bunch of stuff and make sure we can store the same values..." );
	printf( "%s", Buffer );
	{
		MEMORY_POOL Pool(sizeof(STUFF), 1024);
		static const uint NUM_DATA = 1000;
		STUFF* Data[NUM_DATA];
		memset( &Data[0], '\0', sizeof(Data) );
		bool Failed = false;
		for(int i = 0; i < NUM_DATA; i++) {
			Data[i] = (STUFF*)Pool.GetBlock();
			Data[i]->one = i;
			Data[i]->two = i;
			Data[i]->three = i & 0xFF;
		}
		for(int i = 0; i < NUM_DATA; i++) {
			if(Data[i]->one != i || Data[i]->two != i || Data[i]->three != ( i & 0xFF ) ) {
				Failed = true;
			}
		}
		printf("%s\n", Failed ? "Failed" : "Passed");
	}

	

    printf("Done with MemoryPoolTest\n");
    
	free(LoadsOfStuff);
}
