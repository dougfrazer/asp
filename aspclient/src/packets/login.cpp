//******************************************************************************
// Login Packet
// ------------
//   Handles login packets
//
// (c) April 2013
// @author Doug Frazer
//******************************************************************************

#include "library/network/packets/login.h"

static LOGIN_PACKET_HANDLER LoginHandler;

void LOGIN_PACKET_HANDLER::Recieve(void* Buffer, void* Context)
{
    //printf("Got a login request");
}

