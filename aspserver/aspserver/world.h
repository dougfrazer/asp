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
void World_SetPosition(ASP_DIRECTION Direction, uint32_t Magnitude, uint32_t UserId, uint32_t* x_out, uint32_t* y_out);
void World_SetInitialPosition(uint32_t UserId, uint32_t *x, uint32_t *y);

#endif
