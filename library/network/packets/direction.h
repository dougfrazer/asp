//******************************************************************************
// Direction Packet
//
// (c) April 2013
// @author Doug Frazer
//******************************************************************************

#ifndef __DIRECTION_H__
#define __DIRECTION_H__

#include "library/ASPLib.h"
#include "library/network/PacketHandler.h"

class DIRECTION_PACKET_HANDLER : public PACKET_HANDLER
{
public:
    enum DIRECTION {
        NORTH,
        EAST,
        SOUTH,
        WEST,
    };

    struct DATA {
        DIRECTION Direction;
        u32       UserId;
    };

public:
    u32 GetSize()   { return sizeof(DATA); }
    u32 GetId()     { return StringHash("DIRECTION"); } // TODO: would be cool to do this at build time

    virtual void Recieve(void* Buffer, void* Context);
};

#endif
