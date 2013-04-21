//
//  world.h
//  aspclient
//
//  Created by Douglas Frazer on 1/3/13.
//  Copyright 2013 Douglas Frazer. All rights reserved.
//

#ifndef __WORLD_H__
#define __WORLD_H__

#include "ASPLib.h"
#include "includes/common_include.h"

void World_Init();
void World_Update(float DeltaTime);
void World_Draw();
void World_Deinit();
void World_SetPosition(u32 x, u32 y, u32 UserId);
bool World_AttemptMovement(ASP_DIRECTION Direction, u32 Magnitude, u32 UserId, u32* x, u32 *y);

#endif
