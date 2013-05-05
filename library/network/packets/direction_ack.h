//******************************************************************************
// Direction Ack Packet
//
// (c) April 2013
// @author Doug Frazer
//******************************************************************************

#ifndef __DIRECTION_ACK_H__
#define __DIRECTION_ACK_H__

#include "library/ASPLib.h"
#include "library/network/PacketHandler.h"

class DIRECTION_ACK_PACKET_HANDLER : public PACKET_HANDLER
{
public:
    struct DATA {
        float  x;
        float  y;
        float  z;
        u32    UserId;
    };

public:
    u32 GetSize()   { return sizeof(DATA); }
    u32 GetId()     { return StringHash("DIRECTION_ACK"); } // TODO: would be cool to do this at build time

    virtual void Recieve(void* Buffer, void* Context);
};

#endif
