//******************************************************************************
// Player
//
// @author Doug Frazer
// (c) May 2013
//******************************************************************************

#include "player.h"

#include "GL/glut.h"

//******************************************************************************
// Data
//******************************************************************************
static u32 PrimaryPlayerUserId = 0;
static PLAYER* PrimaryPlayer = null;
static PLAYER* PlayerList = null;

static CONNECTED_NODE TestSkeletonConnectedNodes[] =
{
        { NECK, BACK },
        { BACK, TORSO },
        { BACK, LEFT_SHOULDER },
        { BACK, RIGHT_SHOULDER },
        { LEFT_SHOULDER, LEFT_ELBOW },
        { LEFT_ELBOW, LEFT_WRIST },
        { RIGHT_SHOULDER, RIGHT_ELBOW },
        { RIGHT_ELBOW, RIGHT_WRIST },
        { TORSO, LEFT_THIGH },
        { TORSO, RIGHT_THIGH },
        { LEFT_THIGH, LEFT_KNEE },
        { LEFT_KNEE, LEFT_FOOT },
        { RIGHT_THIGH, RIGHT_KNEE },
        { RIGHT_KNEE, RIGHT_FOOT },
        { MAX_SKELETON_NODES, MAX_SKELETON_NODES },
};

static SKELETON TestSkeleton = {
    { 
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
    },

    &TestSkeletonConnectedNodes[0],
};


//******************************************************************************
// Constructor
//******************************************************************************
PLAYER::PLAYER(u32 _Id) :
    Id(_Id)
{
    // add it to a cyclic list
    if(PlayerList == null) {
        PlayerList       = this;
        PlayerList->Prev = this;
        PlayerList->Next = this;
    } else {
        this->Prev = PlayerList->Prev;
        this->Next = PlayerList->Prev->Next;
        PlayerList->Prev->Next = this;
        PlayerList->Prev = this;
    }
}
//******************************************************************************
PLAYER::~PLAYER()
{
    // TODO: remove from PlayerList
}
//******************************************************************************

//******************************************************************************
// Public Interface
//******************************************************************************
void PLAYER::Update(float DeltaTime)
{
}
//******************************************************************************
void PLAYER::Draw()
{
    glTranslatef(Position.x, Position.y, Position.z);
    
    glColor3f(1.0, 0.0, 0.0);
    for(uint i = 0; i < MAX_SKELETON_NODES; i++) {
        glPushMatrix();
        glTranslatef(TestSkeleton.Nodes[i].posx,
                     TestSkeleton.Nodes[i].posy,
                     TestSkeleton.Nodes[i].posz);
        glutWireSphere(0.1, 5, 5);
        glPopMatrix();
    }

    glColor3f(1.0, 1.0, 1.0);
    for(int i = 0; ; i++) {
        CONNECTED_NODE* c = &TestSkeleton.ConnectedNodes[i];
        if(c->First == MAX_SKELETON_NODES || c->Second == MAX_SKELETON_NODES) break;
        glBegin(GL_LINES);
        glVertex3d(TestSkeleton.Nodes[c->First].posx,
                   TestSkeleton.Nodes[c->First].posy,
                   TestSkeleton.Nodes[c->First].posz);
        glVertex3d(TestSkeleton.Nodes[c->Second].posx,
                   TestSkeleton.Nodes[c->Second].posy,
                   TestSkeleton.Nodes[c->Second].posz);
        glEnd();
    }
}
//******************************************************************************


//******************************************************************************
// Functions that need to be revisited
//******************************************************************************
static PLAYER* Player_AddUser(u32 UserId)
{
    PLAYER* Player = new PLAYER(UserId);
    assert(Player != null);
    Player->Next = PlayerList;
    PlayerList = Player;
    return Player;
}
//******************************************************************************
static PLAYER* Player_FindUser(u32 UserId)
{
    for(PLAYER* p = Player_GetFirst(); p != null; p = Player_GetNext(p)) {
        if(p->Id == UserId) {
            return p;
        }
    }
    return null;
}
//******************************************************************************
void Player_SetPrimaryPlayer(u32 UserId)
{
    PrimaryPlayerUserId = UserId;
}
//******************************************************************************
bool Player_GetPrimaryPlayerPosition(vector4* Position)
{
    if(PrimaryPlayer != null) {
        *Position = PrimaryPlayer->Position;
        return true;
    } else {
        if(PrimaryPlayerUserId != 0) {
            PrimaryPlayer = Player_FindUser(PrimaryPlayerUserId);
        }
    }
    return false;
}
//******************************************************************************
void Player_SetPosition(float x, float y, float z, u32 UserId)
{
    PLAYER* Player = Player_FindUser(UserId);
    if(Player == null) {
        Player = Player_AddUser(UserId);
    }
    assert(Player != null);

    Player->Position.x = x;
    Player->Position.y = y;
    Player->Position.z = z;
}
//******************************************************************************
PLAYER* Player_GetFirst()
{
    return PlayerList;
}
//******************************************************************************
PLAYER* Player_GetNext(PLAYER* Player)
{
    return Player->Next == PlayerList ? null : Player->Next;
}
//******************************************************************************
