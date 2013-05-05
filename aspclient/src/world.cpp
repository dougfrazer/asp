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

// TODO: write game-specific math functions in the library
#include <math.h>

#include "includes/common_include.h"
#include "includes/math.h"
#include "ASPLib.h"

#include "keyboard.h"
#include "world.h"
#include "network.h"

static void World_Reshape(int width, int height);
static void World_Idle();
static void World_Rotate(int button, int state,int x, int y);
static void World_PrintText(float, float, void*, char*, float, float, float, float);
static void World_DrawDebugText();
static void World_UpdateCameraRotation();

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
static float CameraAngle = 0.0;
static struct CAMERA_LOCATION {
	float x;
	float y;
	float z;
} CameraLocation = { 0.0, 0.0, 0.0 };

#define PI 3.14159265

// *****************************************************************************
void World_Init()
{
    int argc = 0;
    char *argv = NULL;
    glutInit(&argc, &argv);

    assert(PlayerList == null);
    
    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
    glutInitWindowSize(640,640);
    glutInitWindowPosition(100, 100);
    
    glutCreateWindow("Awesome Program");
    
    glutDisplayFunc(World_Draw);
    glutReshapeFunc(World_Reshape);
    glutIdleFunc(World_Draw);
	glutMouseFunc(World_Rotate);
    glutKeyboardFunc(Keyboard_KeyPressed); 
    glutKeyboardUpFunc(Keyboard_KeyUp);
}
// *****************************************************************************
void World_Update(float DeltaTime)
{
    glutPostRedisplay();
    glutMainLoopEvent();
	World_UpdateCameraRotation();
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
		CameraLocation.x = Player->Pos.x + sin(CameraAngle*PI/180)*10.0;
		CameraLocation.y = Player->Pos.y + 10.0;
		CameraLocation.z = Player->Pos.z + cos(CameraAngle*PI/180)*10.0;
        gluLookAt( CameraLocation.x, CameraLocation.y, CameraLocation.z,
                   Player->Pos.x, Player->Pos.y, Player->Pos.z, 
                   0.0, 1.0, 0.0 );
    } else {
        gluLookAt( 0.0, 10.0, 10.0, 
                   0.0, 0.0, 0.0, 
                   0.0, 1.0, 0.0 );
    }

    // Draw Ground
    glColor3f(0.0, 1.0, 0.0);
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

	World_DrawDebugText();

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
static void World_DrawDebugText()
{
	char buffer[100];
	struct TEXT_POSITION { float x; float y; float z; } TextPosition = { 0, 0, 0 };
	static float TEXT_HEIGHT = 0.25;
   
	PLAYER* Player = PlayerList;
    while(Player != null) {
        if(Network_IsUserLocal(Player->Id)) {
            break;
        }
        Player = Player->Next;
    }

    if(Player == null) {
		return;
	}
	glPushMatrix();
	glTranslatef(Player->Pos.x, Player->Pos.y + 2.5, Player->Pos.z);
	snprintf(buffer, sizeof(buffer), "Camera Angle: %f", CameraAngle);
	World_PrintText(TextPosition.x, TextPosition.y, GLUT_BITMAP_HELVETICA_10, buffer, 1.0, 1.0, 1.0, 0.5);

	TextPosition.y += TEXT_HEIGHT;
	snprintf(buffer, sizeof(buffer), "Camera Position: %f %f %f", CameraLocation.x, CameraLocation.y, CameraLocation.z);
	World_PrintText(TextPosition.x, TextPosition.y, GLUT_BITMAP_HELVETICA_10, buffer, 1.0, 1.0, 1.0, 0.5);

	TextPosition.y += TEXT_HEIGHT;
	snprintf(buffer, sizeof(buffer), "Player Position: %d %d %d", Player->Pos.x, Player->Pos.y, Player->Pos.z);
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

static int prevx = 0;
static int prevy = 0;
static int currx = 0;
static int curry = 0;
static int MouseState = GLUT_UP;
static void World_Rotate(int button, int state, int x, int y)
{
	prevx = currx;
	prevy = curry;
	currx = x;
	curry = y;
	MouseState = state;
}

static void World_UpdateCameraRotation()
{
	switch(MouseState) {
		case GLUT_DOWN:
			CameraAngle = prevx > currx ? CameraAngle + 10 : CameraAngle - 10;
			break;
		default:
			break;
	}
}
