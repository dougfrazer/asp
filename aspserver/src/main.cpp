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
#define SET_FRAME_TIME 1.0/60.0
int main()
{
    struct timespec SleepReq, SleepRes;
	//struct timespec DeltaPrevious, DeltaCurrent;

    SleepReq.tv_sec = 0;
    SleepReq.tv_nsec = (SET_FRAME_TIME)*(1000)*(1000)*(1000);

	//clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &DeltaPrevious);
    Main_Init();
    
    // TODO: have a listening thread?
    while(true) {
		//clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &DeltaCurrent);
        //DeltaPrevious = DeltaCurrent;
        
        Main_Update(SET_FRAME_TIME);
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
