//******************************************************************************
// Login Ack Packet
// ------------
//   Handles login packets
//
// (c) April 2013
// @author Doug Frazer
//******************************************************************************

#include "ASPLib.h"
#include "PacketHandler.h"

class LOGIN_ACK_PACKET_HANDLER : PACKET_HANDLER
{
public:
    struct DATA {
        u32  UserId;
        u32  Success     : 1;
        u32  Error       : 15;
        u32  Unused      : 16;
        u32  x;
        u32  y;
    };

    u32 GetSize()   { return sizeof(DATA); }
    u32 GetId()     { return StringHash("LOGIN_ACK"); } // TODO: would be cool to do this at build time

    void Recieve(void* Buffer, void* Context);
};

