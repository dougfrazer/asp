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

#ifndef __ASP_SERVER_WORLD_H__
#define __ASP_SERVER_WORLD_H__

#include "ASPLib.h"
#include "network/packets/direction.h"

class PACKET_STREAM;

void World_Init();
void World_Update();
void World_Deinit();
void World_SetInitialPosition(u32 UserId);
void World_RequestState(PACKET_STREAM* Stream);
void World_HandleMovement(DIRECTION_PACKET_HANDLER::DIRECTION Direction, u32 UserId);

#endif
