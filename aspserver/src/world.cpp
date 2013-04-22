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
#include <stdio.h>
#include <pthread.h>

#include "ASPLib.h"
#include "network/PacketStream.h"
#include "includes/common_include.h"
#include "includes/math.h"
#include "network/packets/direction_ack.h"
#include "world.h"

static u32 WorldMap[WORLD_SIZE][WORLD_SIZE];

//*******************************************************************************
// Forward Declarations
//*******************************************************************************
static bool World_FindPlayer(u32 UserId, int* x, int* y);

//*******************************************************************************
// Public Interface
//*******************************************************************************
void World_Init()
{
    Memset(&WorldMap, (u8)0, sizeof(WorldMap));
}
//*******************************************************************************
void World_Update()
{

}
//*******************************************************************************
void World_Deinit()
{
    Memset(&WorldMap, (u8)0, sizeof(WorldMap));
}
//*******************************************************************************
bool World_SetPosition(u32 x, u32 y, u32 UserId)
{
    int x_curr = 0;
    int y_curr = 0;
    
    // Find Player
    if(World_FindPlayer(UserId, &x_curr, &y_curr)) {
        WorldMap[x_curr][y_curr] = 0;
    } else {
        printf("Got a SetPosition request on a user that doesnt exist in our world (%d)", UserId);
        return false;
    }

    if(WorldMap[x][y] == 0) {
        WorldMap[x][y] = UserId;
        return true;
    }
    return false;
}
//*******************************************************************************
void World_SetInitialPosition(u32 UserId)
{
    // See if he is for some reason already in our world
	int x, y;
	if(World_FindPlayer(UserId, &x,&y)) return;

    // OK, he doesnt exist, just put him in the first available spot
    for(u32 i = 0; i < WORLD_SIZE; i++) {
        for(u32 j = 0; j < WORLD_SIZE; j++) {
            if(WorldMap[i][j] == 0) {
                printf("[%lu] Player (%d) placed at position (%d,%d)\n", (unsigned long)pthread_self(), UserId, i, j);
                WorldMap[i][j] = UserId;
                return;
            }
        }
    }
    assert(false); // our world is full???
}
//*******************************************************************************
void World_RequestState(PACKET_STREAM* Stream)
{
    DIRECTION_ACK_PACKET_HANDLER::DATA Data;

    for(u32 i = 0; i < WORLD_SIZE; i++) {
        for(u32 j = 0; j < WORLD_SIZE; j++) {
            if(WorldMap[i][j] != 0) {
                Data.x = i;
                Data.y = j;
                Data.UserId = WorldMap[i][j];
                Stream->AddPacket(StringHash("DIRECTION_ACK"), sizeof(Data), &Data);
            }
        }
    }
}
//*******************************************************************************
static bool World_FindPlayer(u32 UserId, int* x, int* y)
{
    // TODO: more efficient algorithm
    for(u32 i = 0; i < WORLD_SIZE; i++) {
        for(u32 j = 0; j < WORLD_SIZE; j++) {
            if(WorldMap[i][j] == UserId) {
                *x = i;
                *y = j;
                return true;
            }
        }
    }
    return false;
}
//*******************************************************************************
