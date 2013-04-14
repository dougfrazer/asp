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
#include <time.h>
#include <math.h>
#include <unistd.h>

#include "includes/common_include.h"

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
#define SET_UPDATE_TIME 1.0/60.0
int main()
{
    struct timespec SleepReq, SleepRes;

    SleepReq.tv_sec = 0;
    SleepReq.tv_nsec = (SET_UPDATE_TIME)*(1000)*(1000)*(1000);
    
    Main_Init();
    
    // TODO: threads
    while(true) {
        Main_Update(SET_UPDATE_TIME);
        nanosleep(&SleepReq, &SleepRes);
    }
    
    Main_Deinit();
    
    return EXIT_SUCCESS;
}
//*******************************************************************************

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
