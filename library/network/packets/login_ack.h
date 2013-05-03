//******************************************************************************
// Login Ack Packet
// ----------------
//   A response to a login packet
//
// (c) April 2013
// @author Doug Frazer
//******************************************************************************

#ifndef __LOGIN_ACK_H__
#define __LOGIN_ACK_H__

#include "ASPLib.h"
#include "network/PacketHandler.h"

class LOGIN_ACK_PACKET_HANDLER : public PACKET_HANDLER
{
public:
    struct DATA {
        u32  UserId;
        u32  Success     : 1;
        u32  Error       : 15;
        u32  Unused      : 16;
        u32  x;
        u32  y;
        u32  z;
    };

    u32 GetSize()   { return sizeof(DATA); }
    u32 GetId()     { return StringHash("LOGIN_ACK"); } // TODO: would be cool to do this at build time

    virtual void Recieve(void* Buffer, void* Context);
};

#endif
