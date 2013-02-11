//*******************************************************************************
// ASP Server
// Main Application
// 
//   This will handle setting up the entire application and dispatching the
//   appropriate update/draw calls to the necessary services.
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
#include "world.h"


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
    struct timespec req, res;
    req.tv_sec = 0;
    req.tv_nsec = 500000;

    Main_Init();
    
    // TODO: have a listening thread?
    while(true) {
        clock_t CurrentClock = clock();
        float DeltaTime = ((float)CurrentClock - (float)PreviousClock)/CLOCKS_PER_SEC;
        PreviousClock = CurrentClock;
        
        Main_Update(DeltaTime);
        nanosleep(&req, &res);
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
    World_Init();
}
//*******************************************************************************
static void Main_Update(float DeltaTime)
{
    Network_Update(DeltaTime);
    World_Update();
}
//*******************************************************************************
static void Main_Deinit()
{
    Network_Deinit();
    World_Deinit();
}
//*******************************************************************************
