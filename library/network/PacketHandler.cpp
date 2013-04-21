//*****************************************************************************
// Packet Handler
// --------------
//   An interface for defining handlers for a certain type of packet.
//   
//   For more details see PacketHandler.h
//
// (c) April 2013
//
// @author Doug Frazer
//*****************************************************************************

#include "stdio.h"

#include "PacketHandler.h"

static PACKET_HANDLER* HandlerTree = null;
static PACKET_HANDLER* HandlerList = null;

//*****************************************************************************
PACKET_HANDLER::PACKET_HANDLER()
{
    Left = null;
    Right = null;

    // add it to a cyclic list
    if(HandlerList == null) {
        HandlerList       = this;
        HandlerList->Prev = this;
        HandlerList->Next = this;
    } else {
        this->Prev = HandlerList->Prev;
        this->Next = HandlerList->Prev->Next;
        HandlerList->Prev->Next = this;
        HandlerList->Prev = this;
    }
}
//*****************************************************************************
void PACKET_HANDLER::Register()
{
    // Add to a binary search tree
    PACKET_HANDLER** Node = FindHandlerLocation(GetId());
    if(Node == null) {
        HandlerTree = this;
    } else if(*Node != null) {
        error("Another packet already has this identifier");
        return;
    } else {
        *Node = this;
    }
}
//*****************************************************************************
void PACKET_HANDLER::Unregister()
{
    PACKET_HANDLER** Node = &HandlerTree;
    PACKET_HANDLER** Parent = null;
    for(uint i = 1; i > 0 && *Node != null && (*Node)->GetId() != GetId(); i <<= 1) {
        Parent = Node;
        Node = ((*Node)->GetId() & i) > 0 ? &(*Node)->Left : &(*Node)->Right;
    }
    if(Parent == null) {
        HandlerTree = HandlerTree->Left != null ? HandlerTree->Left : HandlerTree->Right != null ? HandlerTree->Right : null;
    } else if(*Node == null) {
        error("Error could not find packet handler in the binary search tree\n");
    } else if((*Node)->Left == null && (*Node)->Right == null) {
        (*Parent)->Right = *Node == (*Parent)->Right ? null : (*Parent)->Right;
        (*Parent)->Left = *Node == (*Parent)->Left ? null : (*Parent)->Left;
    } else if((*Node)->Left == null && (*Node)->Right != null) {
        *Node = (*Node)->Right;
    } else if((*Node)->Left != null && (*Node)->Right == null) {
        *Node = (*Node)->Left;
    } else {
        *Node = (*Parent)->Left == *Node ? (*Node)->Left : (*Node)->Right;
    }
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
PACKET_HANDLER* PacketHandler_GetFirst()
{
    return HandlerList;
}
//*****************************************************************************
PACKET_HANDLER* PacketHandler_GetNext(PACKET_HANDLER* Handler) {
    return Handler->Next == HandlerList ? null : Handler->Next; 
}
//*****************************************************************************
void PacketHandler_RegisterAll()
{
    PACKET_HANDLER* Handler = PacketHandler_GetFirst();
    for( ; Handler != null; Handler = PacketHandler_GetNext(Handler)) {
        Handler->Register();
    }
}
//*****************************************************************************
