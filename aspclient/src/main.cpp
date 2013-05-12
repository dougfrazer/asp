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

#include <GL/freeglut.h>
#include "GL/glut.h"

#include "includes/common_include.h"

#include "world.h"
#include "network.h"
#include "keyboard.h"
#include "camera.h"
#include "actor.h"

//*******************************************************************************
// Forward Declarations
//*******************************************************************************
static void Main_Init();
static void Main_InitGlut();
static void Main_Update(float DeltaTime);
static void Main_Draw();
static void Main_Deinit();
static void Main_Reshape(int width, int height);

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
    Main_InitGlut();
    Keyboard_Init();
    Network_Init();
    World_Init();
    Camera_Init();
}
//*******************************************************************************
static void Main_Update(float DeltaTime)
{
    Keyboard_Update(DeltaTime);
    Network_Update(DeltaTime);
    Camera_Update();
    World_Update(DeltaTime);
    Actor_Update(DeltaTime);
    
    glutPostRedisplay();
    glutMainLoopEvent();
}
//*******************************************************************************
static void Main_Draw()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    Camera_Draw();
    World_Draw();
    Actor_Draw();

    glutSwapBuffers();
}
//*******************************************************************************
static void Main_Deinit()
{
    Keyboard_Deinit();
    Network_Deinit();
    World_Deinit();
}
//*******************************************************************************
static void Main_InitGlut()
{
    int argc = 0;
    char *argv = NULL;
    glutInit(&argc, &argv);
    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
    glutInitWindowSize(640,640);
    glutInitWindowPosition(100, 100);
    
    glutCreateWindow("Awesome Program");
    
    glutDisplayFunc(Main_Draw);
    glutReshapeFunc(Main_Reshape);
    glutIdleFunc(null);
    glutMouseFunc(Camera_HandleMousePressed);
    glutKeyboardFunc(Keyboard_KeyPressed); 
    glutKeyboardUpFunc(Keyboard_KeyUp);
}
//*******************************************************************************
static void Main_Reshape(int width, int height)
{
    if(height == 0) height = 1;
    float ratio = width * 1.0 / height;
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glViewport(0, 0, width, height);
    gluPerspective(45.0f, ratio, 0.1f, 100.0f);
    glMatrixMode(GL_MODELVIEW);
}
