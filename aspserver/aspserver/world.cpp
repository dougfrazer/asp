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

#include "asppacket.h"
#include "common_include.h"
#include "world.h"

static uint32_t WorldMap[WORLD_SIZE][WORLD_SIZE];

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
    // TODO: thread concurrency?
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
    for(int i = 0; i < WORLD_SIZE; i++) {
        for(int j = 0; j < WORLD_SIZE; j++) {
            if(WorldMap[i][j] == UserId) {
                printf("[%lu] Player (%d) found at position (%d,%d)\n", (unsigned long)pthread_self(), UserId, i, j);
                return;
            }
        }
    }

    // OK, he doesnt exist, just put him in the first available spot
    for(int i = 0; i < WORLD_SIZE; i++) {
        for(int j = 0; j < WORLD_SIZE; j++) {
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

    for(int i = 0; i < WORLD_SIZE; i++) {
        for(int j = 0; j < WORLD_SIZE; j++) {
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
