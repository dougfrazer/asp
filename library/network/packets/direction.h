//******************************************************************************
// Direction Packet
//
// (c) April 2013
// @author Doug Frazer
//******************************************************************************

#include "ASPLib.h"
#include "PacketHandler.h"

class DIRECTION_PACKET_HANDLER : public PACKET_HANDLER
{
public:
    struct DATA {
        u32  x;
        u32  y;
        u32  UserId;
    };

public:
    u32 GetSize()   { return sizeof(DATA); }
    u32 GetId()     { return StringHash("DIRECTION"); } // TODO: would be cool to do this at build time

    virtual void Recieve(void* Buffer, void* Context);
};

