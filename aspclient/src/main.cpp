//*******************************************************************************
// ASP Client
// Main Applicattion
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
#include <unistd.h>
#include <pthread.h>

#include "includes/common_include.h"

#include "world.h"
#include "network.h"
#include "keyboard.h"
#include "camera.h"

//*******************************************************************************
// Forward Declarations
//*******************************************************************************
static void Main_Init();
static void Main_Update(float DeltaTime);
static void Main_Draw();
static void Main_Deinit();

//*******************************************************************************
// Public Interface
//*******************************************************************************
#define SET_UPDATE_TIME 1/20.0
int main()
{
    time_t PreviousClock = clock();
    
    struct timespec SleepReq, SleepRes;
    
    SleepReq.tv_sec = 0;
    SleepReq.tv_nsec = (SET_UPDATE_TIME)*(1000)*(1000)*(1000);
    
    Main_Init();
    
    while(true) {
        //printf("Running main update on thread=%lu\n", (unsigned long)pthread_self());
        clock_t CurrentClock = clock();
        float DeltaTime = ((float)CurrentClock - (float)PreviousClock)/CLOCKS_PER_SEC;
        PreviousClock = CurrentClock;
        
        Main_Update(DeltaTime);
        Main_Draw();
        nanosleep(&SleepReq, &SleepRes);
    }
    
    Main_Deinit();
    
    return EXIT_SUCCESS;
}

//*******************************************************************************
// Private Interface
//*******************************************************************************
static void Main_Init()
{
    Keyboard_Init();
    Network_Init();
    World_Init();
}
//*******************************************************************************
static void Main_Update(float DeltaTime)
{
    Keyboard_Update(DeltaTime);
    Network_Update(DeltaTime);
    World_Update(DeltaTime);
	Camera_Update();
}
//*******************************************************************************
static void Main_Draw()
{
    World_Draw();
}
//*******************************************************************************
static void Main_Deinit()
{
    Keyboard_Deinit();
    Network_Deinit();
    World_Deinit();
}
//*******************************************************************************
