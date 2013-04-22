//******************************************************************************
// Direction Ack Packet Implementation
//
// (c) April 2013
// @author Doug Frazer
//******************************************************************************

#include "network/packets/direction_ack.h"

static DIRECTION_ACK_PACKET_HANDLER DirectionAckHandler;

void DIRECTION_ACK_PACKET_HANDLER::Recieve(void* Buffer, void* Context)
{
    error("Should not recieve direction ACK on server");
}
