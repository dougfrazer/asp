//******************************************************************************
// Login Ack Packet Implementation
//
// (c) April 2013
// @author Doug Frazer
//******************************************************************************


#include "library/network/packets/login_ack.h"

static LOGIN_ACK_PACKET_HANDLER LoginAckHandler;

void LOGIN_ACK_PACKET_HANDLER::Recieve(void* Buffer, void* Context)
{
    error("Should not recieve Login Ack packets on server");
}
