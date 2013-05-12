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
    
    float r = 1.0, g = 0.0, b = 0.0;
    for(int i = 0; i < Data->NumObjects; i++) {
        glColor3f(r,g,b);
        for(int j = 0; j < Data->Objects[i].NumFaces; j++) {
            glBegin(GL_TRIANGLES);
                glVertex3fv((GLfloat*)&Data->Vertices[ Data->Objects[i].Faces[j].v0 ]);
                glVertex3fv((GLfloat*)&Data->Vertices[ Data->Objects[i].Faces[j].v1 ]);
                glVertex3fv((GLfloat*)&Data->Vertices[ Data->Objects[i].Faces[j].v2 ]);
                
                if(Data->Objects[i].Faces[j].v3 != (u16)-1) {
                    glVertex3fv((GLfloat*)&Data->Vertices[ Data->Objects[i].Faces[j].v2 ]);
                    glVertex3fv((GLfloat*)&Data->Vertices[ Data->Objects[i].Faces[j].v3 ]);
                    glVertex3fv((GLfloat*)&Data->Vertices[ Data->Objects[i].Faces[j].v0 ]);
                }
            glEnd();
        }
        r -= clamp(0.1, 0.0, 1.0);
        g += clamp(0.1, 0.0, 1.0);
        b += clamp(0.2, 0.0, 1.0);
    }
}
//******************************************************************************
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
    pointer_make_absolute((void**)&Data->Objects);
    for(int i = 0; i < Data->NumObjects; i++) {
        pointer_make_absolute((void**)&Data->Objects[i].Faces);
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
