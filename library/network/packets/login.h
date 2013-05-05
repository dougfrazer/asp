//******************************************************************************
// Login Packet
// ------------
//   Handles login packets
//
// (c) April 2013
// @author Doug Frazer
//******************************************************************************

#ifndef __LOGIN_H__
#define __LOGIN_H__

#include "library/ASPLib.h"
#include "library/network/PacketHandler.h"

class LOGIN_PACKET_HANDLER : public PACKET_HANDLER
{
public:
    struct DATA {
        u32  UserId;
    };

public:
    u32 GetSize()   { return sizeof(DATA); }
    u32 GetId()     { return StringHash("LOGIN"); } // TODO: would be cool to do this at build time

    virtual void Recieve(void* Buffer, void* Context);
};

#endif
