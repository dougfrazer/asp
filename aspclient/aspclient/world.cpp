//
//  world.cpp
//  aspclient
//
//  Created by Douglas Frazer on 1/3/13.
//  Copyright 2013 Douglas Frazer. All rights reserved.
//

#include <stdio.h>
#include <GL/freeglut.h>

#include "keyboard.h"
#include "world.h"

static void World_Reshape(int width, int height);
static void World_Idle();

// *****************************************************************************
void World_Init()
{
    int argc = 0;
    char *argv = NULL;
    glutInit(&argc, &argv);
    
    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
    glutInitWindowSize(640, 480);
    
    glutCreateWindow("GLUT Program");
    
    glutDisplayFunc(World_Draw);
    glutReshapeFunc(World_Reshape);
    glutIdleFunc(World_Idle);
    glutKeyboardFunc(Keyboard_KeyPressed); 
    glutKeyboardUpFunc(Keyboard_KeyUp);
}
// *****************************************************************************
void World_Update(float DeltaTime)
{
    glutPostRedisplay();
    glutMainLoopEvent();
}
// *****************************************************************************
void World_Draw()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    glBegin(GL_QUADS);
    glVertex2f(-0.75f, -0.75f);
    glVertex2f( 0.75f, -0.75f);
    glVertex2f( 0.75f,  0.75f);
    glVertex2f(-0.75f,  0.75f);
    glEnd();
    
    glutSwapBuffers();
}
// *****************************************************************************
void World_Deinit()
{
    
}
// *****************************************************************************

// *****************************************************************************
static void World_Reshape(int width, int height)
{
    glViewport(0, 0, width, height);
}
// *****************************************************************************
static void World_Idle()
{
}
// ******************************************************************************
