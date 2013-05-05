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
#include "library/ASPLib.h"

#include "keyboard.h"
#include "world.h"
#include "network.h"
#include "camera.h"

static void World_PrintText(float, float, void*, char*, float, float, float, float);
static void World_DrawDebugText();

struct PLAYER {
    u32      Id;
    vector4  Pos;
    PLAYER*  Next;
};

static PLAYER* PlayerList = null;

// *****************************************************************************
void World_Init()
{
    assert(PlayerList == null);
}
// *****************************************************************************
void World_Update(float DeltaTime)
{
    glutPostRedisplay();
    glutMainLoopEvent();
}
// *****************************************************************************
enum SKELETON_NODES
{
    NECK,
	BACK,

    LEFT_SHOULDER,
    LEFT_ELBOW,
    LEFT_WRIST,
    
    RIGHT_SHOULDER,
    RIGHT_ELBOW,
    RIGHT_WRIST,

    TORSO,

    LEFT_THIGH,
    LEFT_KNEE,
    LEFT_FOOT,

    RIGHT_THIGH,
    RIGHT_KNEE,
    RIGHT_FOOT,

    MAX_SKELETON_NODES,
};

// TODO: quaternions?
struct ORIENTATION
{
    float rotx;
    float roty;
    float rotz;
    
    float posx;
    float posy;
    float posz;
};

ORIENTATION TestSkeleton[MAX_SKELETON_NODES] = {
	{ 0.0, 0.0, 0.0, 0.0, 2.0, 0.0 },     // NECK
	{ 0.0, 0.0, 0.0, 0.0, 1.5, 0.0 },     // BACK

	{ 0.0, 0.0, 0.0, 0.25, 1.75, 0.0 },  // LEFT SHOULDER
	{ 0.0, 0.0, 0.0, 0.35, 1.5,  0.0 },  // LEFT ELBOW
	{ 0.0, 0.0, 0.0, 0.25, 1.25, 0.2 },  // LEFT WRIST

	{ 0.0, 0.0, 0.0, -0.25, 1.75, 0.0 },  // RIGHT SHOULDER
	{ 0.0, 0.0, 0.0, -0.35, 1.5,  0.0 },  // RIGHT ELBOW
	{ 0.0, 0.0, 0.0, -0.25, 1.25, 0.2 },  // RIGHT WRIST

	{ 0.0, 0.0, 0.0, 0.0, 1.0, 0.0 },    // TORSO

	{ 0.0, 0.0, 0.0, 0.25, 0.9,  0.0 },   // LEFT THIGH
	{ 0.0, 0.0, 0.0, 0.25, 0.4,  0.1 },   // LEFT KNEE
	{ 0.0, 0.0, 0.0, 0.25, 0.01, 0.0 },  // LEFT FOOT

	{ 0.0, 0.0, 0.0, -0.25, 0.9,  0.0 },   // RIGHT THIGH
	{ 0.0, 0.0, 0.0, -0.25, 0.4,  0.1 },   // RIGHT KNEE
	{ 0.0, 0.0, 0.0, -0.25, 0.01, 0.0 },  // RIGHT FOOT
};

void World_DrawPlayer(PLAYER* Player)
{
    glTranslatef(Player->Pos.x, Player->Pos.y, Player->Pos.z);
	
    glColor3f(1.0, 0.0, 0.0);
	for(uint i = 0; i < MAX_SKELETON_NODES; i++) {
		glPushMatrix();
		glTranslatef(TestSkeleton[i].posx, TestSkeleton[i].posy, TestSkeleton[i].posz);
		glutSolidSphere(0.1, 20, 20);
		glPopMatrix();
	}
}
// *****************************************************************************
bool World_GetPlayerPosition(vector4* Position)
{
    PLAYER* Player = PlayerList;
    while(Player != null) {
        if(Network_IsUserLocal(Player->Id)) {
            break;
        }
        Player = Player->Next;
    }
	if(Player != null) {
		Position->x = Player->Pos.x;
		Position->y = Player->Pos.y;
		Position->z = Player->Pos.z;
		return true;
	}
	return false;
}
// *****************************************************************************
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

    // Draw each Player
    PLAYER* Player = PlayerList;
    while(Player != null) {
        glPushMatrix();
        World_DrawPlayer(Player);
        glPopMatrix();
        Player = Player->Next;
    }

	World_DrawDebugText();
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
void World_SetPosition(float x, float y, float z, u32 UserId)
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
// ******************************************************************************
static void World_DrawDebugText()
{
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
