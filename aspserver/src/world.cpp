//*******************************************************************************
// World
// Define the state of the world
// 
//   This module will maintain the state of the world from the servers point of view.
//   It is responsible for maintaining locations for every client.
//
// @author Doug Frazer
// December 2012
//*******************************************************************************

#include <assert.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>

#include "library/ASPLib.h"
#include "library/network/PacketStream.h"
#include "includes/common_include.h"
#include "library/network/packets/direction_ack.h"
#include "library/network/packets/direction.h"
#include "world.h"

struct PLAYER {
   u32      Id;
   vector4  Pos;
   PLAYER*  Next;
};

static PLAYER* PlayerList = null;

//*******************************************************************************
// Forward Declarations
//*******************************************************************************
static bool World_FindPlayer(u32 UserId, int* x, int* y);
static PLAYER* World_FindPlayer(u32 UserId);

//*******************************************************************************
// Public Interface
//*******************************************************************************
void World_Init()
{
}
//*******************************************************************************
void World_Update()
{

}
//*******************************************************************************
void World_Deinit()
{
}
//*******************************************************************************
void World_HandleMovement(DIRECTION_PACKET_HANDLER::DIRECTION Direction, u32 UserId)
{
   
    PLAYER* Player = World_FindPlayer(UserId);
    if(Player == null) {
        return;
    }

    switch(Direction)
    {
    case DIRECTION_PACKET_HANDLER::NORTH: Player->Pos.z -= 1; break;
    case DIRECTION_PACKET_HANDLER::SOUTH: Player->Pos.z += 1; break;
    case DIRECTION_PACKET_HANDLER::EAST:  Player->Pos.x += 1; break;
    case DIRECTION_PACKET_HANDLER::WEST:  Player->Pos.x -= 1; break;
    }
}
//******************************************************************************
static PLAYER* World_AddUser(u32 UserId)
{
    PLAYER* Player = (PLAYER*)malloc(sizeof(PLAYER));
    assert(Player != null);
    Player->Next = PlayerList;
    Player->Id = UserId;
    PlayerList = Player;
    return Player;
}
//******************************************************************************
void World_SetInitialPosition(u32 UserId)
{
    // See if he is for some reason already in our world
	int x, y;
	if(World_FindPlayer(UserId, &x,&y)) return;

    PLAYER* Player = World_AddUser(UserId);
    if(Player == null) {
        error("somehow failed to add player to linked list");
    }

    Player->Pos.x = 0;
    Player->Pos.y = 0;
    Player->Pos.z = 1;
}
//*******************************************************************************
void World_RequestState(PACKET_STREAM* Stream)
{
    PLAYER* Player = PlayerList;
    DIRECTION_ACK_PACKET_HANDLER::DATA Data;
    
    while(Player != null) {
        Data.x = Player->Pos.x;
        Data.y = Player->Pos.y;
        Data.z = Player->Pos.z;
        Data.UserId = Player->Id;
        Stream->AddPacket(StringHash("DIRECTION_ACK"), sizeof(Data), &Data);
        Player = Player->Next;
    }
}
//*******************************************************************************
static bool World_FindPlayer(u32 UserId, int* x, int* y)
{
    PLAYER* Player = PlayerList;
    while(Player != null) {
        if(Player->Id == UserId) {
            *x = Player->Pos.x;
            *y = Player->Pos.y;
            return true;
        }
        Player = Player->Next;
    }
    return false;
}
//*******************************************************************************
static PLAYER* World_FindPlayer(u32 UserId)
{
    PLAYER* Player = PlayerList;
    while(Player != null) {
        if(Player->Id == UserId) {
            return Player;
        }
        Player = Player->Next;
    }
    return null;
}
//*******************************************************************************
