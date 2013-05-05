//
//  world.h
//  aspclient
//
//  Created by Douglas Frazer on 1/3/13.
//  Copyright 2013 Douglas Frazer. All rights reserved.
//

#ifndef __WORLD_H__
#define __WORLD_H__

#include "library/ASPLib.h"

void World_Init();
void World_Update(float DeltaTime);
void World_Draw();
void World_Deinit();
void World_SetPosition(float x, float y, float z, u32 UserId);
bool World_GetPlayerPosition(vector4* Position);

#endif
