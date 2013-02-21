//
//  network.h
//  aspclient
//
//  Created by Douglas Frazer on 1/3/13.
//  Copyright 2013 Douglas Frazer. All rights reserved.
//

#include "includes/common_include.h"

#ifndef aspclient_network_h
#define aspclient_network_h

void Network_Init();
void Network_Update(float DeltaTime);
void Network_Deinit();

void Network_SendDirectionPacket(ASP_DIRECTION direction, int magnitude);

#endif
