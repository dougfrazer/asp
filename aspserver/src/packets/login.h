//******************************************************************************
// Login Packet
// ------------
//   Handles login packets
//
// (c) April 2013
// @author Doug Frazer
//******************************************************************************

#include "ASPLib.h"
#include "PacketHandler.h"

class LOGIN_PACKET_HANDLER : PACKET_HANDLER
{
public:
    struct DATA {
        u32  UserId;
    };

public:
    u32 GetSize()   { return sizeof(DATA); }
    u32 GetId()     { return StringHash("LOGIN"); } // TODO: would be cool to do this at build time

    void Recieve(void* Buffer, void* Context);
};
