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

#include "includes/asppacket.h"
#include "includes/common_include.h"
#include "world.h"

static uint32_t WorldMap[WORLD_SIZE][WORLD_SIZE];

//*******************************************************************************
// Forward Declarations
//*******************************************************************************
static bool World_FindPlayer(uint32_t UserId, int* x, int* y);

//*******************************************************************************
// Public Interface
//*******************************************************************************
void World_Init()
{
    zero(&WorldMap);
}
//*******************************************************************************
void World_Update()
{

}
//*******************************************************************************
void World_Deinit()
{
    zero(&WorldMap);
}
//*******************************************************************************
bool World_SetPosition(uint32_t x, uint32_t y, uint32_t UserId)
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
void World_SetInitialPosition(uint32_t UserId)
{
    // See if he is for some reason already in our world
    for(uint32_t i = 0; i < WORLD_SIZE; i++) {
        for(uint32_t j = 0; j < WORLD_SIZE; j++) {
            if(WorldMap[i][j] == UserId) {
                printf("[%lu] Player (%d) found at position (%d,%d)\n", (unsigned long)pthread_self(), UserId, i, j);
                return;
            }
        }
    }

    // OK, he doesnt exist, just put him in the first available spot
    for(uint32_t i = 0; i < WORLD_SIZE; i++) {
        for(uint32_t j = 0; j < WORLD_SIZE; j++) {
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
void World_RequestState(char* Buffer, size_t* Size)
{
    ASP_HEADER Header;
    ASP_DIRECTION_ACK_PACKET Data;
    Header.Type = DIRECTION_ACK;
    Header.Length = sizeof(ASP_DIRECTION_ACK_PACKET);

    for(uint32_t i = 0; i < WORLD_SIZE; i++) {
        for(uint32_t j = 0; j < WORLD_SIZE; j++) {
            if(WorldMap[i][j] != 0) {
                Data.x = i;
                Data.y = j;
                Data.UserId = WorldMap[i][j];
                memcpy(Buffer + *Size, &Header, sizeof(ASP_HEADER));
                *Size += sizeof(ASP_HEADER);
                memcpy(Buffer + *Size, &Data, sizeof(ASP_DIRECTION_ACK_PACKET));
                *Size += sizeof(ASP_DIRECTION_ACK_PACKET);
            }
        }
    }
}
//*******************************************************************************
static bool World_FindPlayer(uint32_t UserId, int* x, int* y)
{
    // TODO: more efficient algorithm
    for(uint32_t i = 0; i < WORLD_SIZE; i++) {
        for(uint32_t j = 0; j < WORLD_SIZE; j++) {
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
