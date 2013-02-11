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

#include "common_include.h"
#include "world.h"

struct Position {
    int x;
    int y;
};
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
void World_SetPosition(ASP_DIRECTION Direction, uint32_t Magnitude, uint32_t UserId, uint32_t *x_out, uint32_t *y_out)
{
    // TODO: thread concurrency?
    // TODO: heavy lifting should be done by the CLIENT not the server... move all this to the client and send X and Y not direction and magnitude
    Position CurrentPosition;
    bool PlayerFound = false;
    for(int i = 0; i < WORLD_SIZE; i++) {
        for(int j = 0; j < WORLD_SIZE; j++) {
            if(WorldMap[i][j] == UserId) {
                CurrentPosition.x = i;
                CurrentPosition.y = j;
                PlayerFound = true;
            }
        }
    }
    assert(PlayerFound); // he doesnt exist? wtf?
    
    Position Destination;
    switch(Direction)
    {
    case NORTH:
        {
            Destination.x = CurrentPosition.x;
            Destination.y = clamp(CurrentPosition.y + Magnitude, 0, WORLD_SIZE - 1);
        } break;
    case SOUTH:
        {
            Destination.x = CurrentPosition.x;
            Destination.y = clamp(CurrentPosition.y - Magnitude, 0, WORLD_SIZE - 1);
        } break;
    case EAST:
        {
            Destination.x = clamp(CurrentPosition.x + Magnitude, 0, WORLD_SIZE - 1);
            Destination.y = CurrentPosition.y;
        } break;
    case WEST:
        {
            Destination.x = clamp(CurrentPosition.x - Magnitude, 0, WORLD_SIZE - 1);
            Destination.y = CurrentPosition.y;
        } break;
    }
    if(WorldMap[Destination.x][Destination.y] == 0 ) {
        // noone is there
        WorldMap[Destination.x][Destination.y] = UserId;
        WorldMap[CurrentPosition.x][CurrentPosition.y] = 0;
        *x_out = Destination.x;
        *y_out = Destination.y;
    } else if(WorldMap[Destination.x][Destination.y] == UserId) {
        // this user is currently there (probably a bug)
        *x_out = Destination.x;
        *y_out = Destination.y;
    } else {
        // someone else is there, cant move there
        *x_out = CurrentPosition.x;
        *y_out = CurrentPosition.y;
    }
}
//*******************************************************************************
void World_SetInitialPosition(uint32_t UserId, uint32_t *x, uint32_t *y)
{
    // See if he is for some reason already in our world
    for(int i = 0; i < WORLD_SIZE; i++) {
        for(int j = 0; j < WORLD_SIZE; j++) {
            if(WorldMap[i][j] == UserId) {
                printf("Player (%d) found at position (%d,%d)\n", UserId, i, j);
                *x = i;
                *y = j;
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
                *x = i;
                *y = j;
                return;
            }
        }
    }
    assert(false); // our world is full???
}
