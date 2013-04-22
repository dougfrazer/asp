//******************************************************************************
// Keepalive Packet
//
// (c) April 2013
// @author Doug Frazer
//******************************************************************************

#ifndef __KEEPALIVE_H__
#define __KEEPALIVE_H__

#include "ASPLib.h"
#include "network/PacketHandler.h"

class KEEPALIVE_PACKET_HANDLER : public PACKET_HANDLER
{
public:
    u32 GetSize()   { return 0; }
    u32 GetId()     { return StringHash("KEEPALIVE"); } // TODO: would be cool to do this at build time

    virtual void Recieve(void* Buffer, void* Context); 
};

#endif
