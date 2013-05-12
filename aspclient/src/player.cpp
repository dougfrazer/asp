//******************************************************************************
// Player
//
// @author Doug Frazer
// (c) May 2013
//******************************************************************************

#include "player.h"
#include "../data/obj.h"
#include "stdio.h"

#include "GL/glut.h"

//******************************************************************************
// Data
//******************************************************************************
static u32 PrimaryPlayerUserId = 0;
static PLAYER* PrimaryPlayer = null;
static PLAYER* PlayerList = null;

//******************************************************************************
// Constructor
//******************************************************************************
PLAYER::PLAYER(u32 _Id) :
    Id(_Id),
    Data(null),
    DataName("/home/doug/asp/aspclient/data/cgc_character_female_basemesh.data")
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
    if(Data == null) {
        Load(DataName);
    }
}
//******************************************************************************
void PLAYER::Draw()
{
    if(Data == null) return;

    glTranslatef(Position.x, Position.y, Position.z);
    
    glColor3f(1.0, 0.0, 0.0);
    for(int i = 0; i < Data->NumVertices; i++) {
        glPushMatrix();
        glTranslatef(Data->Vertices[i].x,
                     Data->Vertices[i].y,
                     Data->Vertices[i].z);
        glutWireSphere(0.1, 1, 1);
        glPopMatrix();
    }
}
//******************************************************************************
inline void pointer_make_absolute(void** x) { *x = (void*)((intptr_t)x + (intptr_t)*x); }
void PLAYER::Load(const char* Filename)
{
    FILE* file = fopen(Filename, "r");
    assert(file != null);
    fseek(file, 0L, SEEK_END);
    size_t filesize = ftell(file);
    fseek(file, 0L, SEEK_SET);
    Data = (ObjData*)malloc(filesize); 
    assert(fread((char*)Data, filesize, 1, file));
    fclose(file);

    pointer_make_absolute((void**)&Data->Vertices);
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
