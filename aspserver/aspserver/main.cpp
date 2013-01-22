//*******************************************************************************
// ASP Server
//
// @author Doug Frazer
// December 2012
//*******************************************************************************

#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <time.h>
#include <unistd.h>

#include "common_include.h"

#include "network.h"

static const float MIN_FRAME_TIME = 1;

//*******************************************************************************
// Forward Declarations
//*******************************************************************************
static void Main_Init();
static void Main_Update(float DeltaTime);
static void Main_Deinit();

//*******************************************************************************
// Public Interface
//*******************************************************************************
int main()
{
    time_t PreviousClock = clock();

    Main_Init();
    
    while(true) {
        printf("Running main update on thread=%lu\n", (unsigned long)pthread_self());
        clock_t CurrentClock = clock();
        float DeltaTime = ((float)CurrentClock - (float)PreviousClock)/CLOCKS_PER_SEC;
        PreviousClock = CurrentClock;
        
        Main_Update(DeltaTime);
        
        sleep(MIN_FRAME_TIME);
    }
    
    Main_Deinit();
    
    return EXIT_SUCCESS;
}

//*******************************************************************************
// Private Interface
//*******************************************************************************
static void Main_Init()
{
    Network_Init();
}
//*******************************************************************************
static void Main_Update(float DeltaTime)
{
    Network_Update(DeltaTime);
}
//*******************************************************************************
static void Main_Deinit()
{
    Network_Deinit();
}
//*******************************************************************************
