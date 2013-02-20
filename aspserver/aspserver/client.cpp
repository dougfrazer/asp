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

#include <assert.h>
#include <unordered_map>
#include <stdio.h>
#include <string.h>

#include "network.h"
#include "asppacket.h"
#include "client.h"
#include "world.h"


//*******************************************************************************
// Global Data (should I make a manager class?)
//*******************************************************************************
static std::unordered_map<int,CLIENT*> ConnectedClients;
typedef std::unordered_map<int,CLIENT*>::const_iterator MapIterator;

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
    ASP_PACKET* Packet = (ASP_PACKET*)Data;
    while(Size > 0) {
        Size -= sizeof(ASP_HEADER) + Packet->Header.Length;
        switch(Packet->Header.Type)
        {
            case LOGIN:
                {
                    ASP_LOGIN_PACKET* Data = (ASP_LOGIN_PACKET*)(Packet + 1);
                    HandleLogin(Data);
                    Packet = (ASP_PACKET*)(Data + 1);
                } break;
            case DIRECTION:
                {
                    ASP_DIRECTION_PACKET* Data = (ASP_DIRECTION_PACKET*)(Packet + 1);
                    HandleDirection(Data);
                    Packet = (ASP_PACKET*)(Data + 1);
                } break;
            case KEEPALIVE:
                {
                    HandleKeepalive();
                    Packet = Packet + 1;
                } break;
            case NONE:
            case LOGIN_ACK:
            case DIRECTION_ACK:
                printf("Error client sent a server-only packet type=%d\n", Packet->Header.Type);
                return;
            default:
                break;
        }
    }
    assert(Size == 0);
}
//*******************************************************************************

//*******************************************************************************
// Private Interface
//*******************************************************************************
void CLIENT::HandleKeepalive()
{
    printf("[%d] Got a keepalive\n", UserId);
}
//*******************************************************************************
void CLIENT::HandleDirection(ASP_DIRECTION_PACKET* Data)
{
    printf("[%d] Got a movement request\n", UserId);
    if(UserId == -1) {
        printf("[%d] User not logged in, can not process movement packet\n", UserId);
    }

    if(!World_SetPosition(Data->x, Data->y, UserId)) {
        printf("Error setting user %d to position (%d,%d)\n", UserId, Data->x, Data->y);
    }

}
//*******************************************************************************
void CLIENT::HandleLogin(ASP_LOGIN_PACKET* Data)
{
    char                  buffer[50];
    ASP_PACKET*           Packet;
    ASP_LOGIN_ACK_PACKET* LoginAck;
    MapIterator           Client;

    printf("[%d] Got a login request\n", UserId);
    Packet = (ASP_PACKET*)buffer;
    Packet->Header.Type = LOGIN_ACK;
    Packet->Header.Length = sizeof(ASP_LOGIN_ACK_PACKET);
    LoginAck = (ASP_LOGIN_ACK_PACKET*)(&Packet->Body[0]);
    
    Client = ConnectedClients.find(Data->UserId);
    if(Client == ConnectedClients.end()) {
        printf("[%d] Login with userid %d\n", UserId, Data->UserId);
        ConnectedClients.insert( { Data->UserId, this } );
        UserId = Data->UserId;
        World_SetInitialPosition(UserId);
        LoginAck->Success = true;
        LoginAck->UserId = UserId;
        LoginAck->x = 0;
        LoginAck->y = 0;
    } else {
        printf("[%d] User with id=%d already exists\n", UserId, Data->UserId);
        LoginAck->Success = false;
        LoginAck->UserId = Data->UserId;
        LoginAck->x = 0;
        LoginAck->y = 0;
        LoginAck->Error = 1; // TODO: enumerate errors
    }
    
    QueuePacket(buffer, sizeof(ASP_HEADER) + Packet->Header.Length);
}
//*******************************************************************************
void CLIENT::QueuePacket(char* buffer, size_t size)
{
    if(SendBufferSize + size >= TRANSMIT_SIZE) {
        Network_RequestTransmit();
    }
    assert(SendBufferSize + size < BUFFER_SIZE);

    memcpy(SendBuffer + SendBufferSize, buffer, size);
    SendBufferSize += size;
}
//*******************************************************************************
void CLIENT::GetWorldState()
{
    char   WorldState[1500];
    size_t WorldStateSize = 0;

    World_RequestState(WorldState, &WorldStateSize);

    memcpy(SendBuffer + SendBufferSize, WorldState, WorldStateSize);
    SendBufferSize += WorldStateSize;
}
//*******************************************************************************
void CLIENT::GetSendBuffer(char** Buffer, size_t* Size)
{
    // It is assumed we wipe out the data after this call
    *Buffer = &SendBuffer[0];
    *Size = SendBufferSize;
    SendBufferSize = 0;
}
//*******************************************************************************

