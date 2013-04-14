//******************************************************************************
// Keepalive Packet
//
// (c) April 2013
// @author Doug Frazer
//******************************************************************************

#include "ASPLib.h"
#include "PacketHandler.h"

class KEEPALIVE_PACKET_HANDLER : PACKET_HANDLER
{
public:
    u32 GetSize()   { return 0; }
    u32 GetId()     { return StringHash("KEEPALIVE"); } // TODO: would be cool to do this at build time

    void Recieve(void* Buffer, void* Context);
};

