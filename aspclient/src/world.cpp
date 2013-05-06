//******************************************************************************
//  world.cpp
//
//  @author Doug Frazer 
//  (c) January 2013
//******************************************************************************

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <GL/glut.h>

#include "includes/common_include.h"
#include "includes/math.h"
#include "library/ASPLib.h"

#include "keyboard.h"
#include "world.h"
#include "network.h"
#include "camera.h"

static void World_PrintText(float, float, void*, char*, float, float, float, float);
static void World_DrawDebugText();

//*****************************************************************************
void World_Init()
{
}
//*****************************************************************************
void World_Update(float DeltaTime)
{
}
//*****************************************************************************
void World_Draw()
{
    // Draw Ground
    glColor3f(0.0, 1.0, 0.0);
    glBegin(GL_QUADS);
        glVertex3f(-5.0, 0.0, -5.0);
        glVertex3f(-5.0, 0.0,  5.0);
        glVertex3f( 5.0, 0.0,  5.0);
        glVertex3f( 5.0, 0.0, -5.0);
    glEnd();

    World_DrawDebugText();
}
//*****************************************************************************
void World_Deinit()
{
    
}
//*****************************************************************************
static void World_DrawDebugText()
{
/*
    char buffer[100];
    struct TEXT_POSITION { float x; float y; float z; } TextPosition = { 0, 0, 0 };
    static float TEXT_HEIGHT = 0.25;
   
    vector4 PlayerPosition;
    vector4 CameraLocation;
    float CameraAngle;

    if(!World_GetPlayerPosition(&PlayerPosition)) {
        return;
    }
    CameraLocation = Camera_GetLocation();
    CameraAngle = Camera_GetAngle();
    
    glPushMatrix();
    glTranslatef(PlayerPosition.x, PlayerPosition.y + 2.5, PlayerPosition.z);
    snprintf(buffer, sizeof(buffer), "Camera Angle: %f", CameraAngle);
    World_PrintText(TextPosition.x, TextPosition.y, GLUT_BITMAP_HELVETICA_10, buffer, 1.0, 1.0, 1.0, 0.5);

    TextPosition.y += TEXT_HEIGHT;
    snprintf(buffer, sizeof(buffer), "Camera Position: %f %f %f", CameraLocation.x, CameraLocation.y, CameraLocation.z);
    World_PrintText(TextPosition.x, TextPosition.y, GLUT_BITMAP_HELVETICA_10, buffer, 1.0, 1.0, 1.0, 0.5);

    TextPosition.y += TEXT_HEIGHT;
    snprintf(buffer, sizeof(buffer), "Player Position: %d %d %d", (int)PlayerPosition.x, (int)PlayerPosition.y, (int)PlayerPosition.z);
    World_PrintText(TextPosition.x, TextPosition.y, GLUT_BITMAP_HELVETICA_10, buffer, 1.0, 1.0, 1.0, 0.5);

    glPopMatrix();
*/
}
//******************************************************************************
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
//******************************************************************************
