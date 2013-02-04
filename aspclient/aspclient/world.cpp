//
//  world.cpp
//  aspclient
//
//  Created by Douglas Frazer on 1/3/13.
//  Copyright 2013 Douglas Frazer. All rights reserved.
//

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <GL/freeglut.h>

#include "common_include.h"

#include "keyboard.h"
#include "world.h"

static void World_Reshape(int width, int height);
static void World_Idle();
static void World_PrintText(float, float, void*, char*, float, float, float, float);

static uint32_t WorldMap[WORLD_SIZE][WORLD_SIZE];

// *****************************************************************************
void World_Init()
{
    int argc = 0;
    char *argv = NULL;
    glutInit(&argc, &argv);

    zero(&WorldMap);
    
    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
    glutInitWindowSize(1024, 768);
    
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
    
    // draw a black background
    glColor3f(0.0f, 0.0f, 0.0f);
    glBegin(GL_QUADS);
    glVertex2f(-1.0f, -1.0f);
    glVertex2f( 1.0f, -1.0f);
    glVertex2f( 1.0f,  1.0f);
    glVertex2f(-1.0f,  1.0f);
    glEnd();


    // draw a bunch of squares
    for(uint32_t i = 0; i < WORLD_SIZE; i++) {
        for(uint32_t j = 0; j < WORLD_SIZE; j++) {
            if(WorldMap[i][j] != 0) {
                glColor3f(0.0f, 1.0f, 0.0f);
            } else {
                glColor3f(1.0f, 0.0f, 0.0f);
            }

            // TODO: this is complete crap, but its all going to get ripped out anyway so
            //       make sure to delete it when an actual design is made
            struct Position {
                float x;
                float y;
            };
            // Add a padding equal to 10% of a square on each edge
            float Offset = (1.8 / ( (float)WORLD_SIZE*2 )) * 0.1;
            float TextPositionOffset = (1.8 / ( (float)WORLD_SIZE) ) * 0.45;
            struct Position BottomLeft, BottomRight, TopLeft, TopRight;
            BottomLeft.x =  LinearInterpolate(i  , 0, WORLD_SIZE, -0.9f, 0.9f) + Offset;
            BottomLeft.y =  LinearInterpolate(j  , 0, WORLD_SIZE, -0.9f, 0.9f) + Offset;
            BottomRight.x = LinearInterpolate(i+1, 0, WORLD_SIZE, -0.9f, 0.9f) - Offset;
            BottomRight.y = LinearInterpolate(j  , 0, WORLD_SIZE, -0.9f, 0.9f) + Offset;
            TopRight.x =    LinearInterpolate(i+1, 0, WORLD_SIZE, -0.9f, 0.9f) - Offset;
            TopRight.y =    LinearInterpolate(j+1, 0, WORLD_SIZE, -0.9f, 0.9f) - Offset;
            TopLeft.x =     LinearInterpolate(i  , 0, WORLD_SIZE, -0.9f, 0.9f) + Offset;
            TopLeft.y =     LinearInterpolate(j+1, 0, WORLD_SIZE, -0.9f, 0.9f) - Offset;
            glBegin(GL_QUADS);
            glVertex2f( BottomLeft.x, BottomLeft.y );
            glVertex2f( BottomRight.x, BottomRight.y );
            glVertex2f( TopRight.x, TopRight.y );
            glVertex2f( TopLeft.x, TopLeft.y );
            glEnd();
            if(WorldMap[i][j] != 0) {
                char buffer[10];
                snprintf(buffer, sizeof(buffer), "%d", WorldMap[i][j]);
                World_PrintText(BottomLeft.x + TextPositionOffset, BottomLeft.y + TextPositionOffset, GLUT_BITMAP_HELVETICA_18, buffer, 1.0f, 1.0f, 1.0f, 0.5f);
            }
        }
    }
    
    glutSwapBuffers();
}
// *****************************************************************************
void World_Deinit()
{
    
}
// *****************************************************************************
static uint32_t* World_FindUser(uint32_t UserId)
{
    for(uint32_t i = 0; i < WORLD_SIZE; i++) {
        for(uint32_t j = 0; j < WORLD_SIZE; j++) {
            if(WorldMap[i][j] == UserId) {
                return &WorldMap[i][j];
            }
        }
    }
    return NULL;
}
// ****************************************************************************
void World_SetPosition(uint32_t x, uint32_t y, uint32_t UserId)
{
    // set his previous location to zero
    uint32_t* User = World_FindUser(UserId);
    if(User != NULL) *User = 0;

    // write his current location
    WorldMap[x][y] = UserId;
}
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
static void World_PrintText(float x, float y, void* font, char* text, float r, float g, float b, float a)
{
    if(text == NULL || strlen(text) == 0) return;
    bool blending = false;
    if(glIsEnabled(GL_BLEND)) blending = true;
    glEnable(GL_BLEND);
    glColor4f(r,g,b,a);
    glRasterPos2f(x,y);
    while(*text) {
        glutBitmapCharacter(font, *text);
        text++;
    }
    if(!blending) glDisable(GL_BLEND);
}
// ******************************************************************************
