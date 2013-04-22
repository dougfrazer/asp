//*****************************************************************************
// Packet Handler
// --------------
//   An interface for defining handlers for a certain type of packet.
//
//   There is a list of handlers, which contains every handler regardless 
//   of whether it is registered or not and a tree of registered handlers.
//
// (c) April 2013
//
// @author Doug Frazer
//*****************************************************************************

#ifndef __PACKET_HANDLER_H__
#define __PACKET_HANDLER_H__

#include "ASPLib.h"

class PACKET_HANDLER
{
public:
    PACKET_HANDLER();
    virtual ~PACKET_HANDLER() {}
public:
    void Register();
    void Unregister();

    static PACKET_HANDLER** FindHandlerLocation(u32 Id);
    static PACKET_HANDLER*  FindHandler(u32 Id);

    virtual void Recieve(void* Buffer, void* Context) = 0;

    virtual u32 GetId() = 0;
    virtual u32 GetSize() = 0;

    PACKET_HANDLER* Left;
    PACKET_HANDLER* Right;

    PACKET_HANDLER* Next;
    PACKET_HANDLER* Prev;
};

PACKET_HANDLER* PacketHandler_GetFirst();
PACKET_HANDLER* PacketHandler_GetNext(PACKET_HANDLER* Handler);
void            PacketHandler_RegisterAll();

#endif
