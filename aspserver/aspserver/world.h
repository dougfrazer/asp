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

#include <stdint.h>
#include "common_include.h"

#ifndef __ASP_SERVER_WORLD_H__
#define __ASP_SERVER_WORLD_H__

void World_Init();
void World_Update();
void World_Deinit();
bool World_SetPosition(uint32_t x, uint32_t y, uint32_t UserId);
void World_SetInitialPosition(uint32_t UserId);
void World_RequestState(char* Buffer, size_t* Size);

#endif
