//*****************************************************************************
// Packet Handler
// --------------
//   An interface for defining handlers for a certain type of packet.
//
// (c) April 2013
//
// @author Doug Frazer
//*****************************************************************************


#include "PacketHandler.h"

static PACKET_HANDLER* HandlerTree = null;

//*****************************************************************************
PACKET_HANDLER::PACKET_HANDLER()
{
    Left = null;
    Right = null;
}
//*****************************************************************************
void PACKET_HANDLER::Register()
{
    // Add to a binary search tree
    PACKET_HANDLER** Node = FindHandlerLocation(GetId());
    if(Node == null) {
        HandlerTree = this;
    } else if(*Node == null) {
        error("Another packet already has this identifier");
        return;
    } else {
        *Node = this;
    }
}
//*****************************************************************************
void PACKET_HANDLER::Unregister()
{
    // TODO: remove from binary search tree
}
//*****************************************************************************
PACKET_HANDLER** PACKET_HANDLER::FindHandlerLocation(u32 Id)
{
    PACKET_HANDLER** Node = &HandlerTree;
    for(uint i = 1; i > 0 && *Node != null && (*Node)->GetId() != Id; i <<= 1) {
        Node = ((*Node)->GetId() & i) > 0 ? &(*Node)->Left : &(*Node)->Right;
    }
    return Node;
}
//*****************************************************************************
PACKET_HANDLER* PACKET_HANDLER::FindHandler(u32 Id)
{
    PACKET_HANDLER** Handler = FindHandlerLocation(Id);
    return Handler == null ? null : *Handler;
}
//*****************************************************************************
