//******************************************************************************
// Keepalive Packet
//
// (c) April 2013
// @author Doug Frazer
//******************************************************************************

#include "keepalive.h"

void KEEPALIVE_PACKET_HANDLER::Recieve(void* Buffer, void* Context)
{
    printf("Got a keepalive packet\n");
}
