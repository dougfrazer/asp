//******************************************************************************
// Direction Packet
//
// (c) April 2013
// @author Doug Frazer
//******************************************************************************

#include "library/network/packets/direction_ack.h"

#include "../world.h"

#include "stdio.h"

static DIRECTION_ACK_PACKET_HANDLER DirectionAckHandler;

void DIRECTION_ACK_PACKET_HANDLER::Recieve(void* Buffer, void* Context)
{
    DATA* Data = (DATA*)Buffer;
    if(Data == null) {
        error("Got a null pointer in direction handler\n");
        return;
    }

    // printf("Recieved a DirectionAck packet [User=%d | x=%f | y=%f | z=%f]\n",
    //        Data->UserId, Data->x, Data->y, Data->z);
    World_SetPosition( Data->x, Data->y, Data->z, Data->UserId );
}
