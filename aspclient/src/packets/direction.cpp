//******************************************************************************
// Direction Packet
//
// (c) April 2013
// @author Doug Frazer
//******************************************************************************

#include "packets/direction.h"

#include "world.h"

void DIRECTION_PACKET_HANDLER::Recieve(void* Buffer, void* Context)
{
    DATA* Data = (DATA*)Buffer;
    if(Data == null) {
        error("Got a null pointer in direction handler\n");
        return;
    }

//    printf("Recieved a DirectionAck packet [User=%d | x=%d | y=%d]\n", Data->UserId, Data->x, Data->y);
    World_SetPosition( Data->x, Data->y, Data->UserId );
}
