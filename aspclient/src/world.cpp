//******************************************************************************
//  world.cpp
//
//  @author Doug Frazer 
//  (c) January 2013
//******************************************************************************

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <GL/freeglut.h>
#include <GL/glut.h>

#include "includes/common_include.h"
#include "includes/math.h"
#include "ASPLib.h"

#include "keyboard.h"
#include "world.h"
#include "network.h"

static void World_Reshape(int width, int height);
static void World_Idle();
static void World_PrintText(float, float, void*, char*, float, float, float, float);

struct POSITION {
    int x;
    int y;
    int z;
};

struct PLAYER {
    u32      Id;
    POSITION Pos;
    PLAYER*  Next;
};

static PLAYER* PlayerList = null;

// *****************************************************************************
void World_Init()
{
    int argc = 0;
    char *argv = NULL;
    glutInit(&argc, &argv);

    assert(PlayerList == null);
    
    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
    glutInitWindowSize(320,320);
    glutInitWindowPosition(100, 100);
    
    glutCreateWindow("Awesome Program");
    
    glutDisplayFunc(World_Draw);
    glutReshapeFunc(World_Reshape);
    glutIdleFunc(World_Draw);
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
void World_DrawPlayer(PLAYER* Player)
{
    glColor3f(1.0f, 0.0f, 0.0f);
    glTranslatef(Player->Pos.x, Player->Pos.y, Player->Pos.z);
    glutSolidSphere(0.75f, 20, 20);
}
// *****************************************************************************

float angle = 0.0f;

void World_Draw()
{
    PLAYER* Player;

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Reset Transformations
    glLoadIdentity();

    // Set Camera
    // Find local player and look at him
    Player = PlayerList;
    while(Player != null) {
        if(Network_IsUserLocal(Player->Id)) {
            break;
        }
        Player = Player->Next;
    }
    if(Player != null) {
        gluLookAt( Player->Pos.x, Player->Pos.y + 10.0, Player->Pos.z + 10.0,
                   Player->Pos.x, Player->Pos.y, Player->Pos.z, 
                   0.0f, 1.0f, 0.0f );
    } else {
        gluLookAt( 0.0f, 10.0f, 10.0f, 
                   0.0f, 0.0f, 0.0f, 
                   0.0f, 1.0f, 0.0f );
    }

    // Draw Ground
    glColor3f(0.0f, 1.0f, 0.0f);
    glBegin(GL_QUADS);
        glVertex3f(-5.0, 0.0, -5.0);
        glVertex3f(-5.0, 0.0,  5.0);
        glVertex3f( 5.0, 0.0,  5.0);
        glVertex3f( 5.0, 0.0, -5.0);
    glEnd();

    // Draw each Player
    Player = PlayerList;
    while(Player != null) {
        glPushMatrix();
        World_DrawPlayer(Player);
        glPopMatrix();
        Player = Player->Next;
    }

    glutSwapBuffers();
}
// *****************************************************************************
void World_Deinit()
{
    
}
// *****************************************************************************
static PLAYER* World_AddUser(u32 UserId)
{
    PLAYER* Player = (PLAYER*)malloc(sizeof(PLAYER));
    assert(Player != null);
    Player->Next = PlayerList;
    Player->Id = UserId;
    PlayerList = Player;
    return Player;
}
// *****************************************************************************
static PLAYER* World_FindUser(u32 UserId)
{
    PLAYER* p = PlayerList;
    while(p != null) {
        if(p->Id == UserId) {
            return p;
        }
        p = p->Next;
    }
    return null;
}
// ****************************************************************************
void World_SetPosition(u32 x, u32 y, u32 z, u32 UserId)
{
    PLAYER* Player = World_FindUser(UserId);
    if(Player == null) {
        Player = World_AddUser(UserId);
    }
    assert(Player != null);

    Player->Pos.x = x;
    Player->Pos.y = y;
    Player->Pos.z = z;
}
// *****************************************************************************
static void World_Reshape(int width, int height)
{
    if(height == 0) height = 1;
    float ratio = width * 1.0 / height;
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glViewport(0, 0, width, height);
    gluPerspective(45.0f, ratio, 0.1f, 100.0f);
    glMatrixMode(GL_MODELVIEW);
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

