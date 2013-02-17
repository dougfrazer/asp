//
//  world.h
//  aspclient
//
//  Created by Douglas Frazer on 1/3/13.
//  Copyright 2013 Douglas Frazer. All rights reserved.
//

#include <stdint.h>

#ifndef aspclient_world_h
#define aspclient_world_h

void World_Init();
void World_Update(float DeltaTime);
void World_Draw();
void World_Deinit();
void World_SetPosition(uint32_t x, uint32_t y, uint32_t UserId);
bool World_AttemptMovement(ASP_DIRECTION Direction, uint32_t Magnitude, uint32_t UserId, uint32_t* x, uint32_t *y);

#endif
