//
//  network.h
//  aspserver
//
//  Created by Douglas Frazer on 1/2/13.
//  Copyright 2013 Douglas Frazer. All rights reserved.
//

#include <stdint.h>

#ifndef __NETWORK_H__
#define __NETWORK_H__

void Network_Init();
void Network_Update(float DeltaTime);
void Network_Deinit();
void Network_UpdatePosition(uint32_t UserId, uint32_t x, uint32_t y);

#endif
