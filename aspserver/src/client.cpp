//*******************************************************************************
// CLIENT
//   A container class for an individual client.  A client is defined by their
//   IP address and the main network class will maintain a mapping from IP to
//   CLIENT and pass messages directly to their assosciated clients.
//   
// @author Doug Frazer
// Copyright (c) Doug Frazer
// February 2013
//*******************************************************************************

#include <stdio.h>
#include <string.h>

#include "network.h"
#include "client.h"
#include "world.h"

//*******************************************************************************
// Constructor/Deconstructor
//*******************************************************************************
CLIENT::CLIENT(sockaddr* _Address, socklen_t _AddressLength)
{
    UserId = -1;
    Address = *_Address;
    AddressLength = _AddressLength;
}
//*******************************************************************************
CLIENT::~CLIENT()
{
}
//*******************************************************************************

//*******************************************************************************
// Public Interface
//*******************************************************************************
void CLIENT::ProcessData(char* Data, int Size)
{
    Stream.RecievePackets(Data, Size, this);
}
//*******************************************************************************
void CLIENT::GetWorldState()
{
    // TODO: redesign
    char   WorldState[1500];
    size_t WorldStateSize = 0;

    World_RequestState(&Stream);

    assert(Stream.DataOffset + WorldStateSize < sizeof(Stream.Data));

    memcpy(Stream.Data + Stream.DataOffset, WorldState, WorldStateSize);
    Stream.DataOffset += WorldStateSize;
}
//*******************************************************************************
void CLIENT::Transmit(int sockfd)
{
    Stream.Transmit(sockfd, &Address);
}
//*******************************************************************************
