//*****************************************************************************
// Packet Handler
// --------------
//   An interface for defining handlers for a certain type of packet.
//
// (c) April 2013
//
// @author Doug Frazer
//*****************************************************************************

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

    virtual void Recieve(void* Buffer) = 0;

    virtual u32 GetId() = 0;
    virtual u32 GetSize() = 0;
private:
    PACKET_HANDLER* Left;
    PACKET_HANDLER* Right;
};

PACKET_HANDLER* PacketHandler_GetHandler(u32 Id);
