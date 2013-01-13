//*******************************************************************************
// ASP Server
//
// @author Doug Frazer
// December 2012
//*******************************************************************************

#include <assert.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

#include "common_include.h"

#include "world.h"
#include "network.h"
#include "keyboard.h"

static const float MIN_FRAME_TIME = 1;

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
int main()
{
    time_t PreviousClock = clock();
    
    Main_Init();
    
    while(true) {
        //printf("Running main update on thread=%u\n", pthread_self());
        clock_t CurrentClock = clock();
        float DeltaTime = ((float)CurrentClock - (float)PreviousClock)/CLOCKS_PER_SEC;
        PreviousClock = CurrentClock;
        
        Main_Update(DeltaTime);
        Main_Draw();
        
        usleep(500);
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
