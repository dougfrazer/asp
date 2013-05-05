//******************************************************************************
// Direction Packet
//
// (c) April 2013
// @author Doug Frazer
//******************************************************************************

#include "library/network/packets/direction.h"

#include "../client.h"
#include "../world.h"

#include <cstdio>

static DIRECTION_PACKET_HANDLER DirectionHander;

void DIRECTION_PACKET_HANDLER::Recieve(void* Buffer, void* Context)
{
    DATA* Data = (DATA*)Buffer;
    CLIENT* Client = (CLIENT*)Context;
    if(Data == null || Client == null) {
        error("Got a null pointer in direction handler\n");
        return;
    }

    printf("Got a move command from userid=%d for userid=%d\n", Client->UserId, Data->UserId);

    World_HandleMovement(Data->Direction, Client->UserId);
}
